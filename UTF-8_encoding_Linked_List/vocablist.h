#ifndef VOCABLIST_H
#define VOCABLIST_H

#include "vocabnode.h"

/* @Author
* Student Name: Cansu YANIK
* Student ID : 150170704
* Date: 08.11.2018 */

struct vocab_list
{
	vocab_node *head;
	void create();
	void print();
	void add_char(wchar_t* );
	void add_occurence(wchar_t* , wchar_t* );
	int get_occurence(wchar_t* );
	int get_union_occurence (wchar_t* , wchar_t* );
	
	double update_turkish_letter_decimal(double );  /*this function is for giving proper decimal numbers to the turkish letters*/
};


#endif
