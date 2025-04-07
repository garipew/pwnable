## random
This level is all about computer's random numbers.
## Solution
The program reads from stdin a value and then xors it with a random value, if the result is 0xcafebabe we get the flag.<br>
So, what does this mean? Do we have to keep trying until we randomly get the right value? **No!**.<br>
The thing is **computer's random numbers aren't really random**, they are what we call pseudo-random and this means that, in practice, they can be replicated. How do we replicate them then?<br>
To replicate a pseudo-random number, we have to use a special value, a seed, on our generator.
```c
include <stdio.h>

int main(){
        unsigned int random;
        random = rand();        // random value!

        unsigned int key=0;
        scanf("%d", &key);

        if( (key ^ random) == 0xcafebabe ){
                printf("Good!\n");
                setregid(getegid(), getegid());
                system("/bin/cat flag");
                return 0;
        }

        printf("Wrong, maybe you should try 2^32 cases.\n");
        return 0;
}
```
Since there is no being assigned as the seed here, what this means in practice is that this program is using the default value as a seed, which means that it can very easily be replicated. 
```c
random = rand();        // random value!
```
This line will always have the same value, in any program you write! Which means that we can very easily get the key, writing this snippet of code:
```c
#include <stdio.h>
#include <stdlib.h>

int main(){
        printf("%d\n", rand() ^ 0xcafebabe);
        return 0;
}     
```
We run it, get our key, now we run the random program on our home, pass our key as input and...<br>
Success!
