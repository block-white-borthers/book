#ifndef __BOOKDATABASE_H
#define __BOOKDATABASE_H

#define DBHOST 	   "localhost"  //数据库主机IP
#define DBPORT	   3306		//数据库端口
#define DBUSER	   "root"	//数据用户名
#define DBPASSWORD   "root"	//数据库密码
#define DBNAME     "db_books"   //数据库名
//#define BDBNAME		"db_books"	备用数据库名，用于自动创建(思路不清晰的结果)
#define TABLENAME  "tb_books"	//表名
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/mysql.h"

#if     _MSC_VER > 1000
#pragma comment(lib,"libmysql.lib")
#endif

/* define function */
int connectdatabase(MYSQL *con);/* connect database use macro */
void showhead();/* display format head */
void showall(); /* show mysql data all */
int showdata();/* show data from tb_book */
void addbook();/* add book item to tb_books */
void modifybook();/* modify book item to tb_books */
void deletebook();/* delete book item to tb_books */
void querybook();/* select book info from tb_books */
int allow(char *choose);/* allow to exec */
void inquire();/* return to menu */
int data_init();/**/

int runbooksystem();/* run system fun api */
/* define value */
struct book_info{
char id[10];/* book id */
char bookname[10];/* book name */
char author[10];/* book author */
char bookfrom[10];/* book from */

};/**/



#endif

