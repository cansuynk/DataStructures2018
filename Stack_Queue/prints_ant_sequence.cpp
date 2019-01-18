#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;


/* @Author
Student Name: Cansu YANIK
Student ID : 150170704
Date: 12.12.2018 */


/*Here are the Stack and Queue implementations. I took them from our lecture's 
codes and directly used them in my code. I just changed their names with the desired names*/


struct Node{
	long int data;
	Node *next;
};

/*Queue struct definition*/
struct queueAnt{
	Node *front;
	Node *back;
	void create();
	void close();
	void enqueue(long int);
	long int dequeue();
	bool isempty();
};

/*Stack struct definition*/
struct stackAnt{	
	Node *head;
	void create();
	void close();
	void push(long int);
	long int pop();
	bool isempty();
};

/*Here is the Ants struct which is given to us*/
struct Ants {
	queueAnt ants ;
	queueAnt holeDepths ;
	stackAnt hole ;
	void ReadFile ( char *);
	void ShowContents ( bool );
	void CrossRoad ();
};

/* * * * * * * Implementions of the queueAnt struct's functions * * * * * * */

/*Create empty queue*/
void queueAnt::create(){
	front = NULL;
	back = NULL;
}

/*Close the queue*/
void queueAnt::close(){
	Node *p;
	while(front){
		p = front;
		front = front->next;
		delete p;
	}
}

/*Add new element onto queue*/
void queueAnt::enqueue(long int newdata){
	Node *newnode = new Node;
	newnode->data = newdata;
	newnode->next = NULL;
	if(isempty()){
		back = newnode;
		front = back;
	}
	else{
		back->next = newnode;
		back = newnode;
	}
}

/*Take an element from queue*/
long int queueAnt::dequeue(){
	Node *topnode;
	long int temp;
	topnode = front;
	front = front->next;
	temp = topnode->data;
	delete topnode;
	return temp;
}

/*Check if queue is empty*/
bool queueAnt::isempty(){
	return front == NULL;
}

/* * * * * * * Implementions of the stackAnt struct's functions * * * * * * */

/*create empty stack*/
void stackAnt::create(){
	head = NULL;
}

/*close the stack*/
void stackAnt::close(){
	Node *p;
	while (head){
		p = head;
		head = head->next;
		delete p;
	}
}

/*push new element onto stack*/
void stackAnt::push(long int newdata){
	Node *newnode = new Node;
	newnode->data = newdata;
	newnode->next = head;	
	head = newnode;
}

/*pop an element from stack*/
long int stackAnt::pop(){
	Node *topnode;
	long int temp;
	topnode = head;
	head = head->next;
	temp = topnode->data;
	delete topnode;
	return temp;
}

/*check if stack is empty*/
bool stackAnt::isempty(){
	return head == NULL;
}

/* * * * * * * Implementions of the Ants struct's functions * * * * * * */

void Ants::ReadFile(char *filename){

	long int ant_number=0;  	/*Holds number of ants*/
	long int holes_depths=0;	/*Holds depths of the holes*/

	ifstream file;

	file.open( filename );		/*Opens file*/
	if(!file){					/*If the file cannot be opened*/
		
		cerr << "Cannot open grid file" << endl;
		exit(1);
		
	}

	ants.create();			/*Creates the ants queue*/
	holeDepths.create();	/*Creates the holeDepths queue*/

	file >> ant_number;		/*Reads the number of ants from the file*/
	
	for (int i=1; i<=ant_number; i++){		/*Adds the ants into the queue*/
		ants.enqueue(i);
	}

	while( file >> holes_depths){  			/*Takes the numbers of holes depths*/
		
		holeDepths.enqueue(holes_depths);	/*and adds them into the queue until the end of the file*/
		
		if( file.eof()  ){					/*If the file is end, return*/
			return;
		}
	}

	file.close();	/*After reading the file, I can close the file*/
}

void Ants::ShowContents(bool Value){
	Node *traverse;				/*A node pointer, this is for traversing the queue*/
	
	if(Value){					/*If the Value is 1(True), this means that I want to print the content of the ants queue*/
		traverse = ants.front;	/*Therefore, traverse is pointed to the begining of the ants queue*/
	}
	else{						/*Else if the Value is 0(False), this means that I want to print the content of the holeDepths queue*/
		traverse = holeDepths.front;	/*Therefore, traverse is pointed to the begining of the holeDepths queue*/
	}

	while(traverse!=NULL){		/*Then, I can print the content of desired queue*/
		cout << traverse->data << " ";  /*Prints the data of queue*/
		traverse=traverse->next;		/*Traversing*/
	}
	cout << endl;

}

void Ants::CrossRoad(){
	long int current_hole_depth=0;	/*Holds the beginning of the holeDepths queue (the depth of the current hole)*/
	long int falling_ant=0;			/*Holds the ants falling into the hole*/

	hole.create();					/*Creates the hole stack*/

	while(!holeDepths.isempty()){	/*Until crossing the whole holes*/
	
		current_hole_depth=holeDepths.dequeue();	/*Takes the depths of the holes from it's queue*/

		for( int i=0; i<current_hole_depth; i++){	/*M leading ants go into the hole, M=current_hole_depth*/
			
			falling_ant=ants.dequeue();
			hole.push(falling_ant); 	 /*These ants will fill the hole so push them into the hole stack*/
		}

		while(!hole.isempty()){		/*Until no ants left in the hole.*/
			
			falling_ant=hole.pop();		
			ants.enqueue(falling_ant);	/*Pulled ants will be added to the end of the ants queue*/
		}
	
	}
	hole.close();	/*I can close the hole stack and holeDepths queue since I'm done with them*/
	holeDepths.close();

}
/*I just added the close operation for the ants queue in the given main function*/

int main (int argc , char ** argv ){
	Ants a;
	a. ReadFile ( argv [1]); // store the number of ants and depths of holes
	cout << "The initial Ant sequence is: ";
	a. ShowContents (1); // list ant sequence ( initially : 1, 2, ... , N)
	cout << "The depth of holes are: ";
	a. ShowContents (0); // list depth of holes
	a. CrossRoad ();
	cout << "The final Ant sequence is: ";
	a. ShowContents (1);
	
	a.ants.close();
	return 0;
}
