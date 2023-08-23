#include "pch.h"
#include<iostream>
#include"Connection.h"
#include"CommonConnectionPool.h"
using namespace std;


int main() {
#if 0
	//测试增删改查功能
	Connection conn;
	char sql[1024] = { 0 };
	sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
	conn.connect("172.20.22.131", 3306, "root", "1135tty*","chat");
	conn.update(sql);
#endif

#if 0
	ConnectionPool *cp = ConnectionPool::getConnectionPool();
	cp->loadConfigFile();
#endif

#if 0
	clock_t begin = clock();
	for (int i = 0; i < 5000; i++) {
		Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
		conn.connect("172.20.22.131", 3306, "root", "1135tty*", "chat");
		conn.update(sql);
	}
	clock_t end = clock();
	cout << end - begin << endl;
#endif

#if 0
	clock_t begin = clock();
	ConnectionPool *cp = ConnectionPool::getConnectionPool();
	for (int i = 0; i < 5000; i++) {
		shared_ptr<Connection> sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
		sp->update(sql);
	}
	clock_t end = clock();
	cout << end - begin << endl;
#endif

#if 1
	clock_t begin = clock();
	Connection conn;
	conn.connect("172.20.22.131", 3306, "root", "1135tty*", "chat");
	thread t1([]() {
		//ConnectionPool *cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			//shared_ptr<Connection> sp = cp->getConnection();
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			//sp->update(sql);
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("172.20.22.131", 3306, "root", "1135tty*", "chat");
			conn.update(sql);
			}
		});
	thread t2([]() {
		//ConnectionPool *cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			//shared_ptr<Connection> sp = cp->getConnection();
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			//sp->update(sql);
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("172.20.22.131", 3306, "root", "1135tty*", "chat");
			conn.update(sql);
			}
		});
	thread t3([]() {
		//ConnectionPool *cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			//shared_ptr<Connection> sp = cp->getConnection();
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			//sp->update(sql);
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("172.20.22.131", 3306, "root", "1135tty*", "chat");
			conn.update(sql);
			}
		});
	thread t4([]() {
		//ConnectionPool *cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++) {
			//shared_ptr<Connection> sp = cp->getConnection();
			//char sql[1024] = { 0 };
			//sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			//sp->update(sql);
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("172.20.22.131", 3306, "root", "1135tty*", "chat");
			conn.update(sql);
			}
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	clock_t end = clock();
	cout << end - begin << endl;
#endif
	return 0;
}

