#for use with c++ files
.SUFFIXES : .cpp

#Compiler
CC = gcc

#use -g for gnu debugger and -std= for c++11 compiling
CXXFLAGS = ''

######Change to match all .cpp files.  Do not include .h files####
OBJS = main.o rb_node.o test_suite.o

TARGET = a.out

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.cpp.o:
	$(CC) -c $(CXXFLAGS) $(INCDIR) $<

clean:
	rm -f $(OBJS) $(TARGET) core