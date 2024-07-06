all:main

main:main.c bookdatabase.c
	windres -o resources.o resources.rc
	gcc -fPIC -shared bookdatabase.c lib\libmysql.a -o libbookdatabase.so -finput-charset="UTF-8" -fexec-charset="gbk"
	gcc main.c libbookdatabase.so lib\libmysql.a resources.o -O1 -o main.exe -finput-charset="UTF-8" -fexec-charset="gbk"


