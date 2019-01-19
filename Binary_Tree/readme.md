## PROBLEM
This Code reads numbers from a text file in order to construct binary tree. Text file must have two lines. 
*	First line contains positive integers which are numbers of the tree nodes. 
*	Second line contains a target number.
We're trying to find two best paths (left and right) which start from the root of the tree and sums up to this target value.

### Important Notes:
*	I used preorder traversal for searches and finding paths.
*	There are two options that are recursion version and iterative version by using stack to solve the problem. 
*	To be able to create the binary tree, I used a dynamic array. I used this method:

In the text file, (2*a)th integer is the right child of the ath  node and (2*a+1)th integer is the left child of the ath node.
*	There can be two best paths: left and right, one best path: just left or right, or no best paths.

### Examples:

First line of the file: 5 3 1 7 10 12 2

![tree](https://user-images.githubusercontent.com/43931498/51431815-ac5a6c00-1c71-11e9-8caf-a2a2dbb25e3b.png)

* Second line of the file:18  
Result:  
Path Found: 5 3 10  
Path Found: 5 1 12  

* Second line of the file:15  
Result:  
Path Found: 5 3 7  
Path Found: No Path Found  
 
* Second line of the file:6  
Result:  
Path Found: No Path Found  
Path Found: 5 1   

* Second line of the file:22  
Result:  
Path Found: No Path Found  
Path Found: No Path Found  
