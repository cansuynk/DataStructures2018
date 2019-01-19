#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>

/* @Author
Student Name: Cansu YANIK
Student ID : 150170704
Date: 24.12.2018 */

using namespace std;

/*Binary Tree Nodes*/
struct Node{
	long int data;
	long int status;	/*This is used to choose the left or right of the node to travel.*/
	bool lastnode;		/*This is used to keep rightmost node, If node is rightmost node, this value will be true, otherwise false.*/
	Node *left;
	Node *right;
};

 /*I used a dynamic array to create binary tree, This array holds the nodes of the tree.*/
struct Array{
	Node *binary;
	long int size;
};

/*Stack's Node*/
struct Node2{
	Node *data;			/*Data of the stack. It holds Tree's nodes*/
	Node2 *next;
	Node2 *prev;
};

/*I implemented a stack to keep the parent nodes to be able to go back if it is necessary for part2*/

/*!!!! I took it from our lecture slides but I added some necessary parts !!!!!!!*/
struct mystack{	
	Node2 *head;
	long int size;
	void create();
	void close();
	void push(Node* );
	Node* pop();
	bool isempty();
};

struct BinaryTree{
	Node *root;
	Array *numberArray;		/*My dynamic array to construct binary tree by using given method*/
	mystack nodes_stack;	/*My original stack to keep the parent nodes*/
	mystack path_stack;		/*A temporary stack to print the best path because my original stack keeps them in reverse order*/
	long int nodecount;		/*Holds number of nodes my tree has*/
	long int target;		/*Holds target number*/
	FILE *file;
	void create();			/*Creates the binary tree*/
	void ReadFile(char *);	/*Reads the file*/
	void Find_Paths();		/*Finds the best paths*/	
	void close();
	void emptytree(Node *);
	
};

/*create empty stack*/
void mystack::create(){
	head = NULL;
	size=0;
}

/*close the stack*/
void mystack::close(){
	Node2 *p;
	while (head){
		p = head;
		head = head->next;
		delete p;
	}
	size=0;
}

/*push new element onto stack*/
void mystack::push(Node* newbinarynode){
	Node2 *newnode = new Node2;
	newnode->data = newbinarynode;
	newnode->next = head;	
	head = newnode;
	size++;
}

/*pop an element from stack*/
Node* mystack::pop(){
	Node2 *topnode;
	Node* temp;
	topnode = head;
	head = head->next;
	temp = topnode->data;
	delete topnode;
	size--;
	return temp;
}

/*check if stack is empty*/
bool mystack::isempty(){
	return head == NULL;
}

void BinaryTree::create(){
	root = NULL;	/*Initally root points NULL*/

	root=numberArray[1].binary;		/*Root is first index of array*/

	/*Here I applied the method which is given in the explanations. Thus I connected nodes*/
	
	for(int index=2; index<=nodecount; index++){
	
		if(index%2==0){
			numberArray[index/2].binary->left=numberArray[index].binary;
		}
		else if(index%2==1){
			numberArray[(index-1)/2].binary->right=numberArray[index].binary;
		}
	
	}

	/*This part is for saving last rightmost node of the binary tree. This is necessary to stop searhing paths when tree is end*/
	Node* temp = root;
	while(temp->right){
		temp=temp->right;
	}
	temp->lastnode=true;
}

void BinaryTree::ReadFile(char *filename){

	long int number1=0;		/*Number taken from the file*/
	long int index=1;		/*Holds the index of the nodes*/
	nodecount = 0;
	string line;			/*I read the file line by line, this holds the line string.*/

	Node *newnode;

	ifstream file;

	file.open( filename );		/*Opens file*/
	if(!file){					/*If the file cannot be opened*/
		
		cerr << "Cannot open grid file" << endl;
		exit(1);
		
	}
	
	numberArray= new Array[105];	/*Allocate a space from memory for my dynamic array*/
	numberArray[0].binary=NULL;		/*I started from index 1 so index 0 is unused*/

	getline(file,line);
	stringstream ss(line);
	string number;


	while(ss >> number){	/*Takes the numbers from line string*/

		number1=atoi(number.c_str());		/*Converts them to integer*/

		newnode= new Node;			/*Creates a node and fills the data*/
		newnode->data=number1;
		newnode->status=0;
		newnode->lastnode=false;
		newnode->left=NULL;
		newnode->right=NULL;

		numberArray[index].binary=newnode;	/*Put it into array in order*/
		index++;
		nodecount++;

	}

	file >> number1;	/*Takes target number*/
	target=number1;

	file.close();		/*Closes file*/

}
void BinaryTree::Find_Paths(){

	long int sum=target;	/*Holds the sum, in each travel, I will subtract the node values from the sum.*/
	int turn=1;				/*This is for determining the first or second path, 1 is for left subtrees of the root, 2 is for right subtrees of the root*/
	bool left_path_found=false;		/*These values are for stopping searching when path is found.*/
	bool right_path_found=false;
	Node* traverse=root;			/*This is for traversing the tree*/
	
	
	nodes_stack.create();			/*Cretaes the orinal and template stack*/
	path_stack.create();

	if(root->data>target){			/*If initially target is smaller than root*/
		cout<<"No Path Found" << endl;
		cout<<"No Path Found" << endl;
	}
	else{
		while(true){
			
			sum=sum-traverse->data;				/*Subtract the current number from the sum*/

			if(sum==0){							/*If sum is 0 (a path is found)*/
				nodes_stack.push(traverse);		/*Push the current node to stack because normally it keeps parents nodes*/
				
				if(turn==1){					/*If we are trying to find first path*/
					left_path_found=true;		/*First path was found*/
					
					cout << "Path Found: ";		/*Since my original stack keeps nodes in reverse order, I transport them into template stack*/
					while(!nodes_stack.isempty()){
						path_stack.push(nodes_stack.pop());
					}
					while(!path_stack.isempty()){	/*And print the numbers*/
						cout << path_stack.pop()->data << " ";
					}
					cout<<endl;

					nodes_stack.close();	/*Refresh the original stack to find second path*/
					nodes_stack.create();
					sum=target;			/*Sum is target now*/
					traverse=root;		/*Starts with the root*/
					turn=2;				/*Now turn is 2 (Right subtree of the root)*/
					root->status=1;		/*This means that: OK. I looked the left child of the node now it is time to look right child of the node.*/
				}
					
				else if(turn==2){	/*If we are trying to find second path*/
					
					if(left_path_found==false)		/*If we couldnot find the first path, we have to print "No Path Found" before printing second path*/
						cout << "No Path Found" << endl;

					right_path_found=true;		/*Second path was found*/

					cout << "Path Found: ";		/*Since my original stack keeps nodes in reverse order, I transport them into template stack*/
					while(!nodes_stack.isempty()){
						path_stack.push(nodes_stack.pop());
					}
					while(!path_stack.isempty()){
						cout << path_stack.pop()->data << " ";
					}
					cout<<endl;

					return;
				}
			}
			else if(traverse->left==NULL || sum<0){		/*If the sum is negative or we reach the end of the way we observe*/
				sum=sum+traverse->data;				/*Add again current number to the sum*/

				if(traverse->lastnode){		/*If this node is last right most node, we can finish searching*/
						break;
				}
				if(nodes_stack.head->data->status==0){		/*If current node is left child of it's parent (stack holds the parents)*/
					nodes_stack.head->data->status=1;		/*Now we can look it's right child*/
					traverse=nodes_stack.head->data->right;


					if(traverse==NULL){		/*If right child is NULL (no right child)*/

					/*	if(nodes_stack.size==1) //sildim diðeri kapsayabilir
							break;
					*/
						if(nodes_stack.head->data->lastnode)	/*If this node is last right most node, we can finish searching*/
							break;

						while(nodes_stack.head->data->status==1){	/*Pop the parent nodes from the stack until find one whose right child has not been looked yet*/
																	/*Dont forget adding them to sum*/
							sum=sum+nodes_stack.pop()->data;
					
						}
						traverse=nodes_stack.head->data->right;		/*Now we can look the right child of the node*/
						nodes_stack.head->data->status=1;			/*Left child was looked, it time for right child*/
					}
				}
				else{			/*If current node is right child of it's parent (stack holds the parents)*/
								/*We have to look other subtrees, we are climbing the tree*/
					if(nodes_stack.size==1)	/*If we reach the root, we can finish searching*/
							break;			

					while(nodes_stack.head->data->status==1){	/*Pop the parent nodes from the stack until find one whose right child has not been looked yet*/
					
						sum=sum+nodes_stack.pop()->data;

						if(nodes_stack.isempty()){		/*If we reach the root, we can go out from this loop*/
							break;
						}
					}

					if(nodes_stack.isempty())	/*If we reach the root, we can finish searching (go out from second loop)*/
							break;

					nodes_stack.head->data->status=1;		/*Left child was looked, it time for right child*/
					traverse=nodes_stack.head->data->right;	/*Now we can look the right child of the node*/
				
				}

				if(nodes_stack.size==1){	/*If we reach the root node, it's time for searching second path*/
					turn=2;	
				}
			
			}
			else{			/*If there is still a node to check*/
				nodes_stack.push(traverse);		/*Push the previous node as an incoming's parent to the stack*/
				if(traverse->status==1){		/*If we have already checked the left child of the node, it's time for checking right child*/
					traverse=traverse->right;	/*Note:: This if statement only for root node*/

					if(traverse==NULL)	/*If this node(root) has not a right child node, we can finish the searching*/
						break;
				}
					
				else	/*If status=0 (This means that: start(or continue) to check with the left child)*/
					traverse=traverse->left;	/*Go its left child*/
			}
		}

		if(!left_path_found && !right_path_found){	/*If both two path couldnot find*/
			cout<<"No Path Found" << endl;
			cout<<"No Path Found" << endl;
		}
		else if(!right_path_found){		/*If only second path couldnot find*/
			cout<<"No Path Found" << endl;
		}
	}
}
/*Gives back all dynamic pointers to the memory*/
void BinaryTree::close(){

	delete[] numberArray;
	emptytree(root);

}

/*I took this part from our lecture slides*/
void BinaryTree::emptytree(Node* p){

	if (p) {
		if (p->left != NULL){
			emptytree(p->left);
			p->left = NULL;
		}
		if (p->right != NULL){
			emptytree(p->right);
			p->right = NULL;
		}
		delete p;
	}
}

int main (int argc , char ** argv){
	
	BinaryTree Tree;			/*My Tree*/
	Tree.ReadFile(argv [1]);	/*Calls the function to read numbers from the file*/
	Tree.create();				/*Calls the function to create tree*/
	Tree.Find_Paths();			/*Calls the function to find the paths*/

	Tree.nodes_stack.close();	/*Calls the function to close the stacks*/
	Tree.path_stack.close();
	Tree.close();				/*Calls the functions to close and to clear the tree and dynamic array*/

	return 0;
}
