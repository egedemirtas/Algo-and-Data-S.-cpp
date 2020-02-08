//egedemirtas 25401

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream> 
#include <string>
#include <fcntl.h> //to be able to use _O_U16TEXT
#include <io.h> //for _setmode
#include <codecvt> // for codecvt_utf8 and locale

using namespace std;
template <class Comparable>
class BinarySearchTree;

//binary node is just like in the slides but modified to keep the meanings of words
template <class Comparable>
class BinaryNode{
Comparable word;
vector<Comparable> meaning; //meanings are stored in a vector
BinaryNode *left;
BinaryNode *right;

BinaryNode(const Comparable & wrd, BinaryNode *l, BinaryNode *r ) : word(wrd), left(l), right(r) { //initialize empty vector in a node
	vector<Comparable> meaning;
}

void addMeaning(const Comparable & mnn){ //adds elements to the meanings vector
	meaning.push_back(mnn);
}

friend class BinarySearchTree<Comparable>;
};

//similar to the slides but modified to insert, print, check meanings
template <class Comparable>
class BinarySearchTree{
public:
	explicit BinarySearchTree( const Comparable & notFound );
	~BinarySearchTree( );
	const Comparable & find( const Comparable & x ) const;
	void insert( const Comparable & x);
	void makeEmpty( );
	void insertMeaning( const Comparable & x, const Comparable & y);
	void addMeaning( const Comparable & x, const Comparable & y);
	void printMeaning(const Comparable & x) const;
	bool checkMeaning( const Comparable & x, const Comparable & y) const;
private:
	BinaryNode<Comparable> *root;
	const Comparable ITEM_NOT_FOUND;
	const Comparable & wordAt( BinaryNode<Comparable> *t ) const;
	BinaryNode<Comparable> * find( const Comparable & x,BinaryNode<Comparable> *t ) const;
	void insert( const Comparable & x, BinaryNode<Comparable> * & t ) const;
	void makeEmpty( BinaryNode<Comparable> * & t ) const;
	void insertMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * & t ) const;
	void addMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * & t ) const;
	void printMeaning(const Comparable & x, BinaryNode<Comparable> *t) const;
	bool checkMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * t ) const;
};

 //constructor; initialize root pointing NULL, no modifications
template<class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable &notFound):ITEM_NOT_FOUND(notFound),root(NULL){}

//deconstructor, no modifications, calls makeEmpty
template<class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree(){
	makeEmpty();
}

//returns the word at t(pointer), no modifications
template<class Comparable>
const Comparable & BinarySearchTree<Comparable>::wordAt( BinaryNode<Comparable> *t ) const{
	return t == NULL ? ITEM_NOT_FOUND : t->word;
}

//public find no modifications, used it to find word, not meaning!
template<class Comparable> 
const Comparable & BinarySearchTree<Comparable>:: find( const Comparable & x ) const{
	return wordAt(find(x,root));
}

//private find no modifications
template<class Comparable> 
BinaryNode<Comparable> * BinarySearchTree<Comparable>:: find( const Comparable & x,BinaryNode<Comparable> *t ) const{
	if(t == NULL){
		return NULL;
	}else if(x < t->word){
		return find(x,t->left);
	}else if(x > t->word){
		return find(x,t->right);
	}else{
		return t;
	}
}

//public insert, no modifications
template<class Comparable>
void BinarySearchTree<Comparable>:: insert( const Comparable & x){
	insert(x, root);
}

//private insert, no modifications
template<class Comparable>
void BinarySearchTree<Comparable>:: insert( const Comparable & x, BinaryNode<Comparable> * & t ) const{
	if(t == NULL){
		t = new BinaryNode<Comparable>(x, NULL, NULL);
	}else if(x < t->word){
		insert(x, t->left);
	}else if(x > t->word){
		insert(x, t->right);
	}
}

//public makeEmpty, no modifications
template<class Comparable>
void BinarySearchTree<Comparable>:: makeEmpty(){
	makeEmpty(root);
}

//private makeEmpty no modifications
template<class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinaryNode<Comparable> * & t ) const{
	if(t != NULL){
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t = NULL;
}

template<class Comparable> //public insertM
void BinarySearchTree<Comparable>:: insertMeaning( const Comparable & x, const Comparable & y){
	insertMeaning(x, y, root);
}

template<class Comparable> //private insertM
void BinarySearchTree<Comparable>:: insertMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * & t ) const{
	BinaryNode<Comparable> *temp = find(x,t);
	temp->addMeaning(y);
}

 //public addMeaning, this is special for command==2, calls private addMeaning
template<class Comparable> //public addMeanign
void BinarySearchTree<Comparable>:: addMeaning( const Comparable & x, const Comparable & y){
	addMeaning(x, y, root);
}

//public addMeaning, different than insertMeanign!!!: adds "," before adding the meaning
template<class Comparable>
void BinarySearchTree<Comparable>:: addMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * & t ) const{
	BinaryNode<Comparable> *temp = find(x,t);
	if(temp->meaning.size()>=1){ // add "," if there is only one meaning(sinc a new one will be entered after that)
		temp->addMeaning(L",");
	}
	temp->addMeaning(y);
}

//public printMeanning, calls private printMeaning, sending word and the root of the tree
template<class Comparable>
void BinarySearchTree<Comparable>::printMeaning(const Comparable & x) const{
	printMeaning(x,root);
}

//private printMeaning
template<class Comparable>
void BinarySearchTree<Comparable>::printMeaning(const Comparable & x, BinaryNode<Comparable> *t) const{
	BinaryNode<Comparable> *temp = find(x,t); //find the word in the tree and remember position with a pointer
	int index = 0;
	if(temp->meaning.size()==0){ //if meaning vector size is 0 then there is nothing to print
		wcout << "***Has No Meaning***";
		return;
	}
	while(index < temp->meaning.size()-1){ //meaning.size()-1 because the last element is spacial case where ", " shouldnt be printed after the word
		if(temp->meaning[index+1].find(',')==string::npos){ //if the next string in the vector is not ","; print word + single space
			wcout << temp->meaning[index] << " ";
		}else{
			wcout << temp->meaning[index]; //if the next in the vector is ", " dont put any space since it will have wrong format such as " , "
		}
		index++;
	}
	wcout << temp->meaning[index]; //print last meaning without any space at the end
}

//public checkMeaning, takes a word and a meaning and sends to private checkMeaning
template<class Comparable> 
bool BinarySearchTree<Comparable>::checkMeaning(const Comparable & x, const Comparable & y) const{
	return checkMeaning(x, y,root);
}

//private checkMeaning, returns true if y is not in the meanings vector
template<class Comparable>
bool BinarySearchTree<Comparable>::checkMeaning(const Comparable & x, const Comparable & y,BinaryNode<Comparable> * t ) const{
	int index=0;
	BinaryNode<Comparable> *temp = find(x,t); //find the word in the tree and remember position with a pointer
	while(temp->meaning.size()>index){ //look up if y is already in meaning vector of the word
		if(temp->meaning[0]==y){
			return false;
		}
		index++;
	}
	return true;
}

int main(){
	//****************************WARNING************************************************************************************
	//I spent ~4hrs to research how to printout turkish characters 
	//and most importantly how to store turkish character conatining lines into a string
	//this may/may not work on different devices/operating systems as I read from stackoverflow
	//for example linux users can get and print turkish characters without additional codes
	//also to store my unicode characters I used wstring
	//since i used wstring and used unicode characters, I needed to use "L" before strings; when i needed to make bool comparisons or store them or use them for a function
	//also used wcin and wcout to print wstrings, just incase
	//the next following lines are selected from different websites(since I personally didnt know how to do it, and Gulsen Demiroz approved)
	std::locale::global(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>)); //even though the next line is for printing turkish characters, 
																							//without this line, it can not convert non english characters, instead prints out weird symbols
    _setmode(_fileno(stdout), _O_U16TEXT); // to print out turkish characters
    _setmode(_fileno(stdin), _O_U16TEXT); //for cin to store turkish characters inside wstring
	//******************************************************************************************************************

	BinarySearchTree<wstring> tree(L"0"); //L for wstring and unicode
	wifstream opentxt; //since i used wstring i also used wifstream, wistringstream
	opentxt.open("dictUpdated.txt"); //opens the txt file, I opened dictUpdated.txt since I can process turkish alphabets,
	if(opentxt.fail()){//if fail to open txt finish the program
		wcout << "Invalid file name.\n";
		wcin.get();
		wcin.ignore();
		return 0;
	}
	
	wstring lines;
	while(getline(opentxt,lines)){ //read the txt line by line
		wistringstream input(lines); // put the line in an input
		if(lines.length()>0){
			wstring word; //store the eng word
			wstring meaning; //store meanings (tr)
			wstring coma; //store coma
			input >> word;
			tree.insert(word); //directly insert eng as word to the tree
			while(input >> meaning){
				if(meaning.find(',') != string::npos){ //if a coma is found
					coma = meaning.substr(meaning.find(','),1); //store the coma
					meaning = meaning.substr(0,meaning.length()-1); //store the meaning witouth coma
					tree.insertMeaning(word, meaning); //insert meaning with perspective word
					tree.insertMeaning(word, coma); //insert coma as meaning(it may not be a meaning but essential for correct output!!!)
				}else{
					tree.insertMeaning(word, meaning); //if no coma directly insert the meaning with the perspective word
				}
			}
		}
	}
	
	//get command from the user
	wcout << "***Help***\nQuery: 1 \nAdd new translation: 2\nAdd new word: 3\nExit: 0\nEnter Command: ";
	wstring command;
	wcin >> command;


	while(command != L"0"){
		wstring search; //to store what user is looking for
		wstring trans; //to store what user would like to enter as translation
		while(command.length()>=2 && command.find('3')==string::npos && command.find('2')==string::npos 
			&& command.find('1')==string::npos && command.find('0')==string::npos){ //insist on getting command until it is exactly 1 or 2 or 3
			wcout << "Enter Command: ";
			wcin.clear();
			wcin.ignore(20,'\n');
			wcin >> command;
		}
		//searches for a word in the tree
		if(command == L"1"){
			wcout << "Enter queried word: ";
			wcin >> search;
			if(tree.find(search) == L"0"){ //I created "BinarySearchTree<wstring> tree(L"0"); " where if the searched word is not found in the tree
										   //it will return ITEM_NOT_FOUND, which I declared it to be 0
				wcout << "The word does not exist, try again\n";
			}else{ //if the word is found print it with right format
				wcout << search << " ---> ";
				tree.printMeaning(tree.find(search)); //find the searched word in tree and print it's meanings
				wcout << "\n";
			}
		}else if(command == L"2"){ //add a NEW meaning for an existing word
			wcout << "Enter queried word: ";
			wcin >> search;
			if(tree.find(search) == L"0"){ //dont try to add meanings to an unexsting word
				wcout << "The word does not exist.\n";
			}else{
				wcout << "Enter the new translation: ";
				wcin >> trans;
				if(tree.checkMeaning(search, trans)){ //dont add the same meaning to the word if it is defined already
					tree.addMeaning(search,trans);
					wcout << "***UPDATED***\n";
				}else{
					wcout << "Translation already exists\n";
				}
			}
		}else if(command == L"3"){ //add a NEW word and a translation
			wcout << "Enter New Word: ";
			wcin >> search;
			if(tree.find(search) != L"0"){ //if the searched word exists give warning
				wcout <<"Word already exists, if you want to add new translation please use command 2.\n";
			}else{
				wcout << "Enter Translations: ";
				wcin >> trans;
				tree.insert(search); //first insert the word
				tree.insertMeaning(search, trans); //then insert the meaning
			}
		}
		wcout << "Enter Command: ";
		wcin.clear();
		wcin.ignore(20,'\n');
		wcin>>command;
	}
	wcout <<"Bye...\n";
	
	system("pause");
	return 0;
}