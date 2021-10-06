SOURCES = $(wildcard *.c*)
HEADERS = $(wildcard *.h)
 
CC := $(shell which clang || which gcc)
 
%.ll : %.c* $(HEADERS)
	$(CC) -emit-llvm -S $<

%.bc : %.c* $(HEADERS)
	$(CC) -Xclang -disable-O0-optnone -emit-llvm $< -c
 
.PHONY : clean
clean :
	rm -f *.bc *.ll