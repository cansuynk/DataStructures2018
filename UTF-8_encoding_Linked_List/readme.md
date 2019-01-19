## PROBLEM
This code reads an UTF-8 text file, split each unique character to create a language model and find the possibility of predicting the next word/character given a previous word/characters.
### Example:
The probability “k” comes right after “m” is calculated like this:

![prob](https://user-images.githubusercontent.com/43931498/51433165-ea499700-1c55-11e9-9ab4-be453e18fee1.PNG)

Important Notes:
*	Linked list data structure is used to create the language model.
*	Case insensitive (“K” and “k” is the same).
*	Language model is in alphabetical order.
*	Space is stored as “<SP>”
*	The given file must be read two times.
*	My code can be used also for Turkish characters (All I, İ, ı and i are taken as i).
*	Language Model structure is like this:

Example sentence: 

t h i s \<SP> i s \<SP> e x a m p l e \<SP> s e n t e n c e.\<SP>

![structure](https://user-images.githubusercontent.com/43931498/51433185-51ffe200-1c56-11e9-95e3-808122db1f09.PNG)



The language model is printed like this:

![output](https://user-images.githubusercontent.com/43931498/51433224-26312c00-1c57-11e9-9df7-900d711253ee.PNG)


The probability of “en” in the given file:

![output2](https://user-images.githubusercontent.com/43931498/51433240-642e5000-1c57-11e9-8a93-4ee9ba529cd9.PNG)
