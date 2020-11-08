#include "myshell.h"
#include <filesystem> // current_path()
#include <unistd.h> // getcwd
#include <limits.h> // PATH_MAX
#include <fstream> // for reading file
using namespace std;


string get_cur_path();


int main(int argc, char *argv[], char *envp[]) {

  string line;
  if (argc > 1) {
    // Get the batch file
    string filename = argv[1]; 
    ifstream file(filename); 

    if (file.is_open()) {
      while(getline(file, line)) {
        // Parse the line
        parseLine(line);
      }
      // Close the file
      file.close();
      exit(0);
    }
  // If there is no batch file
  } else {
    while(true) {
      cout << "myshell ~" << get_cur_path()  << "$ ";
      // Get user input 
      getline(cin, line);
      // Parse user input
      parseLine(line); 
    }
  }
  return 0;
}


// Returns current path
string get_cur_path() {
  char temp[PATH_MAX];
  return (getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

