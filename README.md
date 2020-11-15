# myshell
myshell is a command line interpreter that executes commands read from stdin or from a file.

myshell supports built-in commands, external commands from /bin and special shell commands like I/O redirection, pipes, and background execution.

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


## Examples
```
echo Hello, World!
cat README.md >> hello.txt
wc -w < hello.txt > output.txt
./a.out & ls -l 
ls -l | wc -c
