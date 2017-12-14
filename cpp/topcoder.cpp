/* given N and K, print string which has atmost K exchanges between A and B
*/

#import <iostream>

using namespace std;

void swap(char * a, char * b){
    char temp = *a;
    *a = *b;
    *b = temp;
}

int main(){

    const int n = 10;
    int k = 10;
    char str[n];
    for (int i = 0; i<n; i++){
        if (i < n/2)
            str[i] = 'a';
        else
            str[i] = 'b';
    }
    while (k > 0){
        for (int i=0; i<n-1; i++){
            if (str[i] == 'a' && str[i+1] == 'b'){
                //cout << "\n" << str[i] << str[i+1];
                swap(str + i, str + i + 1);
                //cout << "\n" << str[i] << str[i+1] << "\n";
                k--;
                break;
            } else if (i == n-2){
                cout << "Not possible!";
                return 0;
            }

        }
    }

    for (int i = 0; i<n; i++){
        cout << str[i];
    }


    return 0;
}
