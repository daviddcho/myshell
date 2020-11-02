#include "myshell.h"
#include <unistd.h>
#include <dirent.h>
#include <fstream>

// Builtin commands

// Takes you to home directory
void cd() {
  chdir("/home");
}

// Takes you to desired directory
void cd(char* dest) {
  if (chdir(dest) != 0) {
    cout << "Error: input directory does not exist." << '\n';
  }
}

// Clears the screen
void clr() {
  cout << "\e[1;1H\e[2J";
}

// Lists items in current dir
void dir() {
  DIR *dir;
  struct dirent *dp;
  if((dir = opendir(".")) == NULL) {
    cout << "Error: opendir() error." << '\n';
  } else {
    while ((dp = readdir(dir)) != NULL) {
      cout << dp->d_name << '\n';
    }
  }
  closedir(dir);
}

// Lists items in dest dir
void dir(char* dest) {
  DIR *dir;
  struct dirent *dp;
  if ((dir = opendir(dest)) == NULL) {
    cout << "Error: opendir() error." << '\n';
  } else {
    while ((dp = readdir(dir)) != NULL) {
      cout << dp->d_name << '\n';
    }
  }
  closedir(dir);
}

// Lists all the environment strings;
extern char **environ; 
void environn() {
  string path[] = {"USER", "LOGNAME", "HOME", "PATH", "PWD", "SHELL", "TERM"}; 

  for (string s : path) {
    cout << s << ":" << getenv(s.c_str()) << '\n';
  }
 /* 
  while(*environ) {
    printf("%s\n", *environ++);  
  }
  */
  return;
}

// Display input on the display followed by a new line
void echo(vector<char*> input) {
  for (int i = 1; i < input.size(); i++) {
    if (input.at(i) == NULL) {
      cout << '\n';
      return;
    }
    cout << input.at(i) << " ";
  }
  return;
}

// Display the user manual
void help() {
  string line;
  ifstream file("helpman.txt"); 
  if (file.is_open()) {
    while (getline(file, line)) {
      cout << line << '\n';
    }
    file.close();
  } else {
    cout << "Error: couldn't open help file." << '\n';
  }
  return;
}
// Pause the operation of the shell until any key is pressed
void mypause() {
  while (getchar()) {
    return;    
  }
}

// Quit the shell
void quit() {
  exit(0);
}


