## collision
Same as the previous level, we have three files, the source code, the executable and the flag, waiting to be claimed.
## Solution
Looking at the source code, the first thing that catches our attention is the function "check\_password".
```c
unsigned long check_password(const char* p){        
	int* ip = (int*)p;
	int i;
	int res=0;
	for(i=0; i<5; i++){
		res += ip[i];
	}
	return res;
}
```
Lets break it down:<br>
This function recieves the string p as argument and returns an unsigned long value. The first thing it does is to convert the string, to an integer array. Based on that line alone, we assume that p is at least 4 characters wide, since integers are 4 bytes long, and each character is 1 byte.<br>
Then, i and res is declared. Since the loop goes from 0 to 4, that means that the ip len is at least 5, which would mean our string has to have at least 20 characters.<br>
The function sums all the values on the array and return the result.<br>
Taking a look at the main function we get:
```c
if(strlen(argv[1]) != 20){
	printf("passcode length should be 20 bytes\n");
	return 0;     
}
```
Which confirms our hipothesys, the first argument should be 20 characters wide. After that, we get:
```c
if(hashcode == check_password( argv[1] )){
	setregid(getegid(), getegid());
	system("/bin/cat flag");
	return 0;
}
```
So, all we have to do is to pass as the argument to the executable, a string that, when applied the hashing operation defined in "check\_password", we get the value on hashcode, which was defined above as 0x21DD09EC, or 568134124 in decimal. Dividing it by 5, we get 113626824.8, which means that if every int on our array was 113626824 we would be short on 4, that could be added on the last integer.<br>
On hex, our desired value is 06C5CEC8, its easier to work with hex since every two digits represent a byte, so the conversion is already made. All we have to do now is to type the characters with the hex values of 06, C5, CE and C8 five times in a row and on the last one, we substitute the last char, C8, to CC.<br>
One way to achieve that is to write the bytes instead of searching for the write characters. We can do this using printf.
```bash
printf '\x06\xc5\xce\xc8%.0s' {1..4} && printf '\x06\xc5\xce\xc8' 
```
this will print all the desired 20 bytes, since the program expects it as a command-line argument, we can pass it with
```bash
./col $(printf '\x06\xc5\xce\xc8%.0s' {1..4} && printf '\x06\xc5\xce\xcc')
```
And...<br>
**We get an error.**<br>
It seems like we didnt get the right pass code, but the math checks out, so what is wrong here?<br>
All our problems can be resumed on a single line
```c
int* ip = (int*)p;
```
When we write a string using bytes, the result will be exactly as expected, in memory we will have: 06,c5,ce,c8. But when converting a stringto an integer, because of byte ordering, there's actually two possible results, the conversion result could be 06c5cec8 or **c8cec506**. This behaviour is machine-dependant. <br>
### What does this mean, in practice?
This means that we **were not** typing the right passcode, our bytes were reversed!<br>
To get the right solution, we have to uninvert them. How do we do this? It's simple, we can simply revert the order they appear on our printf's.
```bash
./col $(printf '\xc8\xce\xc5\x06%.0s' {1..4} && printf '\xcc\xce\xc5\x06')
```
Success!
