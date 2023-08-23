/*连接池功能模块*/
#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include "Connection.h"
#include <thread>
#include <memory>
#include <condition_variable>
#include <functional>
using namespace std;

class ConnectionPool {
public:
	static ConnectionPool* getConnectionPool();	
	shared_ptr<Connection> getConnection();	//给外部提供接口，从连接池中获取一个可用的空闲连接
private:
	ConnectionPool();	//单例1 构造函数私有化

	bool loadConfigFile();	//从配置文件加载配置项

	void produceConnectionTask();	//运行在独立的线程中，专门负责生产新连接
	void scannerConnectionTask();	//启动一个新的定时线程，扫描多余的空闲时间超过maxtime的进行回收

	string _ip;					//mysql-ip地址
	unsigned short _port;		//mysql端口号3306
	string _username;			//登录用户名
	string _password;			//登录密码
	string _dbname;			//连接的数据库名称
	int _initSize;			//初始化连接量
	int _maxSize;			//最大连接量
	int _maxIdletime;			//最大空闲时间
	int _connectionTimeout;			//获取连接的超时时间
	queue<Connection*> _connectionQue; //存储mysql连接队列
	mutex _queueMutex;		//维护队列线程安全的互斥锁
	atomic_int _connectionCnt; //记录连接所创建的connection连接的总数量
	condition_variable cv;//设置条件变量，用于连接生产线程和消费线程
};

