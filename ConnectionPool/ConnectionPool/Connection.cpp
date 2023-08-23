#include "pch.h"
#include "public.h"
#include "Connection.h"

//初始化数据库连接
Connection::Connection() {
	_conn = mysql_init(NULL);
}
Connection::~Connection() {
	if (_conn != NULL) mysql_close(_conn);
}
//连接数据库
bool Connection::connect(string ip, unsigned short port, string user,string password, string dbname) {
	MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, NULL, 0);
	return p != NULL;
}
//更新操作 insert\delete\update
bool Connection::update(string sql) {
	if (mysql_query(_conn, sql.c_str())) {
		LOG("更新失败：" + sql);
		return false;
	}
	return true;
}
//查询操作 select
MYSQL_RES* Connection::query(string sql) {
	if (mysql_query(_conn, sql.c_str())) {
		LOG("查询失败：" + sql);
		return NULL;
	}
	return mysql_use_result(_conn);
}