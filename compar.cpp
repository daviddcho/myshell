#include "myshell.h"
#include <sstream>
#include <iterator>
#include <vector>


// parses the arguments from user input line
void parseLine(string line) {
  // Converting user input into command char 
  char command[line.size() + 1];
  strcpy(command, line.c_str());
  
  // Handler for when user enters nothing
  if (command[0] == '\0') {
    return;
  }
  // Tokenizing the input by spaces
  // ex. {"ls", "-l", "|", "wc"}
  char* command_token = strtok(command, " ");
  
  // Organizing command arguments as groups (vector of vector pointers) 
  vector<vector<char*> * > parsedGroups;
  
  // This is the starting group
  parsedGroups.push_back(new vector<char*>);

  int i = 0; // iterator for parsedGroup
  while (command_token != NULL) {
    if (isShellCommand(command_token)) {
      // Add NULL to the current group (not shell group);
      parsedGroups.at(i)->push_back(NULL); 
      // Create a new group for shell command
      parsedGroups.push_back(new vector<char*>);
      // Iterate to shell command
      i++;
      // Add shell command to shell group
      parsedGroups.at(i)->push_back(command_token);
      // Add null to shell group
      parsedGroups.at(i)->push_back(NULL);
    } else {
      // If this is the first group (there is no shell command), add to current group
      // Or if the last command  
      if (i == 0 || isShellCommand(parsedGroups.at(i-1)->at(0)) == true) {
        parsedGroups.at(i)->push_back(command_token);
      } else {
        // Push in a new group
        parsedGroups.push_back(new vector<char*>);
        // Iterate to new group
        i++;
        // Push command into new command group
        parsedGroups.at(i)->push_back(command_token);
      }
    }
    // Used to iterate through command token
    command_token = strtok(NULL, " ");
  }
  // Error check if iteration or group making messed up 
  if (parsedGroups.size() < i) {
    cout << "Error: parsed groups size and iteration don't add up." << '\n';
  }
  // Add NULL to the last group
  parsedGroups.at(i)->push_back(NULL); 
  
  processCommand(parsedGroups);
  return;
}


bool isShellCommand(char* c) {
  string shell[6] = {"<<", "<", "|", ">", ">>", "&"};
  for (string i : shell) {
    if (c == i) {
      return true;
    }
  }
  return false;
}

