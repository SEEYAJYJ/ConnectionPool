#pragma once
#include<string>
#include <mysql.h>
#include <ctime>
#include <iostream>
using namespace std;

/*mysql ���ݿ����*/
class Connection {
public:
	//��ʼ�����ݿ�����
	Connection();
	//�ͷ���Դ
	~Connection();
	//�������ݿ�
	bool connect(string ip, unsigned short port, string user, string password, string dbname);
	//���²��� insert\delete\update
	bool update(string sql);
	//��ѯ���� select
	MYSQL_RES* query(string sql);

	void setFreshAliveTime() {_alivetime=clock();} //ˢ��һ�����ӵ���ʼ����ʱ���
	clock_t getAliveTime() const { return clock() - _alivetime; }

private:
	MYSQL *_conn;
	clock_t _alivetime; //��¼����״̬��Ĵ��ʱ��
};