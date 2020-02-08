#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Job
{
	int id;
	int deadline;
	Job(int id, int deadline) : id(id), deadline(deadline){}

	//all the operations based on deadline, taking consideration of id number as well
    bool operator<(const Job & j){ 
		if(this->deadline < j.deadline){
			return true;
		}
		if(this->deadline == j.deadline && this->id > j.id){ //if deadlines same, id of rhs should be smaller than id of lhs(to give a job with smallest id of worker)
			return true;
		}
		return false;
	}
    bool operator>(const Job & j){
		if(this->deadline > j.deadline){
			return true;
		}
		if(this->deadline == j.deadline && this->id < j.id){
			return true;
		}
		return false;
	}
};

struct Worker
{
	int id;
	Job assigned;
	Worker(int id, Job given) : id(id), assigned(given){ //since each worker has a job, consturctor has Job
		assigned.deadline = given.deadline;
		assigned.id = given.id;
	}

	//all the operator overloads based on the worker's assigned job's deadline( "<", ">" are specified in Job struct)
	bool operator<(const Worker & w){
		if(this->assigned < w.assigned){
			return true;
		}
		return false;
	}
	bool operator>(const Worker & w){
		if(this->assigned > w.assigned){
			return true;
		}
		return false;
	}
	void operator-(const int & d){
		this->assigned.deadline = this->assigned.deadline - d;
	}
};


template <class Comparable>
class BinaryHeap{
public:
	BinaryHeap(vector<Comparable> & Input);
	bool isEmpty( ) const;
	void insert(Comparable & x, ofstream & outfile);
	void deleteMin( );
	void decreaseElements(int t);
	void assignFreeWorkers(vector<Job> & jobs, int & index, ofstream & outfile);
	void outResult(ofstream & outfile);
private:
	int currentSize; // Number of elements in heap
	vector<Comparable> array; // The heap array
	void buildHeap(vector<Comparable> & Input);
	void percolateDown( int hole );
	int passedDays;
};

//outputs the total #of days passed in a file
template <class Comparable>
void BinaryHeap<Comparable>::outResult(ofstream & outfile){
	outfile << "All jobs are completed in "<< passedDays << " days.";
}

//for every worker in the heap who's deadline=0(means completed) should be reassigned to a new job
template <class Comparable>
void BinaryHeap<Comparable>::assignFreeWorkers(vector<Job> & jobs, int & index, ofstream & outfile){
	while(currentSize != array.size() && jobs.size() != index && array[currentSize].assigned.deadline == 0){
		Worker x(array[currentSize].id, jobs[index]);
		insert(x, outfile);
		index++;
	}
}

//decrease each workers jobs by t
template <class Comparable>
void BinaryHeap<Comparable>::decreaseElements(int t){
	for(int i=1; i < array.size(); i++){
		array[i] - t;
	}
}

//check if heap is empty
template <class Comparable>
bool BinaryHeap<Comparable>::isEmpty() const{
	if(currentSize == 1){
		return true;
	}
	return false;
}

//only modified to get vector as a parameter
template <class Comparable>
BinaryHeap<Comparable>::BinaryHeap(vector<Comparable> & Input){
	array = Input;
	currentSize = array.size();
	buildHeap(array);
	passedDays = 0;
}

//same as in lecture slides, except outputs the result of assignment of the worker in a file
template <class Comparable>
void BinaryHeap<Comparable>::insert(Comparable & x, ofstream & outfile){
	// Note that instead of swapping we move the hole up
	int hole = currentSize;
	for ( ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 ){
		array[ hole ] = array[ hole / 2 ];
	}
	array[ hole ] = x;
	outfile << "Job-"<< array[hole].assigned.id <<"->Worker-"<< array[hole].id <<" (Day: "<< passedDays <<")\n";
	currentSize++;
}

//almost same as in lecture slides, except for each deletemin passeddays are calculated
template <class Comparable>
void BinaryHeap<Comparable>::deleteMin( ){
	// move the last element to the first and shrink the array
	int minDeadline = array[1].assigned.deadline;
	decreaseElements(minDeadline);
	while(currentSize > 1 && array[1].assigned.deadline == 0){
		Worker temp(array[1].id, array[1].assigned);
		array[1] = array[--currentSize];
		array[currentSize] = temp;
		if(currentSize > 1){
			percolateDown(1);
		}
	}
	passedDays = passedDays + minDeadline;
}

//same as in lecture slides
template <class Comparable>
void BinaryHeap<Comparable>::percolateDown( int hole ){
	if(currentSize == 1){
		return;
	}
	int child;
	Comparable tmp = array[ hole ]; // tmp is the item that will
									// be percolated down
	for( ; hole * 2 <= currentSize; hole = child ){
		child = hole * 2;
		if ( child != currentSize && child+1 != currentSize && array[child + 1] < array[child] ){
			child++;
		}
		if ( child < currentSize && array[ child ] < tmp ){
			array[ hole ] = array[ child ];
		}
		else{
			break;
		}
	}
	array[ hole ] = tmp;
}

//same as in lecture slides
template <class Comparable>
void BinaryHeap<Comparable>::buildHeap(vector<Comparable> & Input){
	array = Input; // copy input array to private array;
	for (int i = (currentSize / 2)-1; 0<i; i--){
		percolateDown( i );
	}
}

int main(){
	string fileName;
	cout << "Enter a file name(including \".txt\"):\n";
	cin >> fileName;

	ifstream file;
	file.open(fileName);
	if(file.fail()){
		cout << "Invalid file name.\n";
		cin.get();
		cin.ignore();
		return 0;
	}

	//get the first line as number of workers
	string lines;
	getline(file,lines);
	int workerSize = atoi(lines.c_str());
	//create a vector of workers, 0th index is an empty one with negative values(heap should start from 1st index for math operations)
	vector<Worker> workers;
	Worker empty(-1, Job(-1,-1));
	workers.push_back(empty);

	//get the second line as number of jobs and create a vector for it
	getline(file,lines);
	int jobSize = atoi(lines.c_str());
	vector<Job> jobs;

	//get each jobs id and deadline, push them to the vector
	while(getline(file,lines)){
		istringstream input(lines); 
		int jobId;
		int deadline;
		input >> jobId;
		input >> deadline;
		Job x(jobId, deadline);
		jobs.push_back(x);
	}

	//open out file
	fileName = fileName + ".out";
	ofstream outfile (fileName);

	//consider no worker & no jobs
	if(workerSize == 0){
		outfile << "The jobs will not be completed since 0 workers are present.";
		return 0;
	}
	if(jobSize == 0){
		outfile << "There are no jobs to be completed.(0 days has passed)";
		return 0;
	}


	//if #worker>#jobs then this means we have excessive #workers, exclude them for proper execution of program
	if(workerSize > jobSize){
		workerSize = jobSize;
	}

	//for the beginning assign each worker a job in order
	for(int i=1; i<=workerSize; i++){
		Worker x(i, jobs[i-1]);
		workers.push_back(x);
		outfile << "Job-"<< i <<"->Worker-"<< i <<" (Day: 0)\n";
	}

	//put workers in a heap and delete the min of them untill all jobs are done
	int index = workerSize; //since we have already given the first workerSize many jobs already
	BinaryHeap<Worker> workerHeap(workers);
	while(!workerHeap.isEmpty()){
		workerHeap.deleteMin();
		if(jobSize > index){
			workerHeap.assignFreeWorkers(jobs, index, outfile);
		}
	}
	workerHeap.outResult(outfile);
	outfile.close();
	cout << "Your file is ready.\n";

	system("pause");
	return 0;
}