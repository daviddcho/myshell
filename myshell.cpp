#include "myshell.h"
#include <filesystem> // current_path()
#include <unistd.h> // getcwd
#include <limits.h> // PATH_MAX
#include <fstream> // for reading file
using namespace std;

string get_cur_path() {
  char temp[PATH_MAX];
  return (getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

// myshell 
int main(int argc, char *argv[], char *envp[]) {

  string line;
  
  if (argc > 1) {
    // get the batch file
    string filename = argv[1]; 
    ifstream file(filename); 
    // if the file is open
    if (file.is_open()) {
      // while getting line
      while(getline(file, line)) {
        // parse the line
        parseLine(line);
      }
      // close the file
      file.close();
      // exit program
      exit(0);
    }
  // if there is no batch file
  } else {
    while(true) {
      cout << "myshell ~" << get_cur_path()  << "$ ";
      // get user input 
      getline(cin, line);
      // parse user input
      parseLine(line); 
      if ((line == "exit") && (!cin.eof())) {
        exit(0);
      }
    }
  }
  return 0;
}
