#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <ctype.h>
#include <wchar.h>
#include <cwchar>
#include <cstdio>
#include <clocale>
#include <locale.h>
#include "languagemodel.h"

using namespace std;

/* @Author
* Student Name: Cansu YANIK
* Student ID : 150170704
* Date: 08.11.2018 */

/*Important Notes:
* Test files have to be UTF-8 encoded not UTF-8 BOM or others for my code to work properly
* And for speacial characters please write this format when testing:  example: ./lm testfile.txt a "<SP>"  OR ./lm testfile.txt "a" "<SP>"
* And we assumed that for every new line, we have to add space
* Thank you so much*/


wchar_t* language_model::turkish_letter_encoding(int decimal_number, wchar_t* char1){ /*This fuction is for Turkish letters encoding*/
	
	/*These are hex and octal representations of the üöçğş letters*/
	/*I use them to be able to write these letters on the screen of the SSH*/
	
	const char *u_withdots="\x00fc";
	const char *o_withdots="\x00f6";
	const char *c_withdot="\x00e7";
	const char *g_withdot="\360";
	const char *s_withdot="\376";

	
	if(decimal_number==305){*char1='i';} 	/*We assumed that all "iİıI" cases will be i*/
											/*So if the letter is ı, converts it to i*/
		
		/*These conditions are for "üöçğş" letters and to be able to save them*/
		
	if(decimal_number==252){*char1=(wchar_t)*u_withdots;} /*If letter is ü, converts it's hex representation to wchar*/
	if(decimal_number==246){*char1=(wchar_t)*o_withdots;} /*If letter is ö, converts it's hex representation to wchar*/
	if(decimal_number==231){*char1=(wchar_t)*c_withdot;}  /*If letter is ç, converts it's hex representation to wchar*/
	if(decimal_number==287){*char1=(wchar_t)*g_withdot;}  /*If letter is ğ, converts it's hex representation to wchar*/
	if(decimal_number==351){*char1=(wchar_t)*s_withdot;}  /*If letter is ş, converts it's hex representation to wchar*/
	
	return char1;
	
}

void language_model::readData(const char * filename){
	 
	setlocale(LC_ALL, "en_US.UTF-8"); /*Set locale encoding as a US.UTF-8 to be able to read nonEnglish characters*/
	
	wchar_t char1[2];  		/*Holds characters from the file*/
	wchar_t char2[2];
	
	int decimal_number; 	/*Holds decimal number of characters from the file*/
	int decimal_number2;


	FILE *file;
	file=fopen( filename, "r+" ); 	/*Opens file*/

	if(!file){						/*Checks file opens correctly or not*/
		if(!(file = fopen( filename, "w+" ))){
			cerr << "Cannot open the file" << endl;
			exit(1);
		}
	}
	
	vocabularylist= new vocab_list;  /*Creates vocabulary list*/
	vocabularylist->create();
	
	
	while( !feof(file) ){ 		    /*Reads file until it is end*/
	
		fgetws(char1, 2, file); 	/*Takes char from file*/
		
		if(feof(file) || char1==NULL || *char1==EOF){ /*If end of file; break*/
			break;
		}
	
		if(*char1=='\n'){
			continue;
		}
		*char1= towlower(*char1);  		/*If there is upper case letter, converts it to lower*/
		decimal_number=(int)*char1;  	/*Converts letter to decimal form*/

		
		*char1=*turkish_letter_encoding(decimal_number, char1);
				
		vocabularylist->add_char(char1);  /*Adds letter to vocabularylist*/
		
	}
	
	fseek(file, 0, SEEK_SET); /*Sets file pointer at the beginning of the file*/
	int update_char=1;        /*This is a flag to know in which order the characters will be changed*/
	
	fgetws(char1, 2, file);
	fgetws(char2, 2, file);
	
	
	while( !feof(file) ){
		
		*char1= towlower(*char1);  	 /*Takes lower*/
		*char2= towlower(*char2);
		
		decimal_number=(int)*char1;  /*Converts letters to decimal representation*/
		decimal_number2=(int)*char2;
		
		*char1=*turkish_letter_encoding(decimal_number, char1);
		*char2=*turkish_letter_encoding(decimal_number2, char2);
		
				
		if(feof(file) || char1==NULL || *char1==EOF || char2==NULL || *char2==EOF ){
			break;
		}
		
		if(update_char%2==1){   /*At every readig, I increment "update_char" and decide which char1 or char2 will be moved*/
			vocabularylist->add_occurence(char1,char2); /*If "update_char" is a times of 2, char1 will be moved and char2 stays */
			fgetws(char1, 2, file);
			
			if(*char1=='\n'){
				while(*char1=='\n'){
					*char1=(wchar_t)(' ');
					vocabularylist->add_occurence(char2,char1);
					fgetws(char1, 2, file);
				}				
				fgetws(char2, 2, file);
				update_char=0;
			}
		}
		else{
			vocabularylist->add_occurence(char2,char1); /*If not, char2 will be moved and char1 stays*/
			fgetws(char2, 2, file);
			
			if(*char2=='\n'){
				while(*char2=='\n'){
					*char2=(wchar_t)(' ');
					vocabularylist->add_occurence(char1,char2);
					fgetws(char2, 2, file);
				}
				*char1=*char2;
				fgetws(char2, 2, file);
				update_char=0;
			}
		}
		update_char++;
	}
	if(update_char%2==1){     /*For the last line, adds space after the last letter(because we assumed that every line ends with a space)*/
		*char2=(wchar_t)(' ');
		decimal_number=(int)*char1;    /*If last letter is a Turkish letter, we need do encoding*/
		*char1=*turkish_letter_encoding(decimal_number, char1);
		vocabularylist->add_occurence(char1,char2);
	}
	else{
		*char1=(wchar_t)(' ');
		decimal_number=(int)*char2;
		*char1=*turkish_letter_encoding(decimal_number, char2);
		vocabularylist->add_occurence(char2,char1);
	}		
		
	fclose(file);

}

void vocab_list::create(){
	head=NULL;  /*Initially our list is empty*/
}

double vocab_list::update_turkish_letter_decimal(double turkish_letter_decimal){
	
	/*I printed Turkish letters to the secreen, I saw that these negative numbers are printed which is below*/
	/*Therefore, I thinked that, If I equalize them to proper decimal value, I can insert them alphabetically*/
	/*For examle, the decimal value of c is 99 and d is 100, so I gave 99.5 for the ç to be able to insert it.*/
	
	if(turkish_letter_decimal==-4.0){turkish_letter_decimal=117.5;}  /*These conditions are for "üöçğş" letters*/
	else if(turkish_letter_decimal==-10.0){turkish_letter_decimal=111.5;}	
	else if(turkish_letter_decimal==-25.0){turkish_letter_decimal=99.5;}	
	else if(turkish_letter_decimal==-16.0){turkish_letter_decimal=103.5;}
	else if(turkish_letter_decimal==-2.0){turkish_letter_decimal=115.5;}
	
	return turkish_letter_decimal;
}

void vocab_list::add_char(wchar_t ch[2]){ 

	int punctuation=ispunct(*ch); /*If letter is a punctuation or white_space, this letter will be stored at the end of the list*/
	int white_space=0;
	
	if(*ch==' ')
		white_space=1;
		

	vocab_node *traverse;  /*node pointer*/
	traverse=head;

	vocab_node *newnode;   /*newnode*/
	newnode = new vocab_node;
	
	*newnode->character=*ch;
	newnode->list=NULL;
	
	double traverse_decimal=0.0;       /*These are for holding decimal values of the letters*/
	double turkish_letter_decimal=0.0;
	
	turkish_letter_decimal=((double)*ch)*1.0;
	turkish_letter_decimal=update_turkish_letter_decimal(turkish_letter_decimal); /*If incoming letter is a Turkish letter, I must give it to proper decimal value.*/

	
	if (head == NULL){	/*First element(if list is empty)*/
		head = newnode;
		newnode->next=NULL;
		return;
	}
	
	if(white_space == 0 && punctuation == 0){ /*no space no punch just letter addition to the list*/
		/*For insertion after head*/
		
		traverse_decimal=((double)*(head->character))*1.0;  /*If letter which is in the list is a Turkish letter, I must give it to proper decimal value.*/
		traverse_decimal=update_turkish_letter_decimal(traverse_decimal);
		
		if (turkish_letter_decimal<traverse_decimal || *(head->character) ==' ' || ispunct(*(head->character)) !=0){ /*If letter must be in the beginning of the list*/
			newnode->next = head;
			head = newnode;
			return;
		}
		if(*ch == *(head->character)){ /*If incoming letter and the letter beginning of the list are the same*/
			return;
		}
		
		while ( traverse->next ){   /*For insertion somewhere in the list*/
			traverse_decimal=((double)*(traverse->next->character))*1.0;
			traverse_decimal=update_turkish_letter_decimal(traverse_decimal);
			
			if(turkish_letter_decimal<traverse_decimal || *(traverse->next->character) ==' ' || ispunct(*(traverse->next->character)) !=0){
				newnode->next=traverse->next;
				traverse->next=newnode;       /*Travel until next node's character bigger than the letter or next character is a special ch.*/
				return;	
			}
			else if(*ch == *(traverse->next->character)){  /*If letter already exists, return*/
				return;
			}
			traverse = traverse->next;
		}

		if(*ch != *(traverse->character)){ /*if current character not equals to incoming letter, then adds letter to the list*/
			traverse->next=newnode;
			newnode->next=NULL;
		}
	}
	else{  /*If the letter is a special character*/

		if(*ch == *(head->character)){
			return;
		}
		if (*(head->character) ==' ' || ispunct(*(head->character)) !=0 ){ /*If first character of the list is a whitespace*/
																		   /*I can add new letter at the beginning of the list*/
			newnode->next = head;
			head = newnode;
			return;
		}
		
		while( traverse->next && *(traverse->next->character) != ' ' && ispunct(*(traverse->next->character)) == 0 ){
			traverse = traverse->next;
		}
		if(traverse->next==NULL){    /*If I am in the last node*/
			traverse->next=newnode;  /*I can add the letter at the end of the list*/
			newnode->next=NULL;		
		}
		else{  						 /*If this is not last node*/
			while ( traverse->next->next ){ 
				
				if(*ch == *(traverse->next->character)){
					return;
				}		
				traverse = traverse->next;
			}

			if(*ch != *(traverse->next->character)){
				newnode->next=traverse->next;
				traverse->next=newnode;
				return;
			}
		}	
	}	
}

void vocab_list::add_occurence(wchar_t ch[2], wchar_t ch2[2]){

	vocab_node *traverse;  /*node pointer*/
	traverse=head;
	
	
	while(traverse){  			 /*Travels the list until finding the desired letter*/
		if(*(traverse->character)==*ch)
			break;
		else
			traverse=traverse->next;
	}
	
	if(traverse){ 				/*If traverse is not NULL*/
		occur_node *newmiddlenode;   /*Creates newnode (subnode)*/
		newmiddlenode = new occur_node;
		*(newmiddlenode->character)=*ch2;
		
		
		
		occur_node *traverse2;  /*subnode pointer*/
		traverse2=traverse->list;
		

		newmiddlenode->next=NULL;
		if(traverse->list==NULL){  /*If this is first subnode*/
			traverse->list = newmiddlenode;
			traverse2 =newmiddlenode;
			newmiddlenode->next=NULL;
			newmiddlenode->occurence=1;
			return;
		}
		else 
		
		while(traverse2->next){  /*If this is not first subnode*/
			
			if(*(traverse2->character)==*ch2){  /*If there is already same subnode(second letter), increments its occurence*/
				traverse2->occurence++;
				return;		
			}
			traverse2=traverse2->next;
			
		}
		if(*(traverse2->character)==*ch2){  /*Checks the last subnode*/
				traverse2->occurence++;
				return;		
		}
		traverse2->next=newmiddlenode;   /*If not, adds new subnode at the end of the list*/
		newmiddlenode->next=NULL;
		newmiddlenode->occurence=1;
	}
}

void language_model::print(){
	
	vocab_node *traverse;  /*node pointer*/
	traverse=vocabularylist->head;
	
	occur_node *traverse2;  /*subnode pointer*/
	traverse2=traverse->list;
	
	cout << "Language Model:" << endl;
	
	while(traverse->next){ 
		
		if(*(traverse->character)==' ')   /*Prints character*/
			wcout << "<SP>:" << endl;     /*If it is whitespace, prints <SP>*/
		else
			wcout << *(traverse->character) << ":" << endl;
			
		
		while(traverse2){                 			/*Prints co-occurencecharacter and its count*/
			if(*(traverse2->character)==' '){		/*If it is whitespace, prints <SP>*/
				wcout << "\t<<SP>" << ",";
				wcout << traverse2->occurence << ">" << endl;	
			}
				
			else{
				wcout << "\t<" << *(traverse2->character) << ",";
				wcout << traverse2->occurence << ">" << endl;
			}	
			traverse2=traverse2->next;
			
		}
		
		traverse=traverse->next;
		traverse2=traverse->list;
		
	}
	
	traverse2=traverse->list;					/*Because the above while loop ends without printing last node of the list,*/
	if(*(traverse->character)==' ')				/*I wrote another while loop to print last node and its subnodes*/
		wcout << "<SP>:" << endl;
	else
		wcout << *(traverse->character) << ":" << endl;
		
	while(traverse2){
		if(*(traverse2->character)==' '){
			wcout << "\t<<SP>" << ",";
			wcout << traverse2->occurence << ">" << endl;	
		}
			
		else{
			wcout << "\t<" << *(traverse2->character) << ",";
			wcout << traverse2->occurence << ">" << endl;
		}	
		traverse2=traverse2->next;
		
	}
	
}

int vocab_list::get_occurence(wchar_t ch[2]){
	
	vocab_node *traverse;  /*node pointer*/
	traverse=head;
	
	int total_count=0; 	   /*Holds the result*/
	
	while(traverse){  /*Finds desired letter*/
		if(*(traverse->character)==*ch)
			break;
		else
			traverse=traverse->next;
		
		if(traverse==NULL)
			return 0;
	}
	
	
	
	occur_node *traverse2;  /*subnode pointer*/
	traverse2=traverse->list;
		
	
	while(traverse2){ /*Adding all the union co-occurrences of letter with other characters*/
		total_count=total_count+traverse2->occurence;
		traverse2=traverse2->next;	
		
	}
	
	return total_count;
	
}

int vocab_list::get_union_occurence(wchar_t ch[2], wchar_t ch2[2]){
	
	vocab_node *traverse;  /*node pointer*/
	traverse=head;
	
	int union_count=0;
	
	while(traverse){   /*Finds the letter*/
		if(*(traverse->character)==*ch)
			break;
		else
			traverse=traverse->next;
		
		if(traverse==NULL)
			return 0;
	}
	
	
	occur_node *traverse2;  /*subnode pointer*/
	traverse2=traverse->list;
	
	while(traverse2){    /*Finds and returns desired subletter's occurence*/
		if(*(traverse2->character)==*ch2){
			union_count=traverse2->occurence;
			return union_count;		
		}
		traverse2=traverse2->next;
		
		if(traverse2==NULL)
			return 0;
	}	
	return union_count;
}

double language_model::calculateProbability(char *ch, char *ch2){
	
	wchar_t char1[2];
	wchar_t char2[2];
	
	char space[10]="<SP>";
	
	if(*ch==*space){  /*If one of the letters is <SP>, converts to ' '*/
		*ch=' ';		
	}	
	if(*ch2==*space){
		*ch2=' ';
	}

	*char1 = (wchar_t)*ch;     /*Converts given char to UTF-8 encoding format*/
	*char2 = (wchar_t)*ch2;
	
	*char1= towlower(*char1);  	 /*Takes lower*/
	*char2= towlower(*char2);
	
	double result=0.0;
	result= (vocabularylist->get_union_occurence(char2,char1) *1.0) / (vocabularylist->get_occurence(char2)*1.0); /*Calculation*/
	
	if(vocabularylist->get_union_occurence(char2,char1)==0)
		return 0;
	if(vocabularylist->get_occurence(char2)==0)  /*If the list does not have the letter*/
		return 0;
	
	return result;	
	
}


typedef language_model Datastructure;
typedef vocab_node V_Node;
typedef occur_node O_Node;

Datastructure Model;   /*Creates a language Model*/

int main(int argc, char *argv[]){
	
	Model.readData(argv[1]);
	if (argc==2){
		Model.print();
	}
	else{
		double result=Model.calculateProbability(argv[3],argv[2]);
		wcout << "Probability = " << result << endl;
	}
	
	return EXIT_SUCCESS;
}
