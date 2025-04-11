## blackjack
This level is a blackjack implementation?

## The problem
The level description says we will get the flag if we become millionares, so we just have to win? Okay, lets take a look on the code then
```c
    srand((unsigned) time(NULL)); //Generates random seed for rand() function
``` 
We don't have to search much before comming across this line, which would be perfectly fine, if after looking a little bit more we didn't find another 5 more occurences of it, in functions that are called multiple times... 
```c
static unsigned long next = 1;

/* RAND_MAX assumed to be 32767 */
int rand(void) {
	next = next * 1103515245 + 12345;
	return((unsigned)(next/65536) % 32768);
}

void srand(unsigned int seed) {
	next = seed;
}
```
This is an example implementation of both the srand() and the rand() function, which can be found in the man page of srand. We don't have to look much into it before understanding why srand() should be called only once. The first rand() return value after srand() is the most predictable. In that case, we should look for some interesting rand() calls.

## Strategy
Before searching in the dar for some shiny rand() calls, let's define a strategy. What we want is to win, always. How do we win in blackjack? We have to be the one with the closest hand to 21, so, maybe we should get 21 every round right? One ace and one ten would do the job. This means we should look for rand() calls on our card generation.  
```c
int randcard() //Generates random card
{
      
                
     srand((unsigned) time(NULL)); //Generates random seed for rand() function
     random_card = rand()%4+1;
      
     if(random_card==1)
     {   
         clubcard();
         l=k;
     }
      
     if(random_card==2)
     {
         diamondcard();
         l=k;
     }
      
     if(random_card==3)
     {
         heartcard();
         l=k;
     }
          
     if(random_card==4)
     {
         spadecard();
         l=k;
     }    
     return l;
} // End Function   
```
How convenient, we have an entire function dedicated to it... But this one doesn't mean much to us, we don't really care about the suit of our card... All the suit functions though... They all work the same way, the only difference between them is the drawing of the card.  
```c
	srand((unsigned) time(NULL)); //Generates random seed for rand() function
	k=rand()%13+1;
```
This is where we want our magic to happen.

## Solution
With everything set, we can construct our solution. Recaptulating, what we want to do is to mess up with the card generation, so we always get an ace and then a ten. That means we want the first call of rand() to be any number that the remainder of the division by 13 is 10, and the second to be any number that the remainder of the division by 13 is 9, 11 or 12.
Wait, the seed is being set with the return value of time(NULL), how can we possibly use that in our advantage, isn't it like, random?
```
DESCRIPTION
       time() returns the time as the number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
```
This is the man page description of time(), in short: No, lol.
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(){
	while(1){
		srand(time(NULL));
		if(rand()%13==0){
			printf("take ace\n");
			do{
				srand(time(NULL));
			} while(rand()%13 != 9);
			printf("take 10\n");
		}
	}
	return 0;
}
```
Something like this will give us the timing, but where are the cards generated?
```c
void play() //Plays game
{
      
     int p=0; // holds value of player_total
     int i=1; // counter for asking user to hold or stay (aka game turns)
     char choice3;
      
     cash = cash;
     cash_test();
     printf("\nCash: $%d\n",cash); //Prints amount of cash user has
     randcard(); //Generates random card
```
Here, after printing the amout of cash we have, 
```c
         if(p<=21) //If player total is less than 21, ask to hit or stay
         {         
             printf("\n\nWould You Like to Hit or Stay?");
              
             scanf("%c", &choice3);
             while((choice3!='H') && (choice3!='h') && (choice3!='S') && (choice3!='s')) // If invalid choice entered
             {                                                                           
                 printf("\n");
                 printf("Please Enter H to Hit or S to Stay.\n");
                 scanf("%c",&choice3);
             }
 
 
             if((choice3=='H') || (choice3=='h')) // If Hit, continues
             { 
                 randcard();
```
And here, which is inside the play() function. This is not very exciting, since there is no blocking before the first pick, we can't choose it... So we have to adapt.
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


void pick_card(int rank){
	do{
		srand(time(NULL));
	} while((rand()%13+1)!=rank);
	printf("hit\n");
}


int main(){
	int first_card;
	int hand;
	int next_pick;
	while(1){
		printf("What is your current hand value? ");
		scanf("%d", &first_card);
		hand = first_card;
		while(hand < 21){
			next_pick = (21 - hand);
			next_pick = next_pick > 11 ? 11 : next_pick; // pick the ace if possible
			pick_card(next_pick);
			next_pick = next_pick > 10 && next_pick != 11 ? 10 : next_pick; // adjust card value 
			hand += next_pick;
		}
		printf("stay\n");
	}
	return 0;
}

```
This would be a better version. And it would work, if it weren't for these lines
```c
if((choice3=='H') || (choice3=='h')) // If Hit, continues
{ 
	randcard();
        player_total = p + l;
        p = player_total;
        printf("\nYour Total is %d\n", p);
        dealer();
        if(dealer_total==21) //Is dealer total is 21, loss
           {
```
This is a better overview of what happens as soon as we hit, the first thing checked after we pick a card is if the dealer won, not us, so instead of trying to win, we should try to make him lose.
The dealer card is picked on the dealer() function
```c
void dealer() //Function to play for dealer AI
{
     int z;
      
     if(dealer_total<17)
     {
      srand((unsigned) time(NULL) + 1); //Generates random seed for rand() function
      z=rand()%13+1;
```
It only picks a new card if the current hand is less than 17 and the seed is set by time(NULL)+1. Okay, keep that in mind.
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


void pick_card(int rank){
	do{
		srand(time(NULL)+1);
	} while((rand()%13+1)!=rank);
	printf("H\n");
}


int main(){
	int first_card;
	int hand;
	int next_pick;
	while(1){
		printf("What is the dealer current hand? ");
		scanf("%d", &first_card);
		hand = first_card;
		while(hand < 17){
			next_pick = (16 - hand);
			next_pick = next_pick > 10 ? 10 : next_pick; // never picks aces
			if(hand+10>21){
				next_pick = 10; // go over the line if possible
			} 
			pick_card(next_pick);
			next_pick = next_pick > 10 && next_pick != 11 ? 10 : next_pick; // adjust card value 
			hand += next_pick;
		}
		printf("S\n");
	}
	return 0;
}
```
This will be enough to give us the timing, know we just have to play it.
Success.
