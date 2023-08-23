#include "pch.h"
#include "public.h"
#include "Connection.h"

//��ʼ�����ݿ�����
Connection::Connection() {
	_conn = mysql_init(NULL);
}
Connection::~Connection() {
	if (_conn != NULL) mysql_close(_conn);
}
//�������ݿ�
bool Connection::connect(string ip, unsigned short port, string user,string password, string dbname) {
	MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, NULL, 0);
	return p != NULL;
}
//���²��� insert\delete\update
bool Connection::update(string sql) {
	if (mysql_query(_conn, sql.c_str())) {
		LOG("����ʧ�ܣ�" + sql);
		return false;
	}
	return true;
}
//��ѯ���� select
MYSQL_RES* Connection::query(string sql) {
	if (mysql_query(_conn, sql.c_str())) {
		LOG("��ѯʧ�ܣ�" + sql);
		return NULL;
	}
	return mysql_use_result(_conn);
}