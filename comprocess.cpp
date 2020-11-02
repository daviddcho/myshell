#include "myshell.h"
#include <fcntl.h>
#include <unistd.h>
#include <typeinfo>
#include <sys/wait.h>

void processCommand(vector<vector<char*> * > commandGroups) {
  
  // builtin ID in a way
  int builtcom; 
  // If the first command is a built in command
  if ((builtcom = isBuiltIn(commandGroups.at(0)->at(0))) != -1) {
    if (commandGroups.size() > 1) {
      // Storing command.at(1) (the shell commands) into a more convenient name
      vector<char*> *shellCommand = commandGroups.at(1);

      // Handle redirection for Built In 
      builtInRedirection(builtcom, shellCommand, commandGroups);
      return;
    } else {
      // Do a regular built in command with no add ons
      doBuiltIn(builtcom, commandGroups);
    }
  // If not a Built In command
  } else { 
    // This means the user put in a shell command
    if (commandGroups.size() > 1) {
      // Pipe
      if (strcmp(commandGroups.at(1)->at(0), "|") == 0) {
        externalPipe(commandGroups);
        return;
      // Background
      } else if (strcmp(commandGroups.at(1)->at(0), "&") == 0) {
        externalBackG(commandGroups);
        return;
      // STDIN Redirection 
      } else if (strcmp(commandGroups.at(1)->at(0), "<") == 0 || strcmp(commandGroups.at(1)->at(0), ">") == 0 || strcmp(commandGroups.at(1)->at(0), ">>") == 0) {
        externalRedirection(commandGroups);
        return;
      } else { 
        cout << "Error: external commands only support |, &, <, >, and >>." << '\n';
        return;
      }
    // If there are no shell commands, just one external command
    } else {
      externalNoShell(commandGroups);    
      return;
    }
  }
  return;  
}

// Handles redirection for external commands 
// I think this can be a little more compact tho, a lot of reused code
// stdin 0 stdin 1 stderr 2
void externalRedirection(vector<vector<char*> * > commandGroups) {

  vector<char*> *input = commandGroups.at(2);

  int newstdout;
  int newstdin;

  // Handles if there is an output redirection too 
  if (commandGroups.size() > 3) {
    // Get output file from command group
    vector<char*> *output = commandGroups.at(4);
    if (strcmp(commandGroups.at(3)->at(0), ">") == 0) {
      // Create new stdout for the output file 
      newstdout = open(output->at(0), O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    } else if (strcmp(commandGroups.at(3)->at(0), ">>") == 0) {
      // Create new stdout for output file 
      // If file exists: append, if not: create file
      newstdout = open(output->at(0), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    } else {
      cout << "Error: external redirection only supports > and >>" << '\n';
    }
    // Create new stdin for input file 
    newstdin = open(input->at(0), O_RDONLY, S_IRUSR | S_IRGRP); 

    if (newstdin == -1) {
      cout << "Error: Cannot open your input file" << '\n'; 
      return;
    }

    // Saved the original stdin and stdout
    int stdin_saved = dup(0);
    int stdout_saved = dup(1);
    // Replace the stdin and stdout with the new ones
    dup2(newstdin, 0);
    dup2(newstdout, 1);
    // Close the new stdin and out
    close(newstdin);
    close(newstdout);
    // Execute command 
    externalNoShell(commandGroups);
    // Replace the new stdin and out with the saved (original) stdin and out
    dup2(stdin_saved, 0);
    dup2(stdout_saved, 1);
    // Close the saved stdin and out
    close(stdin_saved);
    close(stdout_saved);
  // If its just one redirection
  } else {
    // If its STDIN redirection 
    if (strcmp(commandGroups.at(1)->at(0), "<") == 0) {
      // Create new stdin for input file
      newstdin = open(input->at(0), O_RDONLY, S_IRUSR | S_IRGRP); 

      if (newstdin == -1) {
        cout << "Error: Cannot open your input file" << '\n'; 
        return;
      }
      // Save the original stdin
      int stdin_saved = dup(0);
      // Replace the stdin with newstdin
      dup2(newstdin, 0);
      // Close newstdin
      close(newstdin);
      // Execute command
      externalNoShell(commandGroups);
      // Replace current stdin with saved (original)  stdin
      dup2(stdin_saved, 0);
      // Close saved stdin
      close(stdin_saved);
    // If its STDOUT redirection
    } else if (strcmp(commandGroups.at(1)->at(0), ">") == 0) {
      // Get output file from command groups
      vector<char*> *output = commandGroups.at(2);
      // Create new stdout for output file
      newstdout = open(output->at(0), O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
      // Save the original stdout
      int stdout_saved = dup(1);
      // Replace stdout with newstdout
      dup2(newstdout, 1);
      // Close new stdout
      close(newstdout);
      // Execute command
      externalNoShell(commandGroups);
      // Replace stdout with saved (original) stdout
      dup2(stdout_saved, 1);
      // Close saved stdout
      close(stdout_saved);
    } else if (strcmp(commandGroups.at(1)->at(0), ">>") == 0) {
      // Refer to comments above, they are the same
      vector<char*> *output = commandGroups.at(2);
      // Added an append flag 
      newstdout = open(output->at(0), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
      int stdout_saved = dup(1);
      dup2(newstdout, 1);
      close(newstdout);
      externalNoShell(commandGroups);
      dup2(stdout_saved, 1);
      close(stdout_saved);
    } else {
      cout << "Error: external commands only support <, >, and >>." << '\n';
    }
  }
  return; 
}

// Executes external commmands with background processing
void externalBackG(vector<vector<char*> * > commandGroups) {
  for (int i = 0; i < commandGroups.size(); i += 2) {
    char *argsv[commandGroups.at(i)->size()];
    // Put contents of current group into argsv  
    for (int j = 0; j < commandGroups.at(i)->size(); j++) {
      argsv[j] = commandGroups.at(i)->at(j);
    }
    int pid = fork();
    if (pid == -1) {
      // Error 
      cout << "Error: somethign wrong with PID." << '\n';
    } else if (pid == 0) {
      // Child process
      if (execvp(argsv[0], argsv) == -1) {
        // Error
        cout << "Error: something wrong with execvp " << i << '\n';
        return;
      }
    } else {
      // Wait for all child processes to finish
      waitpid(-1, NULL, 0);
    }
  }
  return;
}

// Executes external commands with piping
void externalPipe(vector<vector<char*> * > commandGroups) {
  char *argsv1[commandGroups.at(0)->size()];
  for (int i = 0; i < commandGroups.at(0)->size(); i++) {
    argsv1[i] = commandGroups.at(0)->at(i);
  }
  char *argsv2[commandGroups.at(2)->size()];
  for (int j = 0; j < commandGroups.at(2)->size(); j++) {
    argsv2[j] = commandGroups.at(2)->at(j);
  }
  
  // create file descriptor
  int fd[2];
  if(pipe(fd) == 0) {
    int pid1 = fork();
    if (pid1 == -1) {
      // print error
      cout << "Error: something wrong with PID." << '\n';
      return;
    } else if (pid1 == 0) {
      // child process
      close(fd[0]); // closing pipes read end 
      // wire pipe's write end to stdout 
      if (dup2(fd[1], 1) == -1) {
        cout << "Error: something wrong with dup2 in pid 1." << '\n';
        return;
      } 
      close(fd[1]); // close pipes write end
      if (execvp(argsv1[0], argsv1) == -1) {
        // print error 
        cout << "Error: something wrong with execvp 1." << '\n';
        return;
      }
    } else {
      waitpid(-1, NULL, 0);
    }

    int pid2 = fork();
    if (pid2 == -1) {
      // print error 
      cout << "Error: something wrong with PID2." << '\n';
      return;
    } else if (pid2 == 0) {
      close(fd[1]); // close pipes write end
      // write pipe's read end to stdin
      if (dup2(fd[0], 0) == -1) {
        cout << "Error: something something dup2 in pid2." << '\n';
        return;
      }
      close(fd[0]); // close pipes read end
      if (execvp(argsv2[0], argsv2) == -1) {
        // print error 
        cout << "Error: something wrong with execvp 2." << '\n';
        return;
      }
    }/* else {
      int status;
      wait(&status);
    }*/
    close(fd[0]);
    close(fd[1]);
  } else {
    // print error
    cout << "Error: pipe didnt equal 0, prolly a error." << '\n';
    return;
  }
  // i guess this is the parent??
  // -1 signifies to wait for ALL children to finish
  waitpid(-1, NULL, 0);
}

// Execute just one external command 
void externalNoShell(vector<vector<char*> * > commandGroups) {
  // Making an array of char pointers 
  char *argsv[commandGroups.at(0)->size()];
  // Transfering the commands args into the array i just made
  for (int i = 0; i < commandGroups.at(0)->size(); i++) {
    argsv[i] = commandGroups.at(0)->at(i);
  }
  
  int pid = fork();
  if (pid == -1) {
    // Print error 
    cout << "Error: something wrong with PID." << '\n';
    return;
  } else if (pid == 0) {
    // This is the child process
    if (execvp(argsv[0], argsv) == -1) {
      // Print error
      cout << "Error: something wrong with execvp." << '\n';
      return;
    }
  } else {
    // Parent process
    int status = 0;
    wait(&status);
  }
}

// Handles redirection for Built In commands
// stdin 0 stdout 1 stderr 2
void builtInRedirection(int builtcom, vector<char*> *shellCommand, vector<vector<char*> * > commandGroups) {

  vector<char*> *args1 = commandGroups.at(0);
  vector<char*> *args2 = commandGroups.at(2);

  int newstdout;
  if (strcmp(shellCommand->at(0), ">") == 0) {
    // redirect stdout to output file
    // overwrites file, no append
    newstdout = open(args2->at(0), O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
  } else if (strcmp(shellCommand->at(0), ">>") == 0) {
    // appends
    newstdout = open(args2->at(0), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  } else {
    cout << "Error: built in commands support > and >> redirection" << '\n';
    return;
  }

  // save stdout
  int stdout_saved = dup(1);
  // set fd 1 to newstdout 
  dup2(newstdout, 1);
  close(newstdout);
  // run command
  doBuiltIn(builtcom, commandGroups);
  // set fd stdout_saved to 1 
  dup2(stdout_saved, 1);
  close(stdout_saved);

  return;
}

int isBuiltIn(char *c) {
  string builtIns[9] = {"cd", "clr", "dir", "environ", "echo", "help", "pause", "quit", "exit"};
  int i = 0;
  for (string s : builtIns) {
    if (c == s) {
      return i;
    }
    i++;
  }
  return -1;
}

void doBuiltIn(int builtcom, vector<vector<char*> *> commandGroups) {
   switch (builtcom) {
      case 0: // cd 
        if (commandGroups.at(0)->size() > 2) {
          cd(commandGroups.at(0)->at(1));
        } else {
          cd();
        }
        break;
      case 1: // clr
        clr();
        break;
      case 2: // dir
        if (commandGroups.at(0)->size() > 2) {
          dir(commandGroups.at(0)->at(1));
        } else {
          dir();
        }
        break;
      case 3: // environ
        environn();
        break;
      case 4: // echo
        echo(*commandGroups.at(0));
        break;
      case 5: // help
        help();
        break;
      case 6: // pause
        mypause();
        break;
      case 7: // quit
        quit();
        break;
      case 8:
        quit();
        break;
      }
  return; 
}
