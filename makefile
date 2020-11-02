CC=g++
CFLAGS=-Wall -Werror

FILES = myshell.cpp compar.cpp comprocess.cpp builtins.cpp

myshell: myshell.cpp
	$(CC) -o myshell $(FILES) 

