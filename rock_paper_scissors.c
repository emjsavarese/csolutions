#include <stdio.h>
#include <string.h>

/* Emily Savarese
   Dr. Misurda
   CSC 352
   This project will play rock, paper, scissors with the user. 
   It will first prompt the user if they want to play, then prompts
   the user for their choice from rock, paper, or scissors. 
   (Scissors beats paper, paper covers rock, rock beats scissors).
   The computer then chooses an option, and compares the two. It keeps
   track of the winner and then prompts the user if they want to play again.
   The cycle continues until the user says 'no' when prompted.
*/


/* main initializes the beginning user and computer scores at 0.
It prompts the user if they want to play. If they type 'yes',
it will run the actual comparison method. if that method returns
1, it updates the user scores. if it returns 2, it updates the 
computer's score. It then prompts the user to play again. The loop
stops and the program is finished if the user says anything
other than 'yes'.
RETURNS: returns 0 to say the program runs successfully.
*/

int main(){
	srand((unsigned int)time(NULL));
	char choice[20];
	int comp_score = 0;
	int user_score = 0;
	int result;
	printf("Welcome to Rock, Paper, Scissors\n\n");
	printf("Would you like to play? ");
	scanf("%s",choice);
	while (strcmp(choice,"yes")== 0){
		result = compare_strings();
		if (result ==1){
			user_score++;
		}
		if(result ==2){
			comp_score++;
		}
		printf("The score is now you: %d computer: %d\n\n", user_score, comp_score);
		printf("Would you like to play? ");
		scanf("%s", choice);
	}
	return 0;
}

/* generates a random value using rand and determines which
choice the computer makes based on the number it throws. It also 
prompts the user for their input, which they hopefully put to be
either rock, paper, or scissors. It then determines if they've tied
(both user and comp are the same) or if the user wins. Otherwise
it presumes the user has lost.
RETURNS: an int determining who gets the points. 0 means no one recieves
points (a tie), 1 means the user has won, 2 means the computer has won.

*/
int compare_strings(){
	//1 is rock, 2 is paper, 3 is scissors
	char comp_choice[20];
	char user_choice[20];
	int value = rand() % (3 - 1 +1) +1;
	if (value ==1){
		strcpy(comp_choice, "rock");
	}
	if (value == 2){
		strcpy(comp_choice, "paper");
	}
	if (value == 3){
		strcpy(comp_choice, "scissors");
	}
	printf("\nWhat is your choice? ");
	scanf("%s",user_choice);
	
	printf("The computer chooses %s. ",comp_choice);	

	if (strcmp(user_choice, comp_choice) ==0){
		printf("You tie this game!\n\n");
		return 0;
	}
	if((strcmp(user_choice,"rock")== 0 && strcmp(comp_choice,"scissors")==0)
		||(strcmp(user_choice,"scissors")==0&& strcmp(comp_choice,"paper")==0)
		||(strcmp(user_choice,"paper")==0 && strcmp(comp_choice,"rock")==0)){
		printf("You win this game!\n\n");
		return 1;
	}
	else{
		printf("You lose this game!\n\n");
		return 2;
	}

}


