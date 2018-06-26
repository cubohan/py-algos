#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include "classifier.h"

//////////////

using namespace std;

class GNB {
public:

	vector<string> possible_labels = {"left","keep","right"};
	vector <vector<double>> partials; // [P(label), [ x1: sigma, mu,  x2: sigma, mu, ...  ], ] 
    vector <double> p_label;


	/**
  	* Constructor
  	*/
 	GNB();

	/**
 	* Destructor
 	*/
 	virtual ~GNB();

 	void train(vector<vector<double> > data, vector<string>  labels);

  	string predict(vector<double>);

};

#endif


//////////////


/**
 * Initializes GNB
 */
GNB::GNB() {

}

GNB::~GNB() {}

void GNB::train(vector<vector<double>> data, vector<string> labels)
{	
	// vector<string> possible_labels = {"left","keep","right"};
    // vector <vector<double>> partials; // [P(label), [ x1: sigma, mu,  x2: sigma, mu, ...  ], ] 
    // vector <double> p_label;

    //
	vector <int> data_count;
	int feat_n = data[0].size();
    for (auto &label : this->possible_labels){
    	this->p_label.push_back(0);
    	vector<double> _partial(feat_n*2, 0);
    	this->partials.push_back(_partial);
    	data_count.push_back(0);
    }
	for (int data_i=0; data_i<data.size(); data_i++){
		int label_i = 0;//std::find(possible_labels.begin(), possible_labels.end(), labels[label_i]) - possible_labels.begin();
		for(int i=0; i<this->possible_labels.size(); i++){
		    if (this->possible_labels[i] == labels[data_i])
		        label_i = i;
		}
		vector<double> &entry = data[data_i];
		for (int feat_i = 0; feat_i < entry.size(); feat_i++){
			if (feat_i == 1){
				entry[feat_i] = fmod(entry[feat_i], 4);
			}
			// format: mu, sigma
			this->partials[label_i][feat_i*2] = (this->partials[label_i][feat_i*2] * data_count[label_i] + entry[feat_i]) / (data_count[label_i] + 1); 
			// mu
			this->partials[label_i][feat_i*2+1] = pow( ( pow(this->partials[label_i][feat_i*2+1], 2) * data_count[label_i] + pow(this->partials[label_i][feat_i*2] - entry[feat_i], 2) ) / (data_count[label_i] + 1), 0.5); 
			// sigma
			
		}	
		data_count[label_i]++;
	}
	for (int i=0; i<this->p_label.size(); i++){
		this->p_label[i] = float(data_count[i]) / data.size();
// 		for (int j=0; j<partials[i].size()/2; j++){
// 		    cout << " FEAT _" << j << ": MU: " << partials[i][j*2] << ", SIG: " << partials[i][j*2+1] << endl;
// 		}
	}
	/*
		Trains the classifier with N data points and labels.

		INPUTS
		data - array of N observations
		  - Each observation is a tuple with 4 values: s, d, 
		    s_dot and d_dot.
		  - Example : [
			  	[3.5, 0.1, 5.9, -0.02],
			  	[8.0, -0.3, 3.0, 2.2],
			  	...
		  	]

		labels - array of N labels
		  - Each label is one of "left", "keep", or "right".
	*/
}

string GNB::predict(vector<double> sample)
{
	/*
		Once trained, this method is called and expected to return 
		a predicted behavior for the given observation.

		INPUTS

		observation - a 4 tuple with s, d, s_dot, d_dot.
		  - Example: [3.5, 0.1, 8.5, -0.2]

		OUTPUT

		A label representing the best guess of the classifier. Can
		be one of "left", "keep" or "right".
		"""
		# TODO - complete this
	*/
	double max_p = 0;
	int label_i = 0;

	for (int i=0; i<this->possible_labels.size(); ++i){
		double l_p = this->p_label[i];
		for (int feat_i=0; feat_i<sample.size(); ++feat_i){
			double &mu = this->partials[i][feat_i*2];
			double var = pow(this->partials[i][feat_i*2+1], 2);
			l_p *= 1 / (2 * M_PI * var) * exp(-(pow(sample[feat_i] - mu, 2) / (2 * var)));
		}
		if (max_p < l_p){
			max_p = l_p;
			label_i = i;
		}
	}

	return this->possible_labels[label_i];

}



///////////////////////

// #include "classifier.h"
// #include <iostream>
// #include <fstream>
// #include <math.h>
// #include <vector>

// using namespace std;

vector<vector<double> > Load_State(string file_name)
{
    ifstream in_state_(file_name.c_str(), ifstream::in);
    vector< vector<double >> state_out;
    string line;
    
    while (getline(in_state_, line)) 
    {
    	
    	istringstream iss(line);
    	vector<double> x_coord;
    	double state1 = 0;
        double state2 = 0;
        double state3 = 0;
        double state4 = 0;
        iss >> state1;
        x_coord.push_back(state1);
        if (iss.peek() == ',') iss.ignore();
        iss >> state2;
        x_coord.push_back(state2);
        if (iss.peek() == ',') iss.ignore();
    	iss >> state3;
    	x_coord.push_back(state3);
        if (iss.peek() == ',') iss.ignore();
        iss >> state4;
        x_coord.push_back(state4);
        if (iss.peek() == ',') iss.ignore();
        state_out.push_back(x_coord);
    }
    return state_out;
}
vector<string> Load_Label(string file_name)
{
    ifstream in_label_(file_name.c_str(), ifstream::in);
    vector< string > label_out;
    string line;
    while (getline(in_label_, line)) 
    {
    	istringstream iss(line);
    	string label;
	    iss >> label;
    
	    label_out.push_back(label);
    }
    return label_out;
    
}

int main() {
    
    vector< vector<double> > X_train = Load_State("./train_states.txt");
    vector< vector<double> > X_test  = Load_State("./test_states.txt");
    vector< string > Y_train  = Load_Label("./train_labels.txt");
    vector< string > Y_test   = Load_Label("./test_labels.txt");
    
	cout << "X_train number of elements " << X_train.size() << endl;
	cout << "X_train element size " << X_train[0].size() << endl;
	cout << "Y_train number of elements" << Y_train.size() << endl;
	
	GNB gnb = GNB();
	
	gnb.train(X_train, Y_train);
	

	cout << "X_test number of elements " << X_test.size() << endl;
	cout << "X_test element size " << X_test[0].size() << endl;
	cout << "Y_test number of elements " << Y_test.size() << endl;
	
	int score = 0;
	for(int i = 0; i < X_test.size(); i++)
	{
		vector<double> coords = X_test[i];
		string predicted = gnb.predict(coords);
		if(predicted.compare(Y_test[i]) == 0)
		{
			score += 1;
		}
	}

	float fraction_correct = float(score) / Y_test.size();
	cout << "You got " << (100*fraction_correct) << " correct" << endl;

	return 0;
}


