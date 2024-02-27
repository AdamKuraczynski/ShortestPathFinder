#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Maximum possible integer used in the algorithm for finding the shortest path
const int maxInt = 2147483647;

// Structure storing node numbers along with the lowest cost of reaching each of them
struct path {
	int number;
	int cost;
};

class Array {
private:
	int size;					// size of the array specified by the user
	int** array;				// two-dimensional array
	int numNodes;				// number of nodes in the array (squares)
	path* road;					// array storing node numbers with the lowest cost of reaching them
	vector<char> directions;	// vector storing movement instructions from the first to the last node
	int cost;					// Variable storing the cost of the path

public:
	void display();				// displays the content of the array
	void generate();			// generates random values for the array
	int findPath();				// finds the shortest possible path from the top left node to the bottom right one
	void writePath();			// determines the necessary movements to traverse the path in the shortest way
	void writeToTxt();			// writes the results to a txt file

	Array();					// Default constructor, creating a 3 x 3 array
	Array(int);					// Constructor with parameter allowing to create an array of any dimensions
	Array(const Array&);		// Copy constructor
	~Array();					// Destructor
};

Array::Array() {
	size = 3;	// By default, we create a 3x3 array

	array = new int* [size];

	for (int i = 0; i < size; ++i) {
		array[i] = new int[size];
	}
}

Array::Array(int n) {
	size = n;	// Creating an array with dimensions specified by the user

	array = new int* [size];

	for (int i = 0; i < size; ++i) {
		array[i] = new int[size];
	}
}

Array::Array(const Array& copy) { // Copy constructor
	size = copy.size;
	array = new int* [size];
	for (int i = 0; i < size; ++i) {
		array[i] = new int[size];
		for (int j = 0; j < size; ++j) {
			array[i][j] = copy.array[i][j];
		}
	}
}

Array::~Array() {  // Destructor
	for (int i = 0; i < size; ++i) {
		delete[] array[i];
	}
	delete[] array;
}

void Array::display() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}

void Array::generate() {
	// Fill the array with pseudo-random digits:
	srand(time(NULL));
	int num = 0;

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			array[i][j] = rand() % 10;
		}
	}
}

int Array::findPath() {
	numNodes = size * size;
	// Creating a dynamic array storing node numbers along with the LOWEST cost of reaching them
	road = new path[numNodes];

	// Assigning numbers and storing the largest possible cost (temporarily)
	for (int i = 0; i < numNodes; ++i) {
		road[i].number = i;
		road[i].cost = maxInt;
	}

	// Writing the cost of reaching the first node
	road[0].cost = array[0][0];

	int counter = 0;
	int sum1 = 0;
	int sum2 = 0;

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			// Last element of the array (bottom right)
			if (counter == numNodes - 1) {
				cout << "Reached the end, the result is: " << road[counter].cost << endl;
			}
			// Last row of the array (without elements below)
			else if (i == (size - 1)) {
				sum1 = road[counter].cost + array[i][j + 1];

				if (sum1 < road[counter + 1].cost)
					road[counter + 1].cost = sum1;
			}
			// Last column of the array (without elements to the right)
			else if (j == (size - 1)) {
				sum1 = road[counter].cost + array[i + 1][j];

				if (sum1 < road[counter + size].cost)
					road[counter + size].cost = sum1;
			}
			// Elements with neighbors on the side and below
			else {
				sum1 = road[counter].cost + array[i][j + 1];
				sum2 = road[counter].cost + array[i + 1][j];

				if (sum1 < road[counter + 1].cost) {
					road[counter + 1].cost = sum1;
				}

				if (sum2 < road[counter + size].cost) {
					road[counter + size].cost = sum2;
				}
			}

			++counter;
		}
	}
	cost = road[numNodes - 1].cost;
	return road[numNodes - 1].cost;
}

void Array::writePath() {
	int index = numNodes - 1;

	for (int i = 0; i < (2 * size - 2); ++i) {
		// There is no row above us
		if (index < size) {
			index--;
			directions.insert(directions.begin(), 'd');
		}
		// There is no column on the left side
		else if (index == i && i % size == 1) {
			index -= size;
			directions.insert(directions.begin(), 'r');
		}
		// Starting
		else {
			if (road[index - 1].cost < road[index - size].cost) {
				index--;
				directions.insert(directions.begin(), 'd');
			}
			else {
				index -= size;
				directions.insert(directions.begin(), 'r');
			}
		}
	}

	for (int i = 0; i < directions.size(); ++i) {
		cout << directions[i] << " ";
	}

	cout << endl;
}

void Array::writeToTxt() {

	ofstream WRITE("data.txt");
	WRITE << "Array: " << endl;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			WRITE << array[i][j] << " ";
		}
		WRITE << endl;
	}
	WRITE << "Path cost is: " << endl;
	// line to hide cout
	streambuf* old = cout.rdbuf(0);
	WRITE << cost;
	// line to restore cout to its initial state
	cout.rdbuf(old);
	WRITE << endl << "Path taken by the algorithm: " << endl;
	for (int i = 0; i < directions.size(); ++i) {
		WRITE << directions[i] << " ";
	}
}

int main() {
	Array array(5);

	array.generate();
	array.display();
	array.findPath();

	array.writePath();

	array.writeToTxt();

	return 0;
}