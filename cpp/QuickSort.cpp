/*
 *  QuickSort algorithm implementation
 *  author: rbcorx
 *  circa 2018  
 */

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

bool verbose = false;
char pathname [] = "/media/rbcorx/My Book/@Ronnie/project Cubohan/algo/@code/QuickSort.txt";
const int numLength = 10000;
int comparisons [] = {0, 0, 0};
int pivotingScheme = 0;

inline void swap(int* arr, int i, int j){
	// change to int pointers instead of array indexes
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

inline int choosePivot(int* arr, int i, int j){
	// chooses pivot and swaps place with first element in the array
	int pivot;

	switch (pivotingScheme){
		case 0:
			// choose first element as pivot
			pivot = arr[i];
			break;
		case 1:
			pivot = arr[j];
			swap(arr, i, j);
			break;
		case 2:
			int midIndex = (j+i)/2;
			int fringeElems [] = {arr[i], arr[midIndex], arr[j]};
			sort(fringeElems, fringeElems+3);
			int midElem = fringeElems[1];
			if (arr[i] == midElem){
				pivot = arr[i];
			}
			else if (arr[midIndex] == midElem){
				pivot = arr[midIndex];
				swap(arr, i, midIndex);
			}
			else if (arr[j] == midElem){
				pivot = arr[j];
				swap(arr, i, j);
			}
			break;
	}
	return pivot;
}

void quicksort(int* arr, int i, int j){
	if (i >= j){
		return;
	}
	
	// roughly estimating comparisons on current recursive call
	comparisons[pivotingScheme] += j-i;
	
	int pivot = choosePivot(arr, i, j);
	
	int lowerBound = i;
	for (int ii=i+1; ii<=j; ++ii){
		if (arr[ii] < pivot) {
			swap(arr, ++lowerBound, ii);
		}
	}
	swap(arr, lowerBound, i);
	quicksort(arr, i, lowerBound-1);
	quicksort(arr, lowerBound+1, j);
}

int main (){
	ifstream inFile;

	inFile.open(pathname);
	if (!inFile){
		cout << "file unavailable! \n";
	}else{
		int num[numLength], num2[numLength], num3[numLength];
		int index = 0;
		while (inFile >> num[index++]){
			// making two more copies for questions
			num2[index-1] = num[index-1];
			num3[index-1] = num[index-1];
			if (index < 10 && verbose)
				cout << num[index-1] << endl;
		}

		int* testArr[] = {num, num2, num3};

		pivotingScheme = 0;
		quicksort(num, 0, numLength-1);
		cout << comparisons[0] << endl;

		pivotingScheme = 1;
		quicksort(num2, 0, numLength-1);
		cout << comparisons[1] << endl;

		pivotingScheme = 2;
		quicksort(num3, 0, numLength-1);
		cout << comparisons[2] << endl;
		

		for (int** test = testArr; test<testArr + 3; test++){
			for (int i=1; i<numLength; i++){
				if(!((*test)[i-1] <= (*test)[i])){
					cout << "failed!!!!!";
				}
			}
		}
		for (int* compares = comparisons; compares<comparisons + 3; compares++){
			cout << " COMPARES :: " << *compares << endl;
		}
		// cout << num;

	}

	return 0;
}
