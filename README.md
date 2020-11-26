# myshell
myshell is a command line interpreter that executes commands read from stdin or from a file.

myshell supports built-in commands, external commands from /bin and Unix utility commands like I/O redirection, pipes, and background execution.

## Usage
```
git clone https://github.com/daviddcho/myshell.git
make && ./myshell
```

## Built-in commands 
- cd \<dir\>: Changes the current directory to the home directory or \<dir\>.
- clr: Clears the screen.
- dir \<dir\>: Lists all contents of the current directory or \<dir\>.
- environ: Lists all environment variables.
- echo <comment>: Prints <comment> to standard output followed by a new line.
- help: Prints the user manual.
- pause: Pauses the operation of the shell until any key is pressed.
- quit: Quits the shell.

## External Commands
myshell executes external commands by using fork(), execvp(), and wait().
The fork() system call creates a child process (a copy of the parent parent process). This child process runs the execvp() call that changes the current process into executing the external command. The parent process waits for the child process to complete executing its command using the wait() call.

## Unix Utility Commands
### I/O redirection (<, >, >>)
This Unix utility can redirect standard input and/or standard output to/from a file rather than to/from a keyboard or display.
```
myshell$ cmd arg1 < input.txt > output.txt
```
### Pipes (|)
```
grep error comprocess.cpp | wc -w
```

### Background execution (&)
```
ping google.com &
```


