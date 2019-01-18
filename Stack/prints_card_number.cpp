#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

/* @Author
* Student Name: Cansu YANIK
* Student ID : 150170704
* Date: 03.12.2018 */

using namespace std;

/*Here is the Stack implementation. I took it from our lecture's codes and directly used it in my code.*/
/*I just added size variable to be able to store size of the stacks*/

struct Node{
	long int data;
	Node *next;
};

// Stack struct definition
struct Stack{	
	Node *head;
	int size;			/*Stores size of the stacks*/
	void create();
	void close();
	void push(long int);
	long int pop();
	bool isempty();
};

// create empty stack
void Stack::create()
{
	head = NULL;
	size=0;
}

// close the stack
void Stack::close()
{
	Node *p;
	while (head){
		p = head;
		head = head->next;
		delete p;
	}
	size=0;
}

// push new element onto stack
void Stack::push(long int newdata)
{
	Node *newnode = new Node;
	newnode->data = newdata;
	newnode->next = head;	
	head = newnode;
	size++;
}

// pop an element from stack
long int Stack::pop()
{
	Node *topnode;
	long int temp;
	topnode = head;
	head = head->next;
	temp = topnode->data;
	delete topnode;
	size--;
	return temp;
}

// check if stack is empty
bool Stack::isempty()
{
	return head == NULL;
}

/*Creates a DataStructure object*/

typedef Stack Datastructure;

Datastructure deck_on_table; 	/*Creates a DataStructure object for a stack of deck on the table*/
Datastructure deck_player1;		/*Creates a DataStructure object for a stack of first player's deck*/
Datastructure deck_player2;		/*Creates a DataStructure object for a stack of second player's deck*/
Datastructure bin;				/*Creates a DataStructure object for a stack of bin's cards*/

/*There are two function prototypes*/

void Read_File(char *filename); /*Prototype for reading file*/
void Game();					/*Prototype for the game*/

int main (int argc, char * argv []){
	
	
	deck_on_table.create();   /*Creates the stacks*/
	deck_player1.create();
	deck_player2.create();
	bin.create();

	(void)argc;		/*Since I never use argument size, I wrote this statement to prevent any error.*/
	
	Read_File(argv[1]); /*Reads file and creates stacks*/
	
	if(deck_player1.size==deck_player2.size)	/*Each player has equal number of cards in their decks.*/
		Game();				/*Ready to play this game so calls game function*/
	

	cout << bin.size << endl;	/*Prints the bin size to the screen without any cosmetics*/

	deck_on_table.close();	/*Closes the all stacks*/
	deck_player1.close();
	deck_player2.close();
	bin.close();

	return EXIT_SUCCESS;
}

/*This function is for reading file and creating stacks*/

void Read_File(char *filename){

	long int tableDeckCount;	/*Holds tableDeckCount*/
	long int playerDeckCount;	/*Holds playerDeckCount*/
	long int card;				/*Holds current card*/
	ifstream file;

	file.open( filename );		/*Opens file*/
	if(!file){		/*If the file cannot be opened*/
		
		cerr << "Cannot open grid file" << endl;
		exit(1);
		
	}
	
	file >> tableDeckCount;
	file >> playerDeckCount;
	

	if(tableDeckCount<0 || tableDeckCount>1000){	/*If tableDeckCount is less than 0 or greater than 1000*/
		cout<< "Invalid table deck count" << endl;	/*This means that the file is invalid and the game will end.*/
		return;
	}
	if(playerDeckCount<0 || playerDeckCount>1000){	/*If playerDeckCount is less than 0 or greater than 1000*/
		cout<< "Invalid player deck count" << endl;	/*This means that the file is invalid and the game will end.*/
		return;
	}
		
	for(int i=0; i<tableDeckCount; i++){	/*Getting numbers as much as tableDeckCount from the files*/

		if( file.eof() ){							/*If file ends without getting enough number*/
			cout<< "Invalid input file" << endl;	/*This means that the file is invalid and the game will end.*/
			return;
		}

		file >> card;	/*Takes card number*/

		if(card<-1000000 || card>1000000 || card==0){	/*If card number is equal to 0 or less than -1000000 or greater than 1000000*/
			cout<< "Invalid card value" << endl;		/*This means that the card value is invalid and continues with the next number*/
		}
		else
			deck_on_table.push(card);	/*If card value is valid, pushs it to the table stack*/
		
	}

	for(int i=0; i<playerDeckCount; i++){	/*Getting numbers as much as playerDeckCount from the files*/

		if( file.eof()  ){							/*If file ends without getting enough number*/
			cout<< "Invalid input file" << endl;	/*This means that the file is invalid and the game will end.*/
			return;
		}

		file >> card;	/*Takes card number*/

		if(card<-1000000 || card>1000000 || card==0){ 	/*If card number is equal to 0 or less than -1000000 or greater than 1000000*/
			cout<< "Invalid card value" << endl;		/*This means that the card value is invalid and continues with the next number*/
		}
		else
			deck_player1.push(card);		/*If card value is valid, pushs it to the player1 stack*/
		
	}

	for(int i=0; i<playerDeckCount; i++){  /*Same procedure is applied for the player2*/

		if( file.eof()  ){
			cout<< "Invalid input file" << endl;
			return;
		}

		file >> card;

		if(card<-1000000 || card>1000000 || card==0){
			cout<< "Invalid card value" << endl;
		}
		else
			deck_player2.push(card);
		
	}
	file.close();  /*Then closes the file*/
}

/*This function is for playing the game and filling the bin if there are cards thrown away*/

void Game(){

	long int card;				/*Holds current card*/
	int player_turn=1; 		/*For decide which player's turn, first player begins the game*/
	
	/*player_turn is incremented at every turn. If this number can be divided by 2 --> Player2's turn, if not --> Player1's turn*/
	
	long int abs_table_card;		/*Holds absolute value of the table card*/

	while ( !deck_on_table.isempty() && !deck_player1.isempty() && !deck_player2.isempty()){ /*Plays the game until the number of cards (Table or players) finishes*/
		
		card=deck_on_table.pop(); 	/*Takes a card from table's deck*/

		if(card<0){  				/*If the card taken from table deck is negative*/
			abs_table_card=card*(-1);	/*I need the absolute value of the card number*/

			for(int i=0; i<abs_table_card; i++){ 		/*The player gives that amount of card in his deck to the competitor*/
			
				if(player_turn % 2 == 1){    /*This means that this is first player's turn*/
					if(deck_player1.head->data > deck_player2.head->data){ 			/*If the player's card is greater than competitor's card*/
						deck_player2.push(deck_player1.pop());						/*Player gives his card to the competitor*/
					}
					else{
						bin.push(deck_player1.pop());								/*If the player's card is less than competitor's card*/
					}																/*Card will be thrown to the bin*/
				}
				else if (player_turn % 2 != 1){    /*This means that this is second player's turn*/
					if(deck_player2.head->data > deck_player1.head->data){			/*If the player's card is greater than competitor's card*/
						deck_player1.push(deck_player2.pop());						/*Player gives his card to the competitor*/
					}
					else{															/*If the player's card is less than competitor's card*/
						bin.push(deck_player2.pop());								/*Card will be thrown to the bin*/
					}
				}

				if(deck_player1.isempty() || deck_player2.isempty()){				/*If player's deck is empty, game will end*/
					break;				
				}

			}
		}
		else{      /*If the card taken from table deck is positive*/
		
			for(int i=0; i<card; i++){	/*The player takes that amount of card in his deck to the competitor*/
				
				if(player_turn % 2 == 1){    /*This means that this is first player's turn*/
				
					if(deck_player2.head->data > deck_player1.head->data){			/*If the competitor's card is greater than player's card*/
						deck_player1.push(deck_player2.pop());						/*Player takes competitor's card*/
					}
					else{
						bin.push(deck_player2.pop());								/*If the competitor's card is less than player's card*/
					}																/*Card will be thrown to the bin*/
				}
				else if (player_turn % 2 == 0){    /*This means that this is second player's turn*/
				
					if(deck_player1.head->data > deck_player2.head->data){			/*If the competitor's card is greater than player's card*/
						deck_player2.push(deck_player1.pop());						/*Player takes competitor's card*/
					}
					else{
						bin.push(deck_player1.pop());								/*If the competitor's card is less than player's card*/
					}																/*Card will be thrown to the bin*/
				}

				if(deck_player1.isempty() || deck_player2.isempty()){				/*If player's deck is empty, game will end*/
					break;				
				}
			}
		}

		player_turn++;  /*Increments player_turn to decide which player will play*/

	}
}
