#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <conio.h>

int guessMode(){//the mode for player guess the number
    printf("\n");
    int ans[4];//the array to access the answer
    srand((unsigned)(time(NULL)));//set the random varible seed

    for(int i=0;i<4;i++){
        start:                  //set the label for 'set the number again'
        ans[i]=rand()%10;
        for(int j=0;j<i;j++){   //when the number is setting, checking if the number is repeated.
            if(ans[i]==ans[j])
                goto start;     //set the number again
        }
    }

    int A,B,guess,judge[4],count=0;     
    //A means the number existing with right position, B means the number existing with wrong position
    //guess is the number from player's guess, judge use to access the every digit from 'guess'
    
    while(A!=4){                //if player doesn't get 4A, keep playing
        A=0,B=0;                //the initialize for A & B
        printf("\nPlease guess the number (4 digits, numbers do not repeat): ");  //the clue for player guessing
        scanf("%d",&guess);
        count++;
        for(int i=3;i>=0;i--){  //decompose the digits of the guess
            judge[i]=guess%10;
            guess/=10;
        }
        //printf("The number is：%d%d%d%d\n",judge[0],judge[1],judge[2],judge[3]); //testing

        for(int i=0;i<4;i++){       //compare the number with answer
            if(judge[i]==ans[i])    //if the position is correct, get 1A
                A++;
            else{
                if(judge[i]==ans[0] || judge[i]==ans[1] || judge[i]==ans[2] || judge[i]==ans[3]) //if the position is wrong, but the number is existing, get 1B
                    B++;
            }
        }

        if(A==4)    //get 4A, player win, break the loop
            break;
        else        //if player doesn't get 4A, return the condition
            printf("you got %dA%dB\n",A,B);
    }

    printf("Congratulations! The answer is %d%d%d%d. You use %d times.\n",ans[0],ans[1],ans[2],ans[3],count);   //Congratulations
    return 0;
}

bool is_unique(int num) { //check the digits of the number is not repeat
    char str[5];
    snprintf(str, 5, "%d", num);
    int len=strlen(str);
    for (int i=0;i<len;i++) {
        for (int j=i+1;j<len;j++) {
            if (str[i]==str[j]) {
                return false;
            }
        }
    }
    return true;
}

void calculate_score(int guess, int last_guess, int *A, int *B) { //compare the last two guesses
    char guess_str[5], lastGuess_str[5];
    snprintf(guess_str, 5, "%d", guess); //access the 'guess'
    snprintf(lastGuess_str, 5, "%d", last_guess); //access the 'guess'
    *A=0, *B=0;
    for (int i=0;i<4;i++) { //compare the two numbers
        if (guess_str[i]==lastGuess_str[i]) {
            (*A)++;
        } else if (strchr(lastGuess_str, guess_str[i])) {
            (*B)++;
        }
    }
}

void generate_all_guesses(int guesses[], int *count) {  //generate all guesses to compare
    *count=0;
    for (int i=1023;i<=9876;i++) {
        if (is_unique(i)) { //check if the digits of the number in unique
            guesses[(*count)++]=i;
        }
    }
}

void filter_guesses(int guesses[], int *count, int last_guess, int A, int B) { //remove the numbers which is not conform to the condition
    int new_count = 0;
    for (int i=0;i<*count;i++) {
        int a,b;
        calculate_score(guesses[i], last_guess, &a, &b); //comparison with all numbers, use a & b get the condition of comparisons
        if (a == A && b == B) {
            guesses[new_count++] = guesses[i];
        }
    }
    *count=new_count;
}

int proposeMode(){//the mode for player propose the number
    printf("\n");
    int first=1234;
    int guess;
    int A, B;
    int attempts=1;
    int guesses[5040]; //all possible numbers of the answer
    int count;

    srand(time(NULL));

    generate_all_guesses(guesses, &count); //generate all possible answer
    printf("Please think a number in your mind.\n");
    printf("I guess %d, I got (Please enter the format: nAnB): ", first);
    scanf("%dA%dB",&A,&B);

    while (A != 4){
        guess = guesses[rand() % count]; //get the number from guesses
        attempts++;
        printf("I guess %d, I got (Please enter the format: nAnB): ", guess); //print the guess from computer
        scanf("%dA%dB",&A,&B); //player input
        if (A != 4) {
            filter_guesses(guesses, &count, guess, A, B); //filter the impossible answer
        }
    }

    if(attempts==1)//The computer is win
        printf("Yes! I got the number!\nThe answer is %d, I use %d times.\n", first, attempts); 
    else
        printf("Yes! I got the number!\nThe answer is %d, I use %d times.\n", guess, attempts);
    return 0;
}

bool check(){ //the function to check if player wants to play another game.
    check:
    printf("Do you want to play another game?(Yes:1/No:0)\n");
    int check; //use to access the choice from player
    if(scanf("%d",&check)==1){
        if(check==1)        //check=1, play another game
            return true;
        else if(check==0)  //check=0, end the game
            return false;
        else{
            printf("Wrong input, please choose again.\n");
            goto check;
        }
    }
    else{
        printf("Invalid input, please re-enter the game.\n");
        return false;
    }  
}

void hide_input(char *answer) {
    char ch;
    int index = 0;
    printf("\nProposer, enter your number(4 digits, no repeat):");
    while ((ch = _getch()) != '\r') { // '\r' 是回車鍵
        printf("*"); // 螢幕上顯示*
        answer[index++] = ch;
    }
    answer[index] = '\0'; // 添加字符串結尾的空字符
    printf("\n");
}

void combatMode(){
    start:
    printf("\nWelcome to combat mode, this is the mode for two player!!\nPlease pick a proposer.(if u r ready, please press 'y'): ");
    char ready;
    scanf(" %c",&ready);
    if(ready=='y'){
        int game=1,score[2];
        for(int i=0;i<=1;i++){
            char ans[4];
            hide_input(ans);
            //printf("the answer is %s\n",ans); //testing
            
            int A=0,B=0,count=0;
            char guess[4];
            while(A!=4){
                A=0,B=0;                //the initialize for A & B
                printf("\nPlayer, Please guess the number (4 digits, numbers do not repeat): ");  //the clue for player guessing
                scanf(" %c%c%c%c",&guess[0],&guess[1],&guess[2],&guess[3]);
                count++;

                for(int i=0;i<4;i++){       //compare the number with answer
                    if(guess[i]==ans[i])    //if the position is correct, get 1A
                        A++;
                    else{
                        if(guess[i]==ans[0] || guess[i]==ans[1] || guess[i]==ans[2] || guess[i]==ans[3]) //if the position is wrong, but the number is existing, get 1B
                            B++;
                    }
                }
                if(A==4)    //get 4A, player win, break the loop
                    break;
                else        //if player doesn't get 4A, return the condition
                    printf("You got %dA%dB\n",A,B);
            }
            score[i]=count;//access the score of this round
            printf("The answer is %c%c%c%c. You use %d times.\n",ans[0],ans[1],ans[2],ans[3],count);   //Congratulations
            if(game==2)
                break;
            else{
                game++;
                printf("Switch side.\n");//the clue of switching side
            }
        }

        if(score[0]>score[1])//decide the winner
            printf("\nThe player of last round is winning.\n");
        else if(score[0]<score[1])
            printf("\nThe player of this round is winning.\n");
        else if(score[0]==score[1])
            printf("\nTie.\n");
    }
    else{
        printf("Wrong input, please choose again.\n");
        goto start;
    }
}

int main(){
    char name[50];
    printf("What's your name?\nYour name: ");
    scanf("%s",&name);

    start:
    printf("\nHello %s~ Welcome to 1A2B!\nPlease choose the mode: (1)Guess the number, (2)Propose the number, (3)Combat mode\nYour choice is(enter 1 or 2 or 3): ",name);//the clue guiding player to choose the mode
    int mode;//use to access the mode choose from player
    while(scanf("%d",&mode)!=EOF){
        if(mode==1){
            guessMode();//mode=1 means player wants to guess the number 
            if(check())//check if player wants to play another game
                goto start; 
            else{
                printf("Thanks you %s~~ Have a nice day.\n",name);
                break;
            }
        }
        else if(mode==2){
            proposeMode();//mode=2 means player wants to propose the number 
            if(check())//check if player wants to play another game
                goto start; 
            else{
                printf("Thanks you %s~~ Have a nice day.\n",name);
                break;
            }
        }
        else if(mode==3){
            combatMode();//mode=2 means player wants to propose the number 
            if(check())//check if player wants to play another game
                goto start; 
            else{
                printf("Thanks you~~ Have a nice day.\n");
                break;
            }
        }
        else{
            printf("Invalid input, please re-enter the game.\n");
            break;//Wrong input, break the game(because the return value of scanf() will crash the project)
        }
    }
}