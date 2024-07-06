#include "./lib/bookdatabase.h"
char ch[2];/*final  use in the bookdatabase.c */
MYSQL_ROW row;/* final */
MYSQL_RES *result;/* final */
MYSQL mysql;/* final */
//初始化数据库,判断是否存在用户数据库和用户表,如果不存在用户数据库和用户表,
//则创建数据库和表.如果没有安装mysql数据库,返回-1,程序将退出.
//这是一个检测函数，返回状态码
int data_init(){
	MYSQL_RES *res;
	MYSQL mysql;/**/
	char query[512]={0};
	if((mysql_init(&mysql))==NULL){
	   printf("初始化MYSQL对象失败,可能是内存不足\n");
	   return -1;}
	mysql_set_character_set(&mysql,"gbk");/* set charset don't have error encode */
	if(!connectdatabase(&mysql)){//数据库为NULL，如果连接失败,就是连接数据库失败或没安装数据库
	 	printf("连接数据库失败或没有安装数据库,请自行检查原因!\n");
		return -1;
		}
	if(mysql_select_db(&mysql,DBNAME)){//返回非0证明数据库不存在,我们来创建数据库。
		
		printf("您是新安装用户,数据库必须进行初始化,请稍等!\n");
		memset(query,0,sizeof(query));//alloc memory 
		snprintf(query,sizeof(query),"create database %s",DBNAME);// format string 
		/* create database statement  */
		if(mysql_real_query(&mysql,query,strlen(query))){
			printf("创建%s数据库失败!\n",DBNAME);
			return -1;
		}
		printf("创建%s数据库成功!\n",DBNAME);		
		mysql_select_db(&mysql,DBNAME);	
	}
	memset(query,0,sizeof(query));/* reset memory ? */
	snprintf(query,sizeof(query),"select * from %s",TABLENAME);
	/* select statement ?*/
		
	if(mysql_real_query(&mysql,query,strlen(query))){//返回非0证明数据库不存在,我们来创建数据表。
		memset(query,0,sizeof(query));/* reset memory query */
		snprintf(query,sizeof(query),"CREATE TABLE `%s`.`%s` \
				 ( `ID` VARCHAR(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书编号' ,\
			        `bookname` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书名' ,\
			       `author` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '作者' ,\
			       `bookfrom` VARCHAR(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '出版社' , PRIMARY KEY (`ID`)) ENGINE = MyISAM CHARSET=utf8 COLLATE utf8_general_ci COMMENT = '图书表';",DBNAME,TABLENAME);
		if(mysql_real_query(&mysql,query,strlen(query))){
			printf("创建%s数据表失败!\n",TABLENAME);
			return -1;
		}
	    printf("创建%s数据表成功!\n程序开始正常运行......\n",TABLENAME);
	}
	mysql_free_result(res=mysql_store_result(&mysql));//前面select*了一次，必须释放掉结果集。
	mysql_close(&mysql); // 关闭 Mysql 连接
	return 0;
}



int allow(char *choose)
{
	if (strcmp(choose,"y")== 0|| strcmp(choose,"Y")==0) 
	{
		return 1;/**/
	}
	else {
		return 0;/**/
	}

}
void menu()
{
system("cls");/**/
printf("************  welcome to book manage system  **********\n\n");/*  */
printf("************   欢迎来到图书管理系统   **********\n\n");/*  */
printf("\t1.打印\t 2.添加\t 3.更改\n\n");/*  */
printf("\t4.删除\t 5.查询\t 6.退出\n\n");/*  */
printf("*********************************\n");/*  */
printf("请输入想要进行的操作数：");/*  */

}

void inquire()
{//set  choose answer to app run
	printf("display menu?y/n:\n");/* tip  */
	scanf("%s",ch);/* use global value choose */
	if(allow(ch)){
	menu();/**/
	}else {
		exit(0);/* exit app */
	}
}
void showhead()
{
printf("==========================================\n");/* tip */
printf("\t           显示图书信息            \n");/* tip */
printf("=============================================\n");/* tip */
printf("\t图书编号 |图书名   |作者   |出版社   \n");/* tip */
printf("-------------------------------------------\n");/*tip  */
}

int connectdatabase(MYSQL *con)
{
	/* init con */ 
	if(!mysql_init(con) ) return -1;/**/


	if(!mysql_real_connect(con, DBHOST, DBUSER, DBPASSWORD, DBNAME, 0, NULL, 0 ))
	{
	 printf("can not connection to %s\n",DBNAME);/* tip */
	 return -1;/**/
	}
	else {
		mysql_set_character_set(con,"gbk");/**/

			printf("show\n");/*  */

		return 1;/**/
	}
}

int showdata(MYSQL_ROW res)
{
if(res)
{
fprintf(stdout, "\t %5s |  %5s |   %5s |   %5s | \n",res[0],res[1],res[2],res[3]);
printf("\t================================== \n");/* tip  */
return 0;/* */
}
else
return -1;/* can not row */
}

void showall()
{
/* connection database */
	if(!connectdatabase(&mysql))
	{
	printf("can not connection database \n");/* tip */
	}
	else{
	/* database connect success */
		if(mysql_query(&mysql, "select * from tb_books"))
		{
		printf("select table: %s  fail\n",TABLENAME);/* tip */
		}
		else{
			
		result = mysql_store_result(&mysql);/* get result set */
		if (mysql_num_rows(result) != NULL) {	
			/* have record ,have data */
			showhead();/*  show head */
			int i=1,count=0;/**/
			count=mysql_num_rows(result);/* get row nums */
			while (row = mysql_fetch_row(result)) {
				/* get record from result set */
			showdata(row);/**/
			/* paging */
			i++;/**/
			if(i%5==0){
			printf("keep any?y/n\n");/* tip */
			memset(ch,0,sizeof(ch));/* init */
			scanf("%s",ch);/*input answer */
			if(allow(ch)){
				i++;/**/
				printf("total %d\n",count);
				/* row counts,have useful in big rows paging,
					 but best use sql statement */
				showhead();/**/
				showdata(row);/**/
			}
			if(!allow(ch)){break;/**/}
			}
			}
		}
		else{
		printf("can not record \n");/* tip */
		}
		mysql_free_result(result);/* free result set */
		}
		mysql_close(&mysql);/* free connect */
	}
	inquire();/* get answer use in return to menu */
}

void addbook()
{
int rowcount;/* count row */
struct book_info  bi;/* book info */
char sql[1024];/**/
if (!connectdatabase(&mysql)) {
	printf("can not connect database \n");/*  */
}
else{
/* database connect success, insert data */
	showhead();/* show head tip */
	if(mysql_query(&mysql,"select * from tb_books"))
		printf("select fail\n");/* tip  */
	else{
	result = mysql_store_result(&mysql);/* get result set obj */
	rowcount = mysql_num_rows(result);/* get rows num from  result set   */
	row = mysql_fetch_row(result);/* get row data from  result set */
	printf("\tID:\n");/* tip */
	scanf("%s",bi.id);/* input	*/
	if(rowcount != NULL){

		do{
		if(!strcmp(bi.id,row[0])){
		printf("had rescord press any key \n");/* tip */
		getch();/* get char */
		mysql_free_result(result);/* free result */
		mysql_close(&mysql);/* close mysql */
		exit(0);/**/
		}
		}while(row = mysql_fetch_row(result));/* loop */
	}
	/* no identical number exists */
	printf("book name:\n");/* tip */
	scanf("%s",bi.bookname);/**/
	printf("book author\n");/* tip */
	scanf("%s",bi.author);/**/
	printf("book from:\n");/* tip */
	scanf("%s",bi.bookfrom);/* */
	snprintf(sql, sizeof(sql),"  INSERT INTO `tb_books` (`ID`, `bookname`, `author`, `bookfrom`) VALUES ('%s', '%s', '%s', '%s');",bi.id,bi.bookname,bi.author,bi.bookfrom);/* format scanf */
	if(mysql_real_query(&mysql,sql,strlen(sql))!=0){
	fprintf(stderr, "can not insert\n",mysql_error(&mysql));/* tip */
	}
	else{printf("success insert \n");/* tip  */ }
	mysql_free_result(result);/* free result set */
	}
	mysql_close(&mysql);/* close mysql */
}
inquire();/* return to menu */
}

void addbook2()
{
struct book_info bi;/* book info */
char sql[1024];/**/
if(!connectdatabase(&mysql))
{
printf("can not connect datbase \n");/* tip */
}
else{
printf("please enter need insert book id:\n");/* tip */
scanf("%s",bi.id);/* input */
snprintf(sql, sizeof(sql),"SELECT * FROM `tb_books` WHERE ID='%s' ", bi.id);/* format string */
if (mysql_real_query(&mysql,sql,strlen(sql))) {
	printf("select data fail\n");/* tip */
}
else{
result = mysql_store_result(&mysql);/* get result set */
if (mysql_num_rows(result) != NULL) {
	/* had record,had data	 */
	printf("had record\n");/*  */
	showhead();/* show head */
	while (row = mysql_fetch_row(result)){
	/* output the row data */
		showdata(row);/* show data */
	}

}else {
	printf("not found data\nyou can add!\n");/* tip */
		/* no identical number exists */
	printf("book name:\n");/* tip */
	scanf("%s",bi.bookname);/**/
	printf("book author\n");/* tip */
	scanf("%s",bi.author);/**/
	printf("book from:\n");/* tip */
	scanf("%s",bi.bookfrom);/* */
	snprintf(sql, sizeof(sql),"  INSERT INTO `tb_books` (`ID`, `bookname`, `author`, `bookfrom`) VALUES ('%s', '%s', '%s', '%s');",bi.id,bi.bookname,bi.author,bi.bookfrom);/* format scanf */
	if(mysql_real_query(&mysql,sql,strlen(sql))!=0){
	fprintf(stderr, "can not insert\n",mysql_error(&mysql));/* tip */
	}
	else{printf("success insert \n");/* tip  */ }
}

}

}
mysql_free_result(result);/* free result */
memset(sql,0,sizeof(sql));/*reset sql */
free(sql);/**/
free(&bi);/* */
mysql_close(&mysql);/* close mysql */
inquire();/* return to menu */

}

void modifybook(){
char sql[1024];/* use in update and select */
struct book_info bi;/* book info */
if(!connectdatabase(&mysql)){
printf("can not connect datavase \n");/* tip */
}else{
/* success connect database */
	printf("please enter need modify the book id:\n");/* tip */
	scanf("%s",bi.id);/* input */
	snprintf(sql, sizeof(sql)," SELECT * FROM `tb_books` WHERE ID=%s", bi.id);/* format string */
	if(mysql_real_query(&mysql,sql,strlen(sql)))
	{
	printf("select fail\n");/* tip */
	}
	else{
	result = mysql_store_result(&mysql);/* get result set */
	if(mysql_num_rows(result) != NULL)
	{
	/* have record ,have data */
		printf("find record info ,display?y/n\n");/* tip */
		scanf("%s",ch);/**/
		if(allow(ch))
		{
		showhead();/* show head title */
		while (row = mysql_fetch_row(result)) {
			/* output the info  */
			showdata(row);/* show data */
		}
		printf("\t==============\n");/* tip */
		printf("modify?y/n :\n");/* tip */
		scanf("%s",ch);/**/
		if(allow(ch)){
		printf("book name\n");/* tip */
		scanf("%s",bi.bookname);/* */
		printf("author:\n");/* tip */
		scanf("%s",bi.author);/**/
		printf("book from:\n");/* tip */
		scanf("%s",bi.bookfrom);/**/
		memset(sql,0,sizeof(sql));/**/
		snprintf(sql, sizeof(sql)," UPDATE `tb_books` SET `bookname`='%s',`author`='%s',`bookfrom`='%s' WHERE ID='%s' ",bi.bookname,bi.author,bi.bookfrom,bi.id );/* format string */
		if(mysql_real_query(&mysql,sql,strlen(sql)) !=0 )
		{
			fprintf(stderr, "can not modify record\n",mysql_error(&mysql));/* tip */
		}else{
		printf("modify success\n");/* tip */

		}
		
		}

		}
	}else{
	printf("not found need modify record\n");/* tip */
	}

	}
	mysql_free_result(result);/* free result */
	
}
mysql_close(&mysql);/**/
inquire();/**/
}

void deletebook()
{
struct book_info bi;/* book info */
char sql[1024];/**/
if (!connectdatabase(&mysql)) {
	printf("can not connect \n");/* tip */
}else {
	printf("please enter need delete book id\n");/* tip */
	scanf("%s",bi.id);/* input */
	snprintf(sql, sizeof(sql),"SELECT * FROM `tb_books` WHERE ID='%s'",bi.id);/*  format string  */
	if (mysql_real_query(&mysql,sql,strlen(sql))) {
		/* select fail  */
		printf("\nselect fail \n");/* tip */
	}else{
	result = mysql_store_result(&mysql);/* get result set */
	if(mysql_num_rows(result) != NULL)
	{
	/* have record ,have data */
		printf("found data,display?y/n\n");/* tip */
		scanf("%s",ch);/**/
		if (allow(ch)) {
			showhead();/* format head  */
			while((row = mysql_fetch_row(result)))
			{
			/* output the row record */
				showdata(row);/**/
			}
		}
		memset(ch,0,sizeof(ch));/*  reset memory */
		printf("delete data>y/n\n");/* tip */
		scanf("%s",ch);/**/
		if(allow(ch))
		{
		memset(sql,0,sizeof(sql));/* reset memory */
		snprintf(sql, sizeof(sql),"DELETE FROM `tb_books` WHERE ID='%s' ", bi.id);/* format data  */
		if (mysql_real_query(&mysql,sql,(strlen(sql)))!= 0) {
			fprintf(stderr, "\tcan not delete rrecord \n",mysql_error(&mysql));/*error */
		}else{
		printf("delete success\n");/* tip */
		}
		
		}
	}else{
	printf("not found need delete data\n");/* tip */
	}
	}
	mysql_free_result(result);/* free result set */
}
free(&bi);/**/
memset(sql,0,sizeof(sql));/**/
free(sql);/**/
mysql_close(&mysql);/* close mysql */
inquire();/* return to menu */
}
void querybook()
{
struct book_info bi;/* book info */
char sql[1024];/**/
if(!connectdatabase(&mysql))
{
printf("can not connect datbase \n");/* tip */
}
else{
printf("please enter need select book id:\n");/* tip */
scanf("%s",bi.id);/* input */
snprintf(sql, sizeof(sql),"SELECT * FROM `tb_books` WHERE ID='%s' ", bi.id);/* format string */
if (mysql_real_query(&mysql,sql,strlen(sql))) {
	printf("select data fail\n");/* tip */
}
else{
result = mysql_store_result(&mysql);/* get result set */
if (mysql_num_rows(result) != NULL) {
	/* had record,had data	 */
	showhead();/* show head */
	while (row = mysql_fetch_row(result)){
	/* output the row data */
		showdata(row);/* show data */
	}

}else {
	printf("not found data\n");/* tip */
}
mysql_free_result(result);/* free result */
}

}
memset(sql,0,sizeof(sql));/*reset sql */
free(sql);/**/
free(&bi);/* */
mysql_close(&mysql);/* close mysql */
inquire();/* return to menu */
}



int runbooksystem()
{
	
	int n;/**/
	menu();/* show menu */
	scanf("%d",&n);/* input */
	while(n){
	switch(n){
		case 1:showall();/* show all book info data */
		       break;/* */

		case 2: addbook2();/* add book info */
		       break;/**/
		case 3:modifybook();/* modify book info */
		       break;/* */

		case 4: deletebook();/*call delete book fun*/
			break;/* */

		case 5:querybook();/**/
		       break;/* */

		case 6:exit(0);/* quit app */
		default:break;/**/
	}
	scanf("%d",&n);/**/
	
	}
}
int data_init2()
{
	connectdatabase(&mysql);/**/
	mysql_select_db(&mysql,DBNAME);/* */
result =mysql_list_tables(&mysql,"tb_books");/**/
while(row =  mysql_fetch_row(result))
	printf("%s\n",row[0]);/*  */

//mysql_query(&mysql," CREATE TABLE `%s`.`tb_test` ( `ID` VARCHAR(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书编号' , `bookname` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书名' , `author` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '作者' , `bookfrom` VARCHAR(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '出版社' , PRIMARY KEY (`ID`)) ENGINE = MyISAM CHARSET=utf8 COLLATE utf8_general_ci COMMENT = '图书表';");


/*mysql_query(&mysql,"create database %s");*/


	printf("end\n");/* tip */
	mysql_free_result(result);/**/
	mysql_close(&mysql);/**/
	return 1;/* */
}


int backupconnectdatabase(MYSQL *con)
{/* the function use in data_init() */
	char sql[512];/**/
	/* init con */ 
	if(!mysql_init(con) ) return -1;/**/


	if(!mysql_real_connect(con, DBHOST, DBUSER, DBPASSWORD, NULL, 0, NULL, 0 ))
	{
	 printf("can not connection to mysql server and not connect to %s\n",DBNAME);/* tip */
	 return -1;/**/
	}
	else {
		/* create backup database */
		memset(sql,0,sizeof(sql));/* reset memory */
		snprintf(sql, sizeof(sql),"create database %s", DBNAME);/* format string */
		if(mysql_real_query(&mysql,sql,strlen(sql))){
		/* not return zero is create database fail */
			printf("create backup database fail\n");/* tip  */
			return -1;/**/
		}else{
			mysql_set_character_set(con,"gbk");/**/
			printf("show ,because create backup database success\n");/* tip */
			return 1;/**/
		}

	}
}

int data_init3()
{
	char sql[1024];/* sql statement  */

if(connectdatabase(&mysql) == (-1)){
	mysql_close(&mysql);/**/
	if(	backupconnectdatabase(&mysql) == (-1))/* connect backup database */
	{
		printf("create backup database %s fail\n",DBNAME);/* tip  */
	   	exit(0);/*exit app*/
	}
}/* connect database and backup database  */
if(mysql_select_db(&mysql,DBNAME)/* select database */){
	/* zero is success,don't create db_books
	 * and not zero do create db_books.tb_books */
printf("not found %s database , need create database \n",DBNAME);/*  tip */
/*  need create database db_books and create table tb_books */
memset(sql,0,sizeof(sql));/**/
snprintf(sql, sizeof(sql),"create database %s", DBNAME);/* format string  */
if(mysql_real_query(&mysql,sql,strlen(sql)))/* exec create database */
{/* return not zero is fail */ 
	printf("create %s database fail\n",DBNAME);/* tip */
	return 0;/**/
}else{
/* now create tb_books table, because before create db_books database success */
	memset(sql,0,sizeof(sql));/* reset memory */
	snprintf(sql, sizeof(sql),"CREATE TABLE `%s`.`%s` ( `ID` VARCHAR(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书编号' , `bookname` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书名' , `author` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '作者' , `bookfrom` VARCHAR(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '出版社' , PRIMARY KEY (`ID`)) ENGINE = MyISAM CHARSET=utf8 COLLATE utf8_general_ci COMMENT = '图书表';",DBNAME,TABLENAME);/* format string */
	if(mysql_real_query(&mysql,sql,strlen(sql))){
	/* not return zero is fail ,so */
		printf("create %s table fail \n",TABLENAME);/* tip  create table fail*/
		return 0;/* */
	}else{
		printf("create %s table success \n",TABLENAME);/* tip */
		return 1;/**/
	}
	
}

}else{
printf("database %s exist ,select %s table \n",DBNAME,TABLENAME);/* tip */
result = mysql_list_tables(&mysql,TABLENAME);/* get result set have more and more */
row    = mysql_fetch_row(result);/* get row data,see have or not have tb_books table  */
if(row){
/* have tb_books table */
printf("%s table exist ,database %s exist\n",TABLENAME,DBNAME);/* tip */
return 1;/**/
}else {
	/* need create table tb_books,because: have database db_books ,not have table tb_books */
	memset(sql,0,sizeof(sql));/**/
	snprintf(sql, sizeof(sql)," CREATE TABLE `%s`.`%s` ( `ID` VARCHAR(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书编号' , `bookname` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '图书名' , `author` VARCHAR(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '作者' , `bookfrom` VARCHAR(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '出版社' , PRIMARY KEY (`ID`)) ENGINE = MyISAM CHARSET=utf8 COLLATE utf8_general_ci COMMENT = '图书表';",DBNAME,TABLENAME);/* format string */
if(mysql_real_query(&mysql,sql,strlen(sql))){
/* return zero is success ,so */
	printf("can not create %s table,but have %s database \n",TABLENAME,DBNAME);/* tip not have table */
	return 0;/**/
}
printf(" create %s table success\n",TABLENAME);/* tip */
/* one more time to select tb_books */
if(mysql_list_tables(&mysql,TABLENAME))
{
printf("had %s table have %s database \n",TABLENAME,DBNAME);/* tip have table and database  */
return 1;/**/
}

}/* create table tb_books end ,because now: have database db_books , have table tb_books */


}

}
