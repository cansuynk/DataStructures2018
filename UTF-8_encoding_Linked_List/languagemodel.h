#ifndef LIST_H
#define LIST_H

#include "vocablist.h"

/* @Author
* Student Name: Cansu YANIK
* Student ID : 150170704
* Date: 08.11.2018 */


struct language_model {

	vocab_list *vocabularylist;
	void readData (const char *);
	double calculateProbability (char*, char*);
	
	wchar_t* turkish_letter_encoding(int , wchar_t* );  /*This fuction is for Turkish letters encoding*/

	void print();
};

#endif
