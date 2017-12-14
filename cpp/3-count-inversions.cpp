#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

inline void swap(int* arr, int i, int j);
inline bool compare(int x, int y, bool reverse);
int merge(int* arr, int* sorted, int i, int mid, int j, long* inverts, bool reverse);
void inversions(int* arr, int* sorted, int i, int j, long* inverts, bool reverse);

int merge(int* arr, int* sorted, int i, int mid, int j, long* inverts, bool reverse){
    int end = j;
    int cur = j;
    int ist = mid;
    while(j>mid && ist>=i){
        if (!compare(arr[ist], arr[j], !reverse)){
            sorted[cur--] = arr[ist--];
        }else{
            sorted[cur--] = arr[j--];
            *inverts += mid - ist;
        }
    }
    while(j>mid){
        *inverts += mid + 1 - i;
        sorted[cur--] = arr[j--];
    }
    while(ist>=i){
        sorted[cur--] = arr[ist--];
    }

    // copying merged segment into original
    int ii = i-1;
    while (ii++<end){
        arr[ii] = sorted[ii];
    }

}

inline void swap(int* arr, int i, int j){
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

inline bool compare(int x, int y, bool reverse){
    if (x==y)
        return true;
    if ((x < y) ^ reverse)
        return false;
    return true;
}

void inversions(int* arr, int* sorted, int i, int j, long* inverts, bool reverse){
    if (i+1 >= j){

        if (i == j){
            sorted[i] = arr[i];
            return;
        }
        if (compare(arr[i], arr[j], reverse) && arr[i] != arr[j]){
            swap(arr, i, j);
            (*inverts) += 1;
        }
        sorted[i] = arr[i];
        sorted[j] = arr[j];
        return;
    }
    int mid = (j+i)/2;
    inversions(arr, sorted, i, mid, inverts, reverse);
    inversions(arr, sorted, mid+1, j, inverts, reverse);
    merge(arr, sorted, i, mid, j, inverts, reverse);
}

int bruteForce(int* arr, int n){
    int inverts = 0;
    for (int i=0; i<n; ++i){
        for (int j=i; j<n; ++j){
            if (arr[i]>arr[j]){
                inverts++;
            }
        }
    }
    return inverts;
}

int* test_file(){
    int n = 100000;
    int* arr = new int[n];

    ifstream file("/Users/rbcorx/Projects/algos/cpp/IntegerArray.txt");
    int t = 0;
    string line;
    if (file.is_open())
    {   while ( getline (file, line) )
        {

            arr[t] = stoi(line);
            t++;
        }
        file.close();
    }
    cout << "returning val!!!";
    return arr;
}

int main(){

    long inverts = 0;
    // int arr[] = {1 ,2 ,3 ,4, 9, 37, 1, 3};
    // int arr[] = {1, 20, 6, 4, 5};
    // int n = 5;
    // int n = 10;
    // int arr[] = {1, 4, 10, 1, 4, 10, 1, 4, 10, 5};

    int n = 100000;
    int* arr = test_file();


    int* sorted = new int[n];
    //int correct = bruteForce(arr, n);
    inversions(arr, sorted, 0, n-1, &inverts, false);
    int correct = 0;
    cout << "inverts: " << inverts << ", correct:" << correct << "\n";

    // debug. outputing sorted array
    // int i = 0;
    // while (i<n-1){
    //     cout << sorted[i++] << ", ";
    // }
    // cout << sorted[i];
    return 0;
}