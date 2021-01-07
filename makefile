CC=g++
CFLAGS=-Wall -Werror

FILES = myshell.cpp compar.cpp comprocess.cpp builtins.cpp -Wc++11

myshell: myshell.cpp
	$(CC) -o myshell $(FILES) 

