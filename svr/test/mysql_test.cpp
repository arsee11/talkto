#include <mysql/mysql.h>
#include <iostream>

using namespace std;

int main()
{
	MYSQL* _mysql = mysql_init(0);
	MYSQL* _connect = mysql_real_connect(_mysql,"127.0.0.1", "root","114225","talkto",3306,0,0 );
	if(_connect == NULL)
	{
		cout<<mysql_error(_mysql)<<endl;
		return 1;
	}

	mysql_query(_connect, "select * from msg");
	MYSQL_RES *res = mysql_store_result(_connect);
	
	return 0;
}
