include <bits/stdc++.h>

using namespace std;

int hackerlandRadioTransmitters(vector <int> x, int k) {
    // Complete this function
    int anchor = 0;
    int ants = 0;
    bool inbtw = false;
    for (int i=1; i<x.size; i++){
    	if (x[i] - x[anchor] > k){
    		anchor = i;
    		if (!inbtw){ 
    			ants++;
	    		anchor--;

    		}
    		inbtw = !inbtw;
    	}
    }
    if (!inbtw){
    	ants++;
    }

    return ants;
}

int main() {
    int n;
    int k;
    cin >> n >> k;
    vector<int> x(n);
    for(int x_i = 0; x_i < n; x_i++){
       cin >> x[x_i];
    }
    int result = hackerlandRadioTransmitters(x, k);
    cout << result << endl;
    return 0;
}
