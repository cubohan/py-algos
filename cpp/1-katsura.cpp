#include <iostream>
#include <cmath>

using namespace std;

int base(int n);
int steps = 0;
int katsura(int x, int n1, int y, int n2){
    /*
        x * y = 10^n * ac + 10^n/2 * ((a + b) * (c + d) - ac - bd) + bd
    */

    steps++;
    if (n1 <= 1 && n2 <= 1){
        return x * y;
    }
    int xmid = n1/2;
    int xpow = (int) (pow(10.0, xmid));

    int a = x / xpow;
    int b = x % xpow;

    int ymid = n2/2;
    int ypow = (int) (pow(10.0, ymid));
    int c = y / ypow;
    int d = y % ypow;

    int ac = katsura(a, n1 - xmid, c, n2 - ymid);
    int bd = katsura(b, xmid, d, ymid);
    if (n1 == n2){

        int mx = xmid>n1-xmid?xmid:n1-xmid;
        int mxpow = (int) (pow(10.0, mx));
        int xmx = (a + b) / mxpow == 0?mx:mx + 1;
        int ymx = (c + d) / mxpow == 0?mx:mx + 1;
        return (xpow * xpow) * ac + xpow * (katsura(a + b, xmx, c + d, ymx) - ac - bd) + bd;
    }
    return (xpow * ypow) * ac + xpow * (katsura(a, n1 - xmid, d, ymid)) +  ypow * (katsura(b, xmid, c, n2 - ymid)) + bd;
}

int run_katsura(int x, int y){
    int n1 = base(x);
    int n2 = base(y);
    return katsura(x, n1, y, n2);
}

int base(int n){
    int b = 0;
    while (n > 0){
        n /= 10;
        b++;
    }
    return b;
}

int main(){

cout << run_katsura(1911, 1111) << endl;
cout << steps << endl;
steps = 0;
cout << run_katsura(19110, 11110) << endl;
cout << steps << endl;
cout << run_katsura(19110, 11110) << endl;
cout << steps << endl;
return 0;
}
