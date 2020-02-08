//Ege Demirtas 25401

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 
#include <chrono>
#include <iomanip>
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
	void storeMeaning(const Comparable & x, vector<string>& resultTree) const;
	bool checkMeaning( const Comparable & x, const Comparable & y) const;
	void findRepeated(int n, vector<string> & query);
private:
	BinaryNode<Comparable> *root;
	const Comparable ITEM_NOT_FOUND;
	const Comparable & wordAt( BinaryNode<Comparable> *t ) const;
	BinaryNode<Comparable> * find( const Comparable & x,BinaryNode<Comparable> *t ) const;
	void insert( const Comparable & x, BinaryNode<Comparable> * & t ) const;
	void makeEmpty( BinaryNode<Comparable> * & t ) const;
	void insertMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * & t ) const;
	void addMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * & t ) const;
	void storeMeaning(const Comparable & x, BinaryNode<Comparable> *t, vector<string>& resultTree) const;
	bool checkMeaning( const Comparable & x, const Comparable & y, BinaryNode<Comparable> * t ) const;
	void findRepeated(BinaryNode<Comparable> *t, int n, vector<string>&query);
	const string findMeaning( const Comparable & x, BinaryNode<Comparable> * t );
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
void BinarySearchTree<Comparable>::storeMeaning(const Comparable & x, vector<string>& resultTree) const{
	storeMeaning(x,root, resultTree);
}

//private printMeaning
template<class Comparable>
void BinarySearchTree<Comparable>::storeMeaning(const Comparable & x, BinaryNode<Comparable> *t, vector<string>& resultTree) const{
	BinaryNode<Comparable> *temp = find(x,t); //find the word in the tree and remember position with a pointer
	int index = 0;
	string fullTxt=x + "    ";/*
	if(temp->meaning.size()==0){ //if meaning vector size is 0 then there is nothing to print
		bst << "***Has No Meaning***";
		return;
	}*/
	while(index < temp->meaning.size()-1){ //meaning.size()-1 because the last element is spacial case where ", " shouldnt be printed after the word
		if(temp->meaning[index+1].find(',')==string::npos){ //if the next string in the vector is not ","; print word + single space
			//bst << temp->meaning[index] << " ";
			fullTxt = fullTxt + temp->meaning[index] + " ";
		}else{
			//bst <<  temp->meaning[index]; //if the next in the vector is ", " dont put any space since it will have wrong format such as " , "
			fullTxt = fullTxt + temp->meaning[index];
		}
		index++;
	}
	//bst << temp->meaning[index]; //print last meaning without any space at the end
	fullTxt = fullTxt + temp->meaning[index] + "\n";
	resultTree.push_back(fullTxt);
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


//ONLY THESE PARTS ARE ADDED AFTER HW2, ABOVE IS UNCHANGED
//public findRepeated, gets n and a vector and sends it to priveate findRepeated with the root of tree
template<class Comparable>
void BinarySearchTree<Comparable>::findRepeated(int n, vector<string>&query){
	findRepeated(root, n, query);
}

//private find repeated, given n, queired words and root of the tree, finds the meanings of the words in query for n times, displays time record
template<class Comparable>
void BinarySearchTree<Comparable>::findRepeated(BinaryNode<Comparable> *t, int n, vector<string>&query){
	auto start = std::chrono::high_resolution_clock::now();
	for(int j =1; j<=n; j++){
		for(int i=0; i<query.size();i++){
			const string x = findMeaning(query[i], t); //finds the meaning in another private function
		}
	}
	auto time = (std::chrono::high_resolution_clock::now() - start).count();
	cout <<left<<setw(16) << n  << time <<"\n";
}


//private findMeaing
//gets a word and the root, finds the word, then finds all the meanings, returns meanings as a string
template<class Comparable>
const string BinarySearchTree<Comparable>::findMeaning(const Comparable & x, BinaryNode<Comparable> * t ){
	string meanings;
	BinaryNode<Comparable> *temp = find(x,t); //find the word in the tree and remember position with a pointer
	for(int i =0; i<temp->meaning.size();i++){
			meanings = temp->meaning[i] + " " ;
	}
	return meanings;
}
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

//the 2D vector for chain mechanism will use this struct to store the word as a string and it's meanings in a vector
template <class HashedObj>
struct Node {
	HashedObj word;
	vector<HashedObj> meaning;
	Node(HashedObj x):word(x){
		vector<HashedObj> meaning;
	}
	Node():word(""){
		vector<HashedObj> meaning;
	}
};


//all functions' explanations are written seperately in the following lines
template <class HashedObj>
class HashTable{
public:
	HashTable( const HashedObj & notFound, int size);
	void insert( const HashedObj & x );
	int hashing(const HashedObj & key, int tableSize);
	void insertMeaning(const HashedObj & x, const HashedObj & y);
	void storeMeaning(const HashedObj & x, vector<string> & resultTable);
	void repeatedFind(int & n, vector<string> & query);
	void status();
private:
	vector<vector<Node<HashedObj>>> theLists; //2D vector for chain mechanism
	const HashedObj ITEM_NOT_FOUND;
	int nextPrime( int n ){ //from lecture slides
		if ( n % 2 == 0 )
			n++;
		for ( ; ! isPrime( n ); n += 2 );
		return n;
	}
	bool isPrime( int n ){ //from lecture slides
		if ( n == 2 || n == 3 )
			return true;
		if ( n == 1 || n % 2 == 0 )
			return false;
		for ( int i = 3; i * i <= n; i += 2 )
			if ( n % i == 0 )
				return false;
		return true;
	}
	void rehash();
	const string findMeaning( const HashedObj & x );
	double itemCount;
};

//constructor, takes a string(will be shown if searched query is not found) and size(for the size of the vector)
template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size ): ITEM_NOT_FOUND( notFound ), theLists( nextPrime( size )){
	itemCount = 0.0;
}

//gets a word, hashes it, finds index, stores the wrd in that index
template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x ){ 
	int index = hashing( x, theLists.size( ) );
	for(int i=0; i<theLists[index].size(); i++){//end the function if the word already exists, in the hw there are 2 "train"
		if(theLists[index][i].word == x){
			return;
		}
	}
	theLists[index].push_back(Node<HashedObj>(x)); //even if there is a collusion the word is put at the end of the given index's vector
	itemCount++; //increment itemcount
	double ratio = itemCount/theLists.size();
	if(ratio >= 0.5){ //if the table is half of full rehash it
		rehash();
	}
	
}

//gets word(x) finds its meanigns, returns the meanings
template <class HashedObj>
const string HashTable<HashedObj>::findMeaning( const HashedObj & x ){
	string meanings;
	int index = hashing( x, theLists.size() ); //find index of word
	for(int j=0; j<theLists[index].size();j++){ //iterate if there is a collusion
		Node<HashedObj> *temp = &theLists[index][j]; //I used a node pointer beacuse it involved confusing index numbers otherwise, but both works fine
		if(temp->word == x){
			for(int i=0; i<temp->meaning.size(); i++){ //store all the meanings
				meanings = meanings + " " + temp->meaning[i];
			}
			return meanings;
		}
	}
	return meanings;
}

//used the same hashing funtion in the slides, no so many collusions, not more than height of three which is log(n)
template <class HashedObj>
int HashTable<HashedObj>::hashing(const HashedObj & key, int tableSize){
	int hashVal = 0;
	for (int i = 0; i < key.length();i++)
		hashVal = 37 * hashVal + key[ i ];
	hashVal = hashVal % tableSize;
	if (hashVal < 0)
		hashVal = hashVal + tableSize;
	return hashVal;
}


template <class HashedObj>
void HashTable<HashedObj>::rehash(){
	int store = itemCount; //store item count, since theList will be erased
	itemCount =0;
	int current = theLists.size();
	int expand = nextPrime( 2 * theLists.size()); //expand as twice and find the closest prime to that
	vector<vector<Node<HashedObj>>> oldLists(theLists.size()); //create another 2D vector to store theList identically
	for(int i=0; i< theLists.size();i++){ //depp copy all the content in theList to oldList
		for(int j=0; j< theLists[i].size();j++){
			oldLists[i].push_back(Node<HashedObj>());
			oldLists[i][j].word = theLists[i][j].word;
			oldLists[i][j].meaning = theLists[i][j].meaning;
		}
	}
	//delete theList completely
	for(int i=0; i< theLists.size();i++){
		for(int j=0; j< theLists[i].size();j++){
			theLists[i].clear();
		}
	}
	theLists.clear();

	//resize theList as expand
	theLists.resize(expand);
	for(int i=0; i< oldLists.size();i++){ //iterate through each node in the oldList, to insert these to theList with the new table size, aka different hashing
		for(int j=0; j< oldLists[i].size();j++){
			itemCount =0; //we need this each iteration because insert(oldLists[i][j].word) inceremnts itemCount by 1 each time, but we stored itemCount already, no worries!!!
			insert(oldLists[i][j].word); //store the word using already defined function
			for(int k=0; k< oldLists[i][j].meaning.size();k++){ //store all the meanigns of that word using already written function
				insertMeaning(oldLists[i][j].word, oldLists[i][j].meaning[k]);
			}
		}
	}

	for(int i=0; i< oldLists.size();i++){
		for(int j=0; j< oldLists[i].size();j++){
			oldLists[i].clear();
		}
	}
	oldLists.clear();
	itemCount = store; //update the itemcount(it is still the same)
	cout <<"rehashed...\nprevious table size: " << current <<
		", new table size: " << expand <<
		", current unique word count " << itemCount << ",\n current load factor: " << itemCount/theLists.size() <<"\n";
}

//given word(x) store it's meaning(y)
template <class HashedObj>
void HashTable<HashedObj>::insertMeaning(const HashedObj & x, const HashedObj & y){
	int index = hashing( x, theLists.size() ); //find the index of word
	for(int i=0; i<theLists[index].size(); i++){ //itearte through that index if there is a collusion
		if(theLists[index][i].word == x){
			theLists[index][i].meaning.push_back(y); //find the word and insert meaning into Nodes's <meanings> vector
			break;
		}
	}
}

//given a word(from query), and a vector, finds the meaning of the word and stores it in the vector
template <class HashedObj>
void HashTable<HashedObj>::storeMeaning(const HashedObj & x, vector<string> & resultTable){
	string fullTxt = x + "    "; //int he file it will be seen as: word     meaning\n   (meanign will be inserted at the end of this function)
	int index = hashing(x, theLists.size()); //find the word's index with the hash function
	for(int i=0; i<theLists[index].size(); i++){ //there might be collusions so iterate through that index to find the word(x)
		if(theLists[index][i].word == x){
			int indexM = 0;
			while(indexM < (theLists[index][i].meaning.size()-1)){ //store all the meanings except last one
				fullTxt = fullTxt + theLists[index][i].meaning[indexM] + " ";
				indexM++;
			}
			fullTxt = fullTxt + theLists[index][i].meaning[indexM] +"\n"; //last meaning is stored in such way that "\n" helps to go to next line 
																			//when we are writing into ht_result from this vector
			resultTable.push_back(fullTxt); //store as "word     meanignA meaningB meaningC"
			break; //no need to iterate further as the word is found and meanigns are recorded into vector
		}
	}
}

//given n, finds the meanings of the words in query for n times, displays time record
template <class HashedObj>
void HashTable<HashedObj>::repeatedFind(int & n, vector<string> & query){
	auto startht = std::chrono::high_resolution_clock::now();
	for(int j =1; j<=n; j++){
		for(int i=0; i<query.size();i++){
			const string x = findMeaning(query[i]);
		}
	}
	auto timeht = (std::chrono::high_resolution_clock::now() - startht).count();
	cout <<left<<setw(16) << n  << timeht <<"\n";
}

template <class HashedObj>
void HashTable<HashedObj>::status(){
	cout << "After preprocessing, the unique word count is " << itemCount << ". Current load ratio is " << itemCount/theLists.size() <<"\n";
}

int main(){
	HashTable<string> table("NOT EXISTS", 53); //hash table, with initial size 53
	BinarySearchTree<string> tree("0"); //binary tree
	vector<string> query; //for queried words
	vector<string> resultTree; //store the results of finding words/meanings in hash table
	vector<string> resultTable; //store the results of finding words/meaings in binary tree
	int queryNum = 0;
	
	ifstream openQuery;
	openQuery.open("query1.txt"); //opens the queried txt file
	if(openQuery.fail()){//if fail to open txt finish the program
		cout << "Invalid file name.\n";
		cin.get();
		cin.ignore();
		return 0;
	}

	string lines;
	while(getline(openQuery,lines)){ //read the query txt line by line
		istringstream input(lines); 
		string queried;
		input >> queried; //store each word in query vector
		query.push_back(queried);
		queryNum++; //record how many queries are there to find average time for benchmarks
	}

	ifstream opentxt;
	opentxt.open("dict.txt"); //opens the txt file
	if(opentxt.fail()){//if fail to open txt finish the program
		cout << "Invalid file name.\n";
		cin.get();
		cin.ignore();
		return 0;
	}

	cout << "Building a binary tree for dict.txt...\nBuilding a hash table for dict.txt...\n";
	while(getline(opentxt,lines)){ //read the txt line by line
		istringstream input(lines); // put the line in an input
		if(lines.length()>0){
			string word; //store the eng word
			string meaning; //store meanings (tr)
			string coma; //store coma
			string total;
			bool paranthesis = false;
			input >> word;
			tree.insert(word); //directly insert eng as word to the tree
			table.insert(word);
			while(input >> meaning){
				if(meaning.find('(') != string::npos){ //if a paranthesis is found
					total = meaning;
					paranthesis = true;
				}else if(meaning.find(')') != string::npos){
					total = total + " " + meaning;
					tree.insertMeaning(word, total);
					table.insertMeaning(word, total);
				}else if(paranthesis){
					total = total + " " + meaning;
				}else if(meaning.find(',') != string::npos){ //if a coma is found
					coma = meaning.substr(meaning.find(','),1); //store the coma
					meaning = meaning.substr(0,meaning.length()-1); //store the meaning witouth coma
					tree.insertMeaning(word, meaning); //insert meaning with perspective word
					tree.insertMeaning(word, coma); //insert coma as meaning(it may not be a meaning but essential for correct output!!!)
					table.insertMeaning(word, meaning);
					table.insertMeaning(word, coma);
				}else{
					tree.insertMeaning(word, meaning); //if no coma directly insert the meaning with the perspective word
					table.insertMeaning(word, meaning);
				}
			}
		}
	}
	table.status();//print out the latest structure of the table, item count load factor

	//iterate through the query vector and store the queried word with its meaning in resultTree vector or resultTable
	cout << "Running queries in query1.txt...\n\n";
	auto start = std::chrono::high_resolution_clock::now();
	for(int i=0; i<queryNum; i++){
		tree.storeMeaning(query[i], resultTree);
	}
	auto time = (std::chrono::high_resolution_clock::now() - start).count();
	cout<<"***********************************************\n"
		<<"Benchmark results for Binary Search Tree (BST):\n***********************************************\n"
		<<"+ Elapsed time: " <<time << "ns\n+ Average query time: " << time/queryNum << " ns\n";

	auto startht = std::chrono::high_resolution_clock::now();
	for(int i=0; i<queryNum; i++){
		table.storeMeaning(query[i], resultTable);
	}
	auto timeht = (std::chrono::high_resolution_clock::now() - startht).count();
	cout<<"\n***********************************************\n"
		<<"Benchmark results for Hash Table:\n***********************************************\n"
		<<"+ Elapsed time: " <<timeht << "ns\n+ Average query time: " << timeht/queryNum << " ns\n+ Speed up: " << double(time)/double(timeht) <<"x\n";

	//find the meanings of the words in query j times: j = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096}
	cout << "\nTime measurements in ns (N, 4096N):\n*****************************\nbst\nN\t\ttime\n";
	for(int j =1; j<=4096; j= j*2){
		tree.findRepeated(j, query);
	}
	cout << "\nht\nN\t\ttime\n";
	for(int j =1; j<=4096; j= j*2){
		table.repeatedFind(j, query);
	}

	//these are for writing into file
	//write the words and their definitions into respective files, using the vectors stored previously
	ofstream bst;
	bst.open("bst_result.txt");
	for(int i=0; i<resultTree.size();i++){
		bst << resultTree[i];
	}
	bst.close();

	ofstream ht;
	ht.open("ht_result.txt");
	for(int i=0; i<resultTable.size();i++){
		ht << resultTable[i];
	}
	ht.close();

	system("pause");
	return 0;
}