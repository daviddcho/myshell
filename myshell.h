#ifndef MYSHELL_H
#define MYSHELL_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>

using namespace std;

// Command Parser
void parseLine(string line);
bool isShellCommand(char* s);
void addShellCommand(char* s, vector<char*> *shellCommands);

// Command Processor
void processCommand(vector<vector<char*> *> commandGroups);

// Built ins
void cd();
void cd(char* dest);
void clr();
void dir();
void dir(char* dest);
void environn();
void echo(vector<char*> input);
void help();
void mypause();
void quit();
void doBuiltIn(int builtcom, vector<vector<char*> *> command);
int isBuiltIn(char *c);
void builtInRedirection(int builtcom, vector<char*> *shellComamnd, vector<vector<char*> * > commandGroups);

// External Commands
void externalNoShell(vector<vector<char*> * > commandGroups);
void externalPipe(vector<vector<char*> * > commandGroups);
void externalBackG(vector<vector<char*> * > commandGroups);
void externalRedirection(vector<vector<char*> * > commandGroups);
// What else do I need?
#endif
