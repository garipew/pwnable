## lotto
This is a lottery level.

## Solution
We have to somehow get our magic ticket, and this time its really random right? It gets the number from /dev/urandom...
The thing is, the validation is not very well done
```c
for(i=0; i<6; i++){
	for(j=0; j<6; j++){
		if(lotto[i] == submit[j]){
			match++;
		}
	}
}
```
Instead of properly checking for correspondence, assuming the order of the elements don't really matter, it checks for each character on the lottery ticket, if there are correspondences on the input, incrementing match if so, but if forgets to check for duplicates. This makes it really easy to guess, instead of matching all characters we just need to match a single one, with all our input. For instance, we have the input "!!!!!!", with that, we just need to keep trying until eventually we get the flag.
Success.
