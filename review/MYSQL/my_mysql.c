
#include "mysql/mysql.h"
#include <stdio.h>

int main()
{
	MYSQL mysql;
	MYSQL_RES* ret;
	MYSQL_ROW row;

	char *qurey = "select * from serverlist";

	if(!mysql_init(&mysql))
	{
		printf("mysql init failure\n");
	}

	if(!mysql_real_connect(&mysql, "127.0.0.1", "mosquito", "wz1313113", "server", 0 , NULL, 0))
	{
		printf("connect mysql failure!\n");
	}
	else
	{
		printf("connected\n");
	}	

	int exec = mysql_query(&mysql, qurey);

	if(exec)
	{
		printf("error: %s\n", mysql_error(&mysql));
	}
	else
	{
		int ret = mysql_use_result(&mysql);
		if(ret)
		{
			row = mysql_fetch_row(ret);
			int i = 0;
			for(i = 0; i < row; i++)
				printf("row: %s\n", row[i]);
		}
		mysql_free_result(ret);

	}
	mysql_close(&mysql);
	return 0;
}