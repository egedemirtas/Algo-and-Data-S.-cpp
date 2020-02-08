//Ege Demirtas 25401
#include <iostream>
#include <string>
using namespace std;

//template class for stack
template <class Object>
class Stack {
private:
	struct Node{ //create stack behaving nodes
		Object element;
		Node *next;
		Node(Object y, Node *x = NULL)
			:element(y), next(x){}
	};
  	Node *top; //only access is from to top of stack
public:
	Stack(); //constructor
  	void push(Object newItem);
  	Object pop();
  	bool isEmpty();
	void printStack();
	void findDelete(Object item);
};

template <class Object>
Stack<Object>:: Stack(){  //top should point to null since the stack is first created empty
	top = NULL;
}

template <class Object>
bool Stack<Object>:: isEmpty(){ //return true if stack is empty
	if(top == NULL){
		return true;
	}
	return false;
}

template <class Object>
void Stack<Object>:: push(Object newItem){ //pushes a new Node(containing an object) to the stack from the top
	top = new Node(newItem, top);
}

template <class Object>
Object Stack<Object>:: pop(){ //pops the top object of the stack if the stack is not empty, if it is empty do nothing
	if(!isEmpty()){
		Node *currentTop = top;
		Object topChar = currentTop->element;
		top = top->next;
		delete currentTop;
		return topChar;
	}
}

template <class Object>
void Stack<Object>:: printStack(){ //prints the objects in the stack
	Stack<Object> copyStack;

	while(!isEmpty()){ //empty out the stack and push it to a copy stack
		copyStack.push(pop());
	}
	while(!copyStack.isEmpty()){ //empty out the copystack whlie printing the objects in it
		cout << copyStack.pop();
	}
}

template <class Object>
void Stack<Object>:: findDelete(Object item){ // gets item as parameter and deletes the item from the stack
	Stack<Object> stackCopy;
	while(!isEmpty()){ //push all the objects to another stack except the searched item
		if(top->element == item){
			pop();
		}else{
			stackCopy.push(pop());
		}
	}
	while(!stackCopy.isEmpty()){ // push all the objects to the emptied stack, now we have the stack without the item
		push(stackCopy.pop());
	}
}

template <class Object>
Stack<Object> & reverseStack(Stack<Object> & stackChar){ //reverses the order of the stack
	Stack<Object> stackCopy;
	while(!stackChar.isEmpty()){ //push all the elements to another elements, this way it is reversed
		stackCopy.push(stackChar.pop());
	}
	return stackCopy; // return the reversed stack
}

int main(){
	string word;
	cout << "Please enter a word\n";
	cin >> word;

	while(word.find('1') == string::npos){ //give the user warning if the input doesnt contain 1, repeat until 1 is found
		cout << "Please enter a new word, must include 1 for output!\n";
		cin >> word;
	}

	Stack<char> stackChar; //store the charaters of the input here
	Stack<char> stackStore; //if 3 is detected, pop the char from stackChar, and push it here, (simulation of left arrow)
	
	for(int i=0; i < word.length(); i++){ //go through the input, character by character
		if(word.at(i) == '1'){ //print stackChar
			/*if stackStore is not empty, push the chars to the stackChar to complete the output,
			even if the cursor is at the middle of the string, the whole string should be visible to the user, 
			since we are simulating a text editor*/
			while(!stackStore.isEmpty()){ 
				stackChar.push(stackStore.pop());
			}
			stackChar.printStack();
			break;
		}else if(word.at(i) == '2'){ //pop item from stackChar to delete a character
			stackChar.pop();
		}
		/*pop the last char at stackChar and push it to the stackStore, feels like pressing left arrow
		if stackChar empty do nothing (the cursor is at the beginning, no need for error output)*/
		else if(word.at(i) == '3'){ 
			if(!stackChar.isEmpty()){
				stackStore.push(stackChar.pop());
			}
		}else if(word.at(i) == '4'){ //simulation of right arrow
			if(!stackStore.isEmpty()){ //if stackStore is not empty, pop char and push it to stackChar; if stackStore is empty then the cursor is at the end, so nothing will be done
				stackChar.push(stackStore.pop());
			}
		}else if(word.at(i) == '5'){ //reverse the order of stackChar
			stackChar = reverseStack(stackChar);
		}else if(word.at(i) == '6'){
			stackChar.findDelete(word.at(i+1)); //delete the char after 6 in the stackChar
			i++; //since the char after 6 is deleted from the stackChar, it should not be added to stackChar in the next iteration, it is skipped
		}
		else{ //if the char is not a special operator psuh it t the stackChar
			stackChar.push(word.at(i));
		}
	}

	cin.get();
	cin.ignore();
	return 0;
}
