#pragma once
#include<string>
#include <mysql.h>
#include <ctime>
#include <iostream>
using namespace std;

/*mysql 数据库操作*/
class Connection {
public:
	//初始化数据库连接
	Connection();
	//释放资源
	~Connection();
	//连接数据库
	bool connect(string ip, unsigned short port, string user, string password, string dbname);
	//更新操作 insert\delete\update
	bool update(string sql);
	//查询操作 select
	MYSQL_RES* query(string sql);

	void setFreshAliveTime() {_alivetime=clock();} //刷新一下连接的起始空闲时间点
	clock_t getAliveTime() const { return clock() - _alivetime; }

private:
	MYSQL *_conn;
	clock_t _alivetime; //记录空闲状态后的存活时间
};