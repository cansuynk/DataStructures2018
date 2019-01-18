#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "grid.h"

using namespace std;

/*For opening files and calling following operations*/
void File::create(char *gridfilename, char *instructionsfilename){

	this->gridfilename=gridfilename;			/*Opens grid file*/
	gridfile=fopen( this->gridfilename, "r+" );

	if(!gridfile){		
		if(!(gridfile = fopen( this->gridfilename, "w+" ))){
			cerr << "Cannot open grid file" << endl;
			exit(1);
		}
	}
	
	create_grid();		/*Creates grid*/
	read_grid();		/*Reads the operations and places to grid*/
	fclose(gridfile);	/*Close gridfile*/
	

	this->instructionsfilename=instructionsfilename;	/*Opens instruction file*/
	instructionsfile=fopen( this->instructionsfilename, "r+" );

	if(!instructionsfile){		
		if(!(instructionsfile = fopen( this->instructionsfilename, "w+" ))){
			cerr << "Cannot open instructions file" << endl;
			exit(1);
		}
	}

	read_instructions();		/*Reads the instructions and executes them*/
	fclose(instructionsfile);	/*Close instructionsfile*/
	
	close();	/*Gives back dynamic pointers*/

}

void File::create_grid(){

	fscanf(gridfile,"%d",&this->row_number);	/*Reads first 2 number from the gridfile to form grid*/
	fscanf(gridfile,"%d",&this->column_number);

	grid=new char*[row_number]; /*Creates 2 dimensions grid array*/

	for (int i=0; i<row_number; i++){
		grid[i]=new char[column_number];
	}
	for (int i=0; i<row_number; i++){
		for (int j=0; j<column_number; j++){
			grid[i][j]=' ';
		}
	}

	operators_array = new Operator[row_number*column_number]; /*Takes a space from the memory to keep operations'structures*/

	cout << "A grid is created: " << row_number << " " << column_number << endl; /*Prints grid created*/
	
}


void File::plus_opr(char **temp_grid, char *type, int row, int column, int size){
	
	int counter=0; /*Determines the locations of plus by adding or subtracting from the column or row */

	/*Pluses can be placed at the bottom, top and two sides of the center so there is 4 placements states*/
	
	while(counter<=size){ /*Until its number reaches the size*/

		/*Placement state for the top of the center*/
		if(row-counter-1 > this->row_number-1 || row-counter-1 < 0 || column-1 > this->column_number-1 || column-1 < 0){ /*Checks borders*/
			border_error=true;
		}
		else if(grid[row-counter-1][column-1]!=' '){ 	/*If this place is not empty*/
			conflict_error=true;						/*There is conflict_error*/
		}
		else{
			temp_grid[row-counter-1][column-1]=*type;	/*if no error, places plus to the temp grid*/
		}
		/*Placement state for the bottom of the center*/
		if(row+counter-1 > this->row_number-1 || row+counter-1 < 0 || column-1 > this->column_number-1 || column-1 < 0){ /*Checks borders*/
			border_error=true;
		}
		else if(grid[row+counter-1][column-1]!=' '){ 	/*If this place is not empty*/
			conflict_error=true;						/*There is conflict_error*/
		}
		else{
			temp_grid[row+counter-1][column-1]=*type; 	/*if no error, places plus to the temp grid*/
		}
		/*Placement state for the left of the center*/
		if(row-1 > this->row_number-1 || row-1 < 0 || column-counter-1 > this->column_number-1 || column-counter-1 < 0){ /*Checks borders*/
			border_error=true;
		}
		else if(grid[row-1][column-counter-1]!=' '){	/*If this place is not empty*/
			conflict_error=true;						/*There is conflict_error*/
		}
		else{
			temp_grid[row-1][column-counter-1]=*type;	/*if no error, places plus to the temp grid*/
		}
		/*Placement state for the right of the center*/
		if(row-1 > this->row_number-1 || row-1 < 0 || column+counter-1 > this->column_number-1 || column+counter-1 < 0){ /*Checks borders*/
			border_error=true;
		}
		else if(grid[row-1][column+counter-1]!=' '){
			conflict_error=true;
		}
		else{
			temp_grid[row-1][column+counter-1]=*type;	/*if no error, places plus to the temp grid*/
		}
		counter++;
	}
}
void File::minus_opr(char **temp_grid, char *type, int row, int column, int size){
	
	int counter=0; /*Determines the locations of minus sign by adding or subtracting from the column*/

	/*Minus sign can be placed at the two sides of the center so there is 2 placements states*/
	
	while(counter<=size){ /*Until its number reaches the size*/

		/*Placement state for the left of the center*/
		if(row-1 > this->row_number-1 || row-1 < 0 || column-counter-1 > this->column_number-1 || column-counter-1 < 0){ /*Checks borders*/
			border_error=true;
		}
		else if(grid[row-1][column-counter-1]!=' '){	/*If this place is not empty*/
			conflict_error=true;						/*There is conflict_error*/
		}
		else{
			temp_grid[row-1][column-counter-1]=*type;	/*if no error, places minus sign to the temp grid*/
		}
		/*Placement state for the right of the center*/
		if(row-1 > this->row_number-1 || row-1 < 0 || column+counter-1 > this->column_number-1 || column+counter-1 < 0){
			border_error=true;
		}
		else if(grid[row-1][column+counter-1]!=' '){
			conflict_error=true;
		}
		else{
			temp_grid[row-1][column+counter-1]=*type;
		}
		counter++;
	}
}
void File::multiply_opr(char **temp_grid, char *type, int row, int column, int size){
	
	int counter=0;	/*Determines the locations of multiply sign by adding or subtracting from the column and row */
	
	/*Multiply sign can be placed at cross directions of the center so there is 4 placements states*/

	while(counter<=size){

		/*Placement state for the top-left of the center*/
		if(row-counter-1 > this->row_number-1 || row-counter-1 < 0 || column-counter-1 > this->column_number-1 || column-counter-1 < 0){ /*Checks borders*/
			border_error=true;
		}
		else if(grid[row-counter-1][column-counter-1]!=' '){	/*If this place is not empty*/
			conflict_error=true;								/*There is conflict_error*/
		}
		else{
			temp_grid[row-counter-1][column-counter-1]=*type;	/*if no error, places multiply sign to the temp grid*/
		}
		/*Placement state for the bottom-left of the center*/
		if(row+counter-1 > this->row_number-1 || row+counter-1 < 0 || column-counter-1 > this->column_number-1 || column-counter-1 < 0){
			border_error=true;
		}
		else if(grid[row+counter-1][column-counter-1]!=' '){
			conflict_error=true;
		}
		else{
			temp_grid[row+counter-1][column-counter-1]=*type;
		}
		/*Placement state for the top-right of the center*/
		if(row-counter-1 > this->row_number-1 || row-counter-1 < 0 || column+counter-1 > this->column_number-1 || column+counter-1 < 0){
			border_error=true;
		}
		else if(grid[row-counter-1][column+counter-1]!=' '){
			conflict_error=true;
		}
		else{
			temp_grid[row-counter-1][column+counter-1]=*type;
		}
		/*Placement state for the bottom-right of the center*/
		if(row+counter-1 > this->row_number-1 || row+counter-1 < 0 || column+counter-1 > this->column_number-1 || column+counter-1 < 0){
			border_error=true;
		}
		else if(grid[row+counter-1][column+counter-1]!=' '){
			conflict_error=true;
		}
		else{
			temp_grid[row+counter-1][column+counter-1]=*type;
		}
		counter++;
	}
}
void File::division_opr(char **temp_grid, char *type, int row, int column, int size){
	
	int counter=0; /*Determines the locations of division sign by adding or subtracting from the column and row */

	/*Division sign can be placed at 2 cross directions of the center so there is 2 placements states*/

	while(counter<=size){

		/*Placement state for the bottom-left of the center*/
		if(row+counter-1 > this->row_number-1 || row+counter-1 < 0 || column-counter-1 > this->column_number-1 || column-counter-1 < 0){ /*Checks borders*/
			border_error=true;
		}
		else if(grid[row+counter-1][column-counter-1]!=' '){	/*If this place is not empty*/
			conflict_error=true;								/*There is conflict_error*/
		}
		else{
			temp_grid[row+counter-1][column-counter-1]=*type;	/*if no error, places division sign to the temp grid*/
		}
		/*Placement state for the top-right of the center*/
		if(row-counter-1 > this->row_number-1 || row-counter-1 < 0 || column+counter-1 > this->column_number-1 || column+counter-1 < 0){
			border_error=true;
		}
		else if(grid[row-counter-1][column+counter-1]!=' '){
			conflict_error=true;
		}
		else{
			temp_grid[row-counter-1][column+counter-1]=*type;
		}
		counter++;
	}
}

void File::read_grid(){

	char *type=new char[5]; /*For reading operations' type*/
	int row;
	int column;
	int size;
	int success_opr=0; /*Increments number of the success operations, necessary to put the success operation into operation array*/
	
	this->border_error=false;
	this->conflict_error=false;


	while( !feof(gridfile) ){

		char **temp_grid = new char*[row_number];	/*I need temp grid in order not to affect to grid*/
		for (int i=0; i<row_number; i++){			/*Created temp grid*/
			temp_grid[i] = new char[column_number];
		}

		for (int i=0; i<row_number; i++){	/*To make empty*/
			for (int j=0; j<column_number; j++){
				temp_grid[i][j]=' ';
			}
		}

		fscanf(gridfile,"%s %d %d %d",type, &row, &column, &size); /*Reads the rest of the file*/

		if( feof(gridfile) ) 
			break;  
		
		if(*type=='+'){ 		/*If operation is plus, calls the plus function for the placement*/
			plus_opr(temp_grid, type, row, column, size);
		}
		else if(*type=='-'){	/*If operation is minus, calls the minus function for the placement*/
			minus_opr(temp_grid, type, row, column, size);
		}
		else if(*type=='x'){	/*If operation is multiply, calls the multiply function for the placement*/
			multiply_opr(temp_grid, type, row, column, size);
		}
		else if(*type=='/'){	/*If operation is division, calls the division function for the placement*/
			division_opr(temp_grid, type, row, column, size);
		}


		if(border_error){  /*If border or conflict error occurs in the operations functions*/
			cout << "BORDER ERROR: Operator " << type << " with size " << size << " can not be placed on ("
				<< row << "," << column << ")." << endl;
		}
		if(conflict_error){
			cout << "CONFLICT ERROR: Operator " << type << " with size " << size << " can not be placed on ("
				<< row << "," << column << ")." << endl;
		}
		if (!border_error && !conflict_error){ /*If there is no error*/

			operators_array[success_opr].type=new char[1]; /*Put the operation's features into the structure*/
			*operators_array[success_opr].type=*type;
			operators_array[success_opr].row=row;
			operators_array[success_opr].column=column;
			operators_array[success_opr].size=size;
			operators_array[success_opr].its_grid=temp_grid;
			
			for (int i=0; i<row_number; i++){ 
				for (int j=0; j<column_number; j++){
					if (temp_grid[i][j]!=' '){
						grid[i][j]=temp_grid[i][j]; /*Places to grid*/

					}
				}
			}
			success_opr++;
			cout << "SUCCESS: Operator " << *type << " with size " << size << " is placed on (" << row << "," << column << ")." << endl;
		}

		this->border_error=false;
		this->conflict_error=false;
		

		if( feof(gridfile) ) 
			break;  
	}

	this->opr_array_size=success_opr;
	delete[] type;
	
}

void File::read_instructions(){

	char *type=new char[5];
	int row;
	int column;
	int size;
	int index=0;
	
	this->border_error=false;
	this->conflict_error=false;


	while( !feof(instructionsfile) ){

		char **temp_grid = new char*[row_number]; /*Creates temp grid*/
		for (int i=0; i<row_number; i++){
			temp_grid[i] = new char[column_number];
		}


		for (int i=0; i<row_number; i++){ 		/*To make empty*/
			for (int j=0; j<column_number; j++){
				temp_grid[i][j]=' ';
			}
		}
		

		fscanf(instructionsfile,"%s",type); /*Reads the file*/
		fscanf(instructionsfile,"%d %d %d", &row, &column, &size);

		if( feof(gridfile) ) 
			break;  

		if(this->grid[row-1][column-1]!=' '){ /*If there is a sign at given location, do the desired mov*/
			

			for(int a=0; a < this->opr_array_size; a++){ /*Finds the index of the necassary operation*/
				for (int i=0; i<row_number; i++){		/*Thus I can reach the center*/
					for (int j=0; j<column_number; j++){
						if (operators_array[a].its_grid[row-1][column-1]==this->grid[row-1][column-1]){
							index=a;
							break;
						}
					}
				}
			}

			for (int i=0; i<row_number; i++){  /*Removes the places of the operation from the grid in order to move*/
				for (int j=0; j<column_number; j++){
					if (operators_array[index].its_grid[i][j]==this->grid[i][j]){
					
						this->grid[i][j]=' ';
					}
				}
			}
			
			/*Holds the new rows or colums in terms of move types*/
			if(strcmp(type, "MVR") == 0){ 
				row=operators_array[index].row;
				column=operators_array[index].column+size;
			}
			else if(strcmp(type, "MVL") == 0){
				row=operators_array[index].row;
				column=operators_array[index].column-size;
			}
			else if(strcmp(type, "MVU") == 0){
				row=operators_array[index].row-size;
				column=operators_array[index].column;
			}
			else if(strcmp(type, "MVD") == 0){
				row=operators_array[index].row+size;
				column=operators_array[index].column;
			}

			/*Calls the placement function in terms of the type of operation with the new row or column*/
			if(*operators_array[index].type=='+'){
				plus_opr(temp_grid, operators_array[index].type, row, column, operators_array[index].size);
			}
			else if(*operators_array[index].type=='-'){
				minus_opr(temp_grid, operators_array[index].type, row, column, operators_array[index].size);
			}
			else if(*operators_array[index].type=='x'){
				multiply_opr(temp_grid, operators_array[index].type, row, column, operators_array[index].size);
			}
			else if(*operators_array[index].type=='/'){
				division_opr(temp_grid, operators_array[index].type, row, column, operators_array[index].size);
			}

			/*If there is border or conflict error with the new location*/
			if(border_error){ 
				cout << "BORDER ERROR: " << *operators_array[index].type << " can not be moved from (" << operators_array[index].row << "," << operators_array[index].column << ") to (" 
					<< row << "," << column << ")."<< endl;
			
			}
			if(conflict_error){
				cout << "CONFLICT ERROR: " << *operators_array[index].type << " can not be moved from (" << operators_array[index].row << "," << operators_array[index].column << ") to (" 
					<< row << "," << column << ")."<< endl;
			}

			if (!border_error && !conflict_error){ /*If there is no border or conflict error*/
	
				cout << "SUCCESS: " << *operators_array[index].type << " moved from ("  << operators_array[index].row << "," << operators_array[index].column << ") to (" 
					<< row << "," << column << ")."<< endl;


				operators_array[index].row=row; /*Updates the row or column of the operation*/
				operators_array[index].column=column;

				for(int i = 0; i < this->row_number; ++i) { /*Deletes old operation grid which holds old places*/
					delete[] operators_array[index].its_grid[i];   
				}
    
				delete[] operators_array[index].its_grid;

				operators_array[index].its_grid=temp_grid; /*Temp grid will be new operation grid which holds new places*/
			}
			else{ /*If there is error, deletes the temp grid because I do not need it*/
			
				for(int i = 0; i < this->row_number; ++i) {
					delete[] temp_grid[i];   
				}
    
				delete[] temp_grid;
			}

			for (int i=0; i<row_number; i++){ /*And puts the operation's places to the grid*/
				for (int j=0; j<column_number; j++){
					if (operators_array[index].its_grid[i][j]!=' '){
						grid[i][j]=operators_array[index].its_grid[i][j];

					}
				}
			}

			border_error=false;
			conflict_error=false;
			index=0;
		}

		if( feof(instructionsfile) ) 
			break;  
	}

	delete[] type;
}


/*Gives back all dynamic pointers to the memory*/
void File::close(){
	for(int i = 0; i < this->row_number; ++i) {
        delete[] grid[i];   
    }
    /*Free the array of pointers*/
    delete[] grid;

	for(int i = 0; i < this->opr_array_size; ++i) {
	
		for(int j = 0; j < this->row_number; ++j) {
			delete[] operators_array[i].its_grid[j];   
		}
		delete[] operators_array[i].its_grid;
		delete[] operators_array[i].type;
	}
	delete[] operators_array;
}


/*Main part*/

typedef File Datastructure; 

Datastructure data;

int main(int argc, char *argv[]){	
	
	data.create(argv[1], argv[2]);	/*Take filenames and sent the create function*/
	
	return EXIT_SUCCESS;
}
