#ifndef GRIDOPERATIONS_DYNAMIC_H
#define GRIDOPERATIONS_DYNAMIC_H
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "operators.h"



struct File{
	
	char *gridfilename;
	FILE *gridfile;

	char *instructionsfilename;
	FILE *instructionsfile;

	Operator *operators_array; /*Keeps operators' structures in the dynamic array*/
	int opr_array_size;        /*Holds operators size*/
	
	
	void create(char *gridfilename, char *instructionsfilename); /*For opening files and calling following operations*/
	
	void create_grid(); 		/*Forms grid with the given number of rows and columns*/
	void read_grid();			/*Reads grid file and executes its operations*/
	void read_instructions();	/*Reads instruction file and executes its operations*/


	void plus_opr(char **temp_grid, char *type, int row, int column, int size);		/*Places plus operation on the grid and checks border and conflict errors*/
	void minus_opr(char **temp_grid, char *type, int row, int column, int size); 	/*Places minus operation on the grid and checks border and conflict errors*/
	void multiply_opr(char **temp_grid, char *type, int row, int column, int size); /*Places multiply operation on the grid and checks border and conflict errors*/
	void division_opr(char **temp_grid, char *type, int row, int column, int size); /*Places division operation on the grid and checks border and conflict errors*/

	void close();		/*Gives back all dynamic pointers to the memory*/

	char **grid; 		/*2 dimensions dynamic grid array holds operations' places*/
	int row_number; 	/*Grid's row number*/
	int column_number;	/*Grid's column number*/

	bool border_error;
	bool conflict_error;

};
#endif
