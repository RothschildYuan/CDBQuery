PROGRAM = CDBTest             # 最后的可执行文件

SOURCES  = $(wildcard *.cpp)          # 通配当前目录下.cpp文件

OBJS    += $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))      #把$(SOURCES)中的变量符合后缀是.cpp的全部替换成.o

GXX = g++ 
GCC = gcc 

CFLAGS  = -g -o2 -DDEBUG -Wall -Wextra 

CCFLAGS  = -std=c++11 -g -o2 -DDEBUG -Wall -Wextra 
CCFLAGS  += -Wno-deprecated	
CCFLAGS  += -D linux -D ORM_OF_MYSQL -D ORM_OF_PGSQL -D ORM_OF_SQLITE
CCFLAGS  += 

INCLUDE = -I ./ -I ../include -I ./common -I ./sqlConnPool 
INCLUDE += -I ./include-internal -I ./include-internal/mysql -I ./include/postgresql

LIB1 = -L. ../lib/libmysql/libmysqlclient.so
LIB2 = -L. ../lib/libpq/libpq.so 	
LIB3 = -L. ../libCDBQuery.so
LIB4 = 
LIB5 = -L -lutil -lpthread -ldl -lrt
LIB6 = 
LIB7 = 
LIB = $(LIB1) $(LIB2)  $(LIB3) $(LIB4) $(LIB5) $(LIB6) $(LIB7)

$(PROGRAM):$(OBJS)
	$(GXX) $(LINK) $^ $(INCLUDE) $(CCFLAGS) $(LIB) -o $@

%.o:%.cpp
	$(GXX) -fPIC -c $< $(CCFLAGS) $(INCLUDE) -o $@

%.o:%.c
	$(GCC) -fPIC -c $< $(CFLAGS) $(INCLUDE) -o $@
	
.PHONY: install clean cleanall
install: $(PROGRAM)
	cp $(PROGRAM) ./bin/
clean : 
	rm -rf $(PROGRAM)
cleanall : 
	rm -rf $(PROGRAM)  *.o ./test/*.o ./sqlConnPool/*.o 
