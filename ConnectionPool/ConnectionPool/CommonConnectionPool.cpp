#include "public.h"
#include "Connection.h"
#include "CommonConnectionPool.h"

ConnectionPool::ConnectionPool() {
	//加载配置项
	if (!loadConfigFile()) {
		return;
	}
	//创建初始数量的连接
	for (int i = 0; i < _initSize; ++i) {
		Connection *p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->setFreshAliveTime();
		_connectionQue.push(p);
		_connectionCnt++;
	}
	//启动新的线程作为连接的生产者
	thread produce(bind(&ConnectionPool::produceConnectionTask, this)); //bind-绑定器
	produce.detach();
	//启动一个新的定时线程，扫描多余的空闲时间超过maxtime的进行回收
	thread scanner(bind(&ConnectionPool::scannerConnectionTask, this)); //bind-绑定器
	scanner.detach();
}

void ConnectionPool::produceConnectionTask() {
	for (;;) {
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty()) {
			cv.wait(lock); //队列不空，生产线程进入等待状态，并且释放锁
		}

		//连接数量没有到达上线，继续创建新的连接
		if (_connectionCnt < _maxSize){
			Connection *p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->setFreshAliveTime(); //刷新空闲起始时间
			_connectionQue.push(p);
			_connectionCnt++;
		}

		cv.notify_all(); //通知消费者线程可以连接了
	}
}

//线程安全懒汉单例模式
ConnectionPool* ConnectionPool::getConnectionPool() {
	static ConnectionPool pool; //静态初始化，由编译器自动进行lock和unlock
	return &pool;
}

bool ConnectionPool::loadConfigFile() {
	FILE *pf = fopen("mysql.cnf", "r");
	if (pf == NULL) {
		LOG("mysql.ini file is not exist");
		return false;
	}
	while (!feof(pf)) {
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		string str = line;
		int idx = str.find('=', 0);
		if (idx == -1) {
			continue;
		}
		int endIdx = str.find("\n", idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endIdx - idx - 1);
		if (key == "ip") {
			_ip = value;
		}
		else if (key == "port") {
			_port = stoi(value);
		}
		else if (key == "username") {
			_username = value;
		}
		else if (key == "password") {
			_password = value;
		}
		else if (key == "dbname") {
			_dbname = value;
		}
		else if (key == "initSize") {
			_initSize = stoi(value);
		}
		else if (key == "maxSize") {
			_maxSize = stoi(value);
		}
		else if (key == "maxIdletime") {
			_maxIdletime = stoi(value);
		}
		else if (key == "connectionTimeout") {
			_connectionTimeout = stoi(value);
		}
	}
}

//给外部提供接口，从连接池中获取一个可用的空闲连接
shared_ptr<Connection> ConnectionPool::getConnection() {
	unique_lock<mutex> lock(_queueMutex);
	while (_connectionQue.empty()) {
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout))) {
			if (_connectionQue.empty()) {
				LOG("空闲时间获取超时——获取连接失败");
				return NULL;
			}
		}
	}

	//智能指针析构时，会把connection资源直接delete掉，相当于调用析构函数，connection会被close。这里需要自定义shared_ptr的释放资源方式，把connection归还为queue中
	shared_ptr<Connection> sp(_connectionQue.front(),[&](Connection *pcon) {
		//这里是在服务器应用线程中调用的，要考虑队列的线程安全操作
		unique_lock<mutex> lock(_queueMutex);
		pcon->setFreshAliveTime();
		_connectionQue.push(pcon);
	});
	_connectionQue.pop();
	cv.notify_all();	//消费完连接，通知生产者线程检查一下
	return sp;
}

//扫描多余的空闲时间超过maxtime的进行回收
void ConnectionPool::scannerConnectionTask() {
	for (;;) {
		//模拟定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdletime));

		//扫描队列，释放多余连接
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize) {
			Connection *p = _connectionQue.front();
			if (p->getAliveTime() > _maxIdletime * 1000) {
				_connectionQue.pop();
				_connectionCnt--;
				delete p; //调用析构函数释放连接
			}
			else {
				break; //队头连接没有超过max，其它连接肯定没有
			}
		}
	}
}