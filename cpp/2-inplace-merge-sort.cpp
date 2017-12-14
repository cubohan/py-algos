#import <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

void swap(int* arr, int i, int j){
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

void isort(int* arr, int st, int en, bool rev){
    for (int i=st; i<en; i++){
        if ((arr[i] < arr[i+1]) ^ rev)
            break;
        else
            swap(arr, i, i+1);
    }
}

void mwwa(int* arr, int i, int n, int j, int m, int w, bool rev){
    while (i <= n && j<= m){
        swap(arr, w++, (arr[i] < arr[j]) ^ (!rev) ? j++ : i++);
    }
    while (i <= n){
        swap(arr, w++, i++);
    }
    while (j <= m){
        swap(arr, w++, j++);
    }
}

void msort(int* arr, int st, int en, bool rev){
    if (st + 1 >= en){
        if (st != en){
            if ((arr[st] < arr[en]) ^ (!rev))
                swap(arr, st, en);
        }
        return;
    }

    int n = (en + st)/2;
    if ((n + 1 - st)%2 != 0){
        n--;
    }
    int wst = (n + st)/2;
    msort(arr, n+1, en, rev);
    bool drop = false;

    while (wst+1-st-(drop?1:0) >= 1){
        msort(arr, st + (drop?1:0), wst, rev);
        mwwa(arr, st + (drop?1:0), wst, n+1, en, wst+1, rev);
        n = wst;
        wst = (wst + st) / 2;
        if ((n + 1 - st)%2 != 0){
            drop = true;
        }else{
            drop = false;
        }
    }
    if (drop){
        isort(arr, st, en, rev);
    }
}

void msort(int* arr, int st, int en){
    msort(arr, st, en, false);
}

int main(){
    srand (time(NULL));

    bool rev = false;
    int n = 1200000;
    int* arr = new int[n];

    for (int i=0; i<n; ++i){
        arr[i] = rand();
    }

    bool checked = true;

    msort(arr, 0, n, rev);

    for (int i=0; i<n; ++i)
        if (!((arr[i]<arr[i+1]) ^ rev) && arr[i]!=arr[i+1])
            checked = false;

    if (!checked)
        cout << endl << "failed!!!" << endl;
    else
        cout << endl << "perfect!!!" << endl;

    // cout << "count isorts: " << isorts << ", step-count: " << isteps << endl;

    return 0;
}
