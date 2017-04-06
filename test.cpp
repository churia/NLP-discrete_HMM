#include "hmm.h"
#include <fstream>
#include <iostream>
using namespace std;

#define  TEST_SIZE  2500
#define  MODEL_NUM  5

void load_file(const char * file, string * store, int size){
	ifstream input(file);
	if(input.is_open()){
		int i = 0;
		while(!input.eof() && i < size)
			input>>store[i++];
		input.close();
	}
	else{
		cout<<"Error opening file "<<file<<endl;
		exit(1);
	}
}

double viterbi(const HMM * hmm, string seq){
	int N = hmm->state_num;
	int T = seq.length();
	double pre_delta[N]; //we only need store delta(t-1,j) 
    double new_delta[N]; //store delta(t,j)
	
   	for(int t = 0; t < T; t++){
		int O_t = seq.at(t)-'A';
		if(t == 0){
			for(int j = 0; j < N; j++)
				pre_delta[j] = hmm->initial[j] * hmm->observation[O_t][j];
		}
		else{
			for(int j = 0; j < N; j++){
				double max_prob = -1;
				double tmp_p;
				for(int i = 0; i < N; i++){
					tmp_p = pre_delta[i] * hmm->transition[i][j];	
					if (tmp_p > max_prob)
						max_prob = tmp_p;
				}
				new_delta[j] = max_prob * hmm->observation[O_t][j];
			}
			for(int j = 0; j < N; j++)
				pre_delta[j] = new_delta[j];
		}
	}

	double p_max = pre_delta[0];
	for(int i = 1; i < N; i++){
		if(pre_delta[i] > p_max)
			p_max = pre_delta[i];
	}

	return p_max;
}

int main(int argc, char**argv){
	if (argc != 4){
		cout<<"./test model_list testing_data result"<<endl;
		exit(1);
	}
	char * modellist_file = argv[1];
	char * test_file = argv[2];
	char * output_file = argv[3];
	
	//load modellist
	string modellist[MODEL_NUM];
	load_file(modellist_file,modellist,MODEL_NUM);
	//load test observances
	string observances[TEST_SIZE];
	load_file(test_file,observances,TEST_SIZE);

	//load hmm models
	HMM hmms[MODEL_NUM];
	load_models(modellist_file,hmms,MODEL_NUM);
	
	//do testing and output
	ofstream output(output_file);
	for(int i = 0; i < TEST_SIZE; i++){
		int best_model;
		double p;
		double p_max = -1;
		for(int j = 0; j < MODEL_NUM; j++){
			p = viterbi(&hmms[j],observances[i]);
			if(p > p_max){
				best_model = j;
				p_max = p;
			}
		}
	//	output<<modellist[best_model]<<endl;
		output << modellist[best_model]<< " " <<p_max <<endl;
	}
	output.close();

	return 0;
}
