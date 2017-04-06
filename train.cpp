#include "hmm.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

#define  TRAIN_SIZE  10000
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

void baum_welch(HMM * hmm, int iters, string * inst){
	int N = hmm->state_num;
	int P = hmm->observ_num;
	int T = inst[0].length();
	double a[N][N];
	double b[P][N];
	double pi[N];
	double alpha[T][N];
	double beta[T][N];
	int O[T];
	double PO;
	double sum_epsilon[N][N];//from 1 to Train_Size
	double sum_gamma[T][N];//from 1 to Train_Size
	double gamma_O_sum[P][N];//from 1 to Train_Size, t=1 to T
	
	//initial
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			a[i][j] = hmm->transition[i][j];
			sum_epsilon[i][j] = 0;
		}
		for(int k = 0; k < P; k++){
			b[k][i] = hmm->observation[k][i];
			gamma_O_sum[k][i] = 0;
		}
		for(int t = 0; t < T; t++)
			sum_gamma[t][i]=0;
		pi[i] = hmm->initial[i];
	}
	
	for(int iter = 0; iter < iters; iter++){
		//E-step
		for(int n = 0; n < TRAIN_SIZE; n++){
			for(int t = 0; t < T; t++)
				O[t] = inst[n].at(t) - 'A';

			//calculate alpha
			for(int t = 0; t < T; t++){
				for(int j = 0; j < N; j++){
					if(t == 0)
						alpha[t][j] = pi[j] * b[O[t]][j];
					else{
						double sum = 0;
						for(int i = 0; i < N; i++)
							sum += alpha[t-1][i] * a[i][j];
						alpha[t][j] = sum * b[O[t]][j];
					}
				}
			}
			
			//calculate beta
			for(int t = T-1; t >= 0; t--){
				for(int i = 0; i < N; i++){
					if(t == T-1)
						beta[t][i] = 1;
					else {
						double sum = 0;
						for(int j = 0; j < N; j++)
							sum += a[i][j] * b[O[t+1]][j] * beta[t+1][j];
						beta[t][i] = sum;
					}
				}
			}
			
			PO = 0;
			for(int i = 0; i < N; i++)
				PO += alpha[T-1][i];

			//calculate epsilon and gamma
			for(int i = 0; i < N; i++){
				for(int j = 0; j < N; j++){
					double eps = 0;
					for(int t = 0; t < T-1; t++)
						eps += alpha[t][i] * a[i][j] * b[O[t+1]][j] * beta[t+1][j];	
					sum_epsilon[i][j] += eps / PO;
				}
				for(int t = 0; t < T; t++){
					double gamma = alpha[t][i] * beta[t][i] / PO;
					sum_gamma[t][i] += gamma;
					gamma_O_sum[O[t]][i] += gamma;
				}
			}
		
		}

		//M-step
		for(int i = 0; i < N; i++){
			
			//calculate pi
			pi[i] = sum_gamma[0][i] / TRAIN_SIZE;

			//calculate A
			double sum_gamma_t1 = 0;
			for(int t = 0; t < T - 1; t++){
				sum_gamma_t1 += sum_gamma[t][i];
				sum_gamma[t][i] = 0; //clean for next iter	
			}
			for(int j = 0; j < N; j++){
				a[i][j] = sum_epsilon[i][j] / sum_gamma_t1;
				sum_epsilon[i][j] = 0;//clean for next iter
			}

			//claculate B
			double sum_gamma_t = sum_gamma_t1 + sum_gamma[T-1][i];
			for(int k = 0; k < P; k++){
				b[k][i] = gamma_O_sum[k][i] / sum_gamma_t;
				gamma_O_sum[k][i] = 0;//clean for next iter
			}
			
			//clean gamma for next iter
			sum_gamma[T-1][i] = 0;
		}

	}

	for(int i = 0; i < N; i++){
		hmm->initial[i]=pi[i];
		for(int j = 0; j < N; j++){
			hmm->transition[i][j] = a[i][j];
		}
		for(int k = 0; k < P; k++){
			hmm->observation[k][i] = b[k][i];
		}
	}
}

int main(int argc, char**argv){
	if (argc != 5){
		cout<<"./train iteration model_initial model_train_data output_model"<<endl;
		exit(1);
	}
	int iter = stoi(argv[1]);
	char * model_init_file = argv[2];
	char * train_file = argv[3];
	char * output_file = argv[4];
	
	//load model_init
	HMM hmm;
	loadHMM(&hmm,model_init_file);
	
	//load train observances
	string instances[TRAIN_SIZE];
	load_file(train_file,instances,TRAIN_SIZE);

	//do training with Baum-Welch algorithm
	baum_welch(&hmm,iter,instances);

	//save hmm models
	free(hmm.model_name);
	hmm.model_name = output_file;
	FILE *output = open_or_die(output_file,"w");
	dumpHMM(output, &hmm);
	
	return 0;
}
