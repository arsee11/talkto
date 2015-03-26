#include <mysql.h>

int main()
{
	MYSQL* _mysql = mysql_init(0);
	MYSQL* _connect = mysql_real_connect(_mysql, ip, user, pwd,db, port, 0, 0);
	mysql_query(_connect, query_str);
	MYSQL_RES *res = mysql_store_result(_connect);
	
	return 0;
}