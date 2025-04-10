## mistake
This level is all about order of precedence.

## Solution
Looking at the main function
```c
int fd;
if(fd=open("/home/mistake/password",O_RDONLY,0400) < 0){
	printf("can't open password %d\n", fd);
	return 0;
}

printf("do not bruteforce...\n");
sleep(time(0)%20);

char pw_buf[PW_LEN+1];
int len;
if(!(len=read(fd,pw_buf,PW_LEN) > 0)){
	printf("read error\n");
	close(fd);
	return 0;
}

char pw_buf2[PW_LEN+1];
printf("input password : ");
scanf("%10s", pw_buf2);

// xor your input
xor(pw_buf2, 10);

if(!strncmp(pw_buf, pw_buf2, PW_LEN)){
	printf("Password OK\n");
	setregid(getegid(), getegid());
	system("/bin/cat flag\n");
}
```
All it does is to compare our input xor'd with what is written on the file "password". If they match, then we get the key. The thing is, there are two problematic lines here
```c
if(fd=open("/home/mistake/password",O_RDONLY,0400) < 0){
```
and
```c
if(!(len=read(fd,pw_buf,PW_LEN) > 0)){
```
The second doesn't give us much advantage, but the first is really dangerous, and we can use it in our favour. The intended behaviour of the line is 
```c
(fd = open()) < 0;
```
But this is not what it is doing, instead, what it does is 
```c
fd = (open() < 0);
```
Which means that fd will be either 0 or 1, and guess what? In linux, fd 0 is **stdin** and fd 1 is **stdout**. If we can make open() return a value greater than or equal to 0, we can easily break this program, because it will compare two inputs we give it.<br>
Looking at the man page of the open function, on the return value section, we get
```
RETURN VALUE
       On  success, open(), openat(), and creat() return the new file descriptor (a nonnegative in
       teger).  On error, -1 is returned and errno is set to indicate the error.
```
So, if the function succeeds, a nonnegative value is returned, this is **exactly** what we want. If we execute the program, it will read two inputs, we just have to give it any 10 character wide string plus its xor'd version.
```c
#define XORKEY 1

void xor(char* s, int len){
        int i;
        for(i=0; i<len; i++){
                s[i] ^= XORKEY;
        }
}
```
To understand how to construct its xor'd version, we check the xor() function, all it does is to xor the first len characters of the string s with 1. This only alters the least significant bit, and in short, increments by 1 an even value and decrements by 1 an odd value. So, if we were to give the string "cccccccccc" applying the xor() function, the result would be "bbbbbbbbbb".<br>
Success!
