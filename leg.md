## leg
In this level, we get two source codes of the leg program. This is all about reading scary assembly.
## Solution
Looking at the C source code
```c
int key=0;
printf("Daddy has very strong arm! : ");
scanf("%d", &key);
if( (key1()+key2()+key3()) == key ){
	printf("Congratz!\n");
	int fd = open("flag", O_RDONLY);
	char buf[100];
	int r = read(fd, buf, 100);
	write(0, buf, r);
} 
```
What we have to do is to somehow match with our input, the result of the sum of the return values of key1(), key2() and key3(). Lets take a look at these functions then
### key1()
```c
int key1(){
        asm("mov r3, pc\n");
}
```
Well, thats actually pretty simple, all this function does is move the r3 register to the pc register. In ARM, the value of the pc register is the current instruction+8, the thing is we still don't know what is the location of the current instruction, so we have to look at the assembly version
```asm
Dump of assembler code for function key1:
   0x00008cd4 <+0>:     push    {r11}           ; (str r11, [sp, #-4]!)
   0x00008cd8 <+4>:     add     r11, sp, #0
   0x00008cdc <+8>:     mov     r3, pc
   0x00008ce0 <+12>:    mov     r0, r3
   0x00008ce4 <+16>:    sub     sp, r11, #0
   0x00008ce8 <+20>:    pop     {r11}           ; (ldr r11, [sp], #4)
   0x00008cec <+24>:    bx      lr
End of assembler dump.  
```
The lines we are interested on are
```asm
   0x00008cdc <+8>:     mov     r3, pc
   0x00008ce0 <+12>:    mov     r0, r3
```
The first is exactly the same on the C version, we set r3 to where pc is. The second one, sets r0 to wherever r3 is, so it can return the value. With that, we found out the return value of the first function.<br>
The return value of key1() is 0x8ce4.
### key2()
On the C version we have
```c
int key2(){
        asm(
        "push   {r6}\n"
        "add    r6, pc, $1\n"
        "bx     r6\n"
        ".code   16\n"
        "mov    r3, pc\n"
        "add    r3, $0x4\n"
        "push   {r3}\n"
        "pop    {pc}\n"
        ".code  32\n"
        "pop    {r6}\n"
        );
}
```
In the first two lines, what the function do is first store the content of the r6 register in the stack, and then store in r6 the result of pc+1. To clarify the value, we look at the assembly
```asm
   0x00008cfc <+12>:    add     r6, pc, #1
```
So now the content of r6 is 0x8d05. Then, following up, the function uses the bx instruction, to set the thumb state, which means we will use now 16-bit instruction set. Then, the next 2 instructions move r3 to pc and then adds 4 to the r3 value. Since we are in thumb state now, the value of the pc register is the current instruction+4.
```asm
0x00008d04 <+20>:    mov     r3, pc
0x00008d06 <+22>:    adds    r3, #4
```
This means that the value on r3 is now 0x8d0c. Following up
```asm
0x00008d08 <+24>:    push    {r3}
0x00008d0a <+26>:    pop     {pc}
```
What they do is to first push the value of r3 to the stack, and then pop it into pc. This changes the state back to the 32-bit instruction set and jump to 0x8d0c. This goes back to the push instruction, with the difference that now, pc step is 4, instead of 2. So now, the instructions executed are
```asm
0x00008d0c <+28>:    pop     {r6} 
0x00008d10 <+32>:    mov     r0, r3
```
So both r3, r6 and r0 now have the same value.
The return value of key2() then is 0x8d0c.
### key3()
This function is pretty straight-forward, just like the first one
```c
int key3(){
        asm("mov r3, lr\n");
}
```
It moves r3 to **where** the function should return, the next instruction after the function call. Looking at the assembly, we see that this is the value which will be returned
```asm
0x00008d28 <+8>:     mov     r3, lr
0x00008d2c <+12>:    mov     r0, r3
```
So, to get the value we look at the function call, on the main function
```asm
0x00008d7c <+64>:    bl      0x8d20 <key3>
0x00008d80 <+68>:    mov     r3, r0
```
The return value of key3() is 0x8d80.
### key value
So, all we have to do is to give the value of 0x8ce4+0x8d0c+0x8d80, which is 0x1a770.<br>
Now we can connect to the level and give 0x1a770, or 108400, as input.<br>
Success!
