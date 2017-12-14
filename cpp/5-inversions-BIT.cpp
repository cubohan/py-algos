#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class BIT{
    public:
        int* arr;
        int lim;
        BIT(int n);
        BIT(int* in, int n);
        void insert(int x);
        int sum(int i, int j);
        void update(int ind, int val);
        int freq(int i);
};

BIT::BIT(int n){
    arr = new int[n+1];
    lim = 0;
}

BIT::BIT(int* in, int n){
    arr = new int[n+1];
    lim = n;
    for (int i=0; i<=n; i++){
        arr[i] = 0;
    }
    for (int i=0; i<n; i++){
        update(i+1, in[i]);
    }
}

void BIT::insert(int x){
    arr[++lim] = x;

}

void BIT::update(int ind, int val){
    while (ind<=lim){
        arr[ind] += val;
        ind += (ind & (-ind));
    }
}

int BIT::sum(int i, int j){
    return freq(j) - freq(i);
}

int BIT::freq(int i){
    int res = 0;
    while(i){
        res += arr[i];
        i -= (i & (-i));
    }
    return res;
}

void _swap(int arr[], int i, int j){
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

void swap(int arr[], int arr2[], int i, int j){
    _swap(arr, i, j);
    _swap(arr2, i, j);
}

bool comp(int arr[], int i, int j){
    return arr[i] <= arr[j];
}

void sort_special(int arr[], int arri[], int i, int j){

    if (i>=j){
        return;
    }

    int pvt = i;
    int cur = pvt + 1;
    int r = j;
    while(cur < r){
        if (comp(arr, pvt, cur)){
            swap(arr, arri, cur, r--);
        }else{
            cur++;
        }
    }

    swap(arr, arri, pvt, comp(arr, pvt, cur)?(--cur):cur);
    cur = cur<=i?(i+1):cur;
    sort_special(arr, arri, i, cur-1);
    sort_special(arr, arri, cur, j);

}

int convertArr(int arr[], const int n){
    int copy[n];
    int indexes[n];

    for (int i=0; i<n; i++){
        copy[i] = arr[i];
        indexes[i] = i;
    }
    sort_special(copy, indexes, 0, n-1);
    int rank = 1;
    for(int i=0; i<n; i++){
        if (i>0){
            if (copy[i] != copy[i-1])
                rank++;
        }
        arr[indexes[i]] = rank;
    }
    return rank;
}

long inversions(int arr[], int n, const int mx){
    int freqs[mx];
    for(int i=0; i<mx; i++){
        freqs[i] = 0;
    }
    for(int i=0; i<n; i++){
        freqs[arr[i]-1]++;
    }
    BIT b(freqs, mx);
    long res = 0;
    for(int i=0; i<n; i++){
        res += b.freq(arr[i]-1);
        b.update(arr[i], -1);
    }

    return res;
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
    // int n = 10;
    // int arr[] = {1, 4, 10, 1, 4, 10, 1, 4, 10, 5};
    // int n = 5;
    //int arr[] = {1, 20, 6, 4, 5};

    long inverts = 0;

    int n = 100000;
    int* arr = test_file();

    int correct = 0; //bruteForce(arr, n);
    int rank = convertArr(arr, n);
    inverts = inversions(arr, n, rank);

    cout << endl << "Inversions: " << inverts << ", correct: " << correct << endl;

    // testing.
    // BIT b(arr, n);
    // int sum = 0;
    // for(int i=1; i<=n; i++){
    //     sum += arr[i-1];
    //     cout << endl << b.arr[i] << endl;
    // }

    // cout << b.freq(10) << endl;
    // cout << sum << endl;
    return 0;
}
