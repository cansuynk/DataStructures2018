## PROBLEM
This code reads an UTF-8 text file, split each unique character to create a language model and find the possibility of predicting the next word/character given a previous word/characters.
### Example:
The probability “k” comes right after “m” is calculated like this:

Important Notes:
•	Linked list data structure is used to create the language model.
•	Case insensitive (“K” and “k” is the same).
•	Language model is in alphabetical order.
•	Space is stored as “<SP>”
•	The given file must be read two times.
•	My code can be used also for Turkish characters (All I, İ, ı and i are taken as i).
•	Language Model structure is like this:

Example sentence: 

t h i s <SP> i s <SP> e x a m p l e <SP> s e n t e n c e.<SP>




The language model is printed like this:



The probability of “en” in the given file:

