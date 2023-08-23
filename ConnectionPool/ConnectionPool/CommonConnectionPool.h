/*���ӳع���ģ��*/
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
	shared_ptr<Connection> getConnection();	//���ⲿ�ṩ�ӿڣ������ӳ��л�ȡһ�����õĿ�������
private:
	ConnectionPool();	//����1 ���캯��˽�л�

	bool loadConfigFile();	//�������ļ�����������

	void produceConnectionTask();	//�����ڶ������߳��У�ר�Ÿ�������������
	void scannerConnectionTask();	//����һ���µĶ�ʱ�̣߳�ɨ�����Ŀ���ʱ�䳬��maxtime�Ľ��л���

	string _ip;					//mysql-ip��ַ
	unsigned short _port;		//mysql�˿ں�3306
	string _username;			//��¼�û���
	string _password;			//��¼����
	string _dbname;			//���ӵ����ݿ�����
	int _initSize;			//��ʼ��������
	int _maxSize;			//���������
	int _maxIdletime;			//������ʱ��
	int _connectionTimeout;			//��ȡ���ӵĳ�ʱʱ��
	queue<Connection*> _connectionQue; //�洢mysql���Ӷ���
	mutex _queueMutex;		//ά�������̰߳�ȫ�Ļ�����
	atomic_int _connectionCnt; //��¼������������connection���ӵ�������
	condition_variable cv;//���������������������������̺߳������߳�
};

