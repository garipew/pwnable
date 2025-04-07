## fd
When connecting to the level, we see that there are three files on the profile. A binary "fd", its source code "fd.c" and our objective, the "flag".

### Solution
Reading the source code, we see that it first read some file descriptor and store its content on a buffer. Then, if "LETMEWIN\n" was written on the buffer, the program then cats the flag.
```c
int len = 0;
len = read(fd, buf, 32);
if(!strcmp("LETMEWIN\n", buf)){
	printf("good job :)\n");
	setregid(getegid(), getegid());
	system("/bin/cat flag");
	exit(0);
}
```
And if we look previously, at the definition of fd, we get:
```c
int fd = atoi( argv[1] ) - 0x1234;
```
So, all we have to do is to use a file descriptor that we can manipulate to give it the write passcode, like, for instance the stdin. On linux, the value of stdin is 0, so, all we have to do is to make fd = 0.<br>
Since 0x1234 is the hex value of 4660, all we have to do is execute the binary, passing 4660 as a command line argument.
```bash
./fd 4660
```
Then, the program will wait for input, we give the passcode expected "LETMEWIN\n", and recieve the flag back. 
