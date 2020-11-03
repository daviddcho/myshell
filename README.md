# myshell
myshell is a command line interpreter that executes commands read from stdin or from a file.

myshell supports built-in commands, external commands from /bin and special shell commands like I/O redirection, pipes, and background execution.
## Usage
```
git clone https://github.com/daviddcho/myshell.git
make && ./myshell
```
## Examples
```
echo Hello, World!
cat README.md >> hello.txt
wc -w < hello.txt > output.txt
./a.out & ls -l 
ls -l | wc -c
