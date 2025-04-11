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

