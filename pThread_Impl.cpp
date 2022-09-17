/**
 * @author Avinash Gaikwad, Neha Sarnaik
 * @rollno MIT2021019, MIT2021085
 */
#include <iostream>
#include <thread>
#include<pthread.h>

using namespace std;

double *originalMat, *lowerTriang, *upperTriang;
int *permutationVector, globalMatSize, numberOfUserThreads;
const string PI = "pi";
const string SI = "si";
struct matInputStruct{
    int colFrom, colTill;
};
struct rowSwapData{
    double* matrix;
    int k, k_dash;
    int colFrom, colTill;
};
struct threadData{
    int colFrom;
    int colTill;
    int k_value;
};
double* multiplyMatrix(double * matOne, double * matTwo){
    double* resultantMatrix = (double*) malloc(sizeof(double ) * (globalMatSize * globalMatSize));
    for (int m = 0; m < globalMatSize; m++) {
        for (int n = 0; n < globalMatSize; n++) {
            resultantMatrix[m * globalMatSize + n] = 0.00;
            for (int k = 0; k < globalMatSize; k++) {
                resultantMatrix[m * globalMatSize + n] += matOne[m * globalMatSize + k] * matTwo[k * globalMatSize + n];
            }
        }
    }
    return resultantMatrix;
}
void serialRandomInput(int n) {
    for (int i=0 ; i<n ; i++){
        permutationVector[i] = i;
        for (int j=0 ; j<n ; j++){
            originalMat[i*n + j] = ((double)(rand()%1000)) / 100.0;
            if(j>i){
                upperTriang[i*n + j] = originalMat[i*n + j];
                lowerTriang[i*n + j] = 0.0;
            }
            else if(j==i){
                upperTriang[i*n + j] = originalMat[i*n + j];
                lowerTriang[i*n + j] = 1.0;
            }
            else{
                upperTriang[i*n + j] = 0.0;
                lowerTriang[i*n + j] = originalMat[i*n + j];
            }
        }
    }
}


void* parallelRandomInput(void *arg){
    auto *pTData =(struct matInputStruct  *)arg;
    srand(pthread_self());
    for(int i = 0; i < globalMatSize; i++){
        for(int j = pTData->colFrom; j <=pTData->colTill; j++){
            originalMat[i*globalMatSize + j] = ((double)(rand()%1000)) / 100.0;
            if(j>i)
            {
                upperTriang[i*globalMatSize + j] = originalMat[i*globalMatSize + j];
                lowerTriang[i*globalMatSize + j] = 0.0;
            }
            else if(j==i)
            {
                upperTriang[i*globalMatSize + j] = originalMat[i*globalMatSize + j];
                lowerTriang[i*globalMatSize + j] = 1.0;
            }
            else
            {
                upperTriang[i*globalMatSize + j] = 0.0;
                lowerTriang[i*globalMatSize + j] = originalMat[i*globalMatSize + j];
            }
        }
    }
    return NULL;
}

void displayMatrix(double* matrix, int dimentions){
    for(int i=0 ; i < dimentions ; i++) {
        for (int j = 0; j < dimentions; j++)
            printf("%.02lf\t", matrix[i * dimentions + j]);
        cout<<endl;
    }
}

void parallelizedMatrixInput(int mat_size){
//    unsigned int numberOfHWLevelConcurrentThreads = std::thread::hardware_concurrency();
    int numberOfHWLevelConcurrentThreads = numberOfUserThreads;
    pthread_t myThreads[numberOfHWLevelConcurrentThreads];
    int threadCounter = 0, i;
    int workLoadPerThread = mat_size / numberOfHWLevelConcurrentThreads;
    if(numberOfHWLevelConcurrentThreads > mat_size){
        for(i = 0; i < mat_size; i++){
            matInputStruct *temp = new matInputStruct();
            temp->colFrom = i;
            temp->colTill = i;
            pthread_create(&myThreads[threadCounter++], NULL, parallelRandomInput, temp);
        }
    }else{
        int i;
        for(i = 0; i < (numberOfHWLevelConcurrentThreads - 1) * workLoadPerThread; i+=workLoadPerThread){
            matInputStruct *temp = new matInputStruct();
            temp->colFrom = i;
            temp->colTill = i + workLoadPerThread - 1;
            pthread_create(&myThreads[threadCounter++], NULL, parallelRandomInput, temp);
        }
        matInputStruct *temp = new matInputStruct();
        temp->colFrom = i;
        temp->colTill = i + workLoadPerThread - 1 + (mat_size % numberOfHWLevelConcurrentThreads);
        pthread_create(&myThreads[threadCounter++], NULL, parallelRandomInput, temp);
    }

    if(workLoadPerThread == 0){
        numberOfHWLevelConcurrentThreads = mat_size;
    }
    for(int i = 0; i < numberOfHWLevelConcurrentThreads; i++){
        pthread_join(myThreads[i], NULL);
    }
}

void* makeNonDiagonalsZero(void* arg)
{
    auto *arg_struct = (threadData*)arg;
    int k = arg_struct->k_value;
    int low = arg_struct->colFrom;
    int high = arg_struct->colTill;
    for(int i = low; i < min(high, globalMatSize); i++)
        for(int j=k ; j<globalMatSize ; j++)
            originalMat[i*globalMatSize + j] -= lowerTriang[i*globalMatSize + k] * upperTriang[k*globalMatSize + j];
    pthread_exit(NULL);
}
void * rowSwapInParallel(void *arg){
    auto *pTData =(struct rowSwapData  *)arg;
    double temp1;
    for(int i = pTData->colFrom; i <= pTData->colTill; i++){
        temp1 = pTData->matrix[pTData->k*globalMatSize + i];
        originalMat[pTData->k*globalMatSize + i] = originalMat[pTData->k_dash*globalMatSize + i];
        originalMat[pTData->k_dash*globalMatSize + i] = temp1;
    }
    pthread_exit(NULL);
}
void callSwapInParallel(double * matrix, int k, int kd){
//    unsigned int numberOfHWLevelConcurrentThreads = std::thread::hardware_concurrency();
    int numberOfHWLevelConcurrentThreads = numberOfUserThreads;
    int workLoadPerThread = globalMatSize / numberOfHWLevelConcurrentThreads;
    int threadCounter = 0;
    pthread_t myThreads[numberOfHWLevelConcurrentThreads];
    if(numberOfHWLevelConcurrentThreads > globalMatSize){
        for(int i = 0; i < globalMatSize; i++){
            auto *commonThreadData = new rowSwapData();
            commonThreadData->matrix = matrix;
            commonThreadData->k = k;
            commonThreadData->k_dash = kd;
            commonThreadData->colFrom = i;
            commonThreadData->colTill = i;
            pthread_create(&myThreads[threadCounter++], NULL, rowSwapInParallel, commonThreadData);
        }
    }else {
        int i;
        for (i = 0; i < (numberOfHWLevelConcurrentThreads - 1) * workLoadPerThread; i += workLoadPerThread) {
            auto *commonThreadData = new rowSwapData();
            commonThreadData->matrix = matrix;
            commonThreadData->k = k;
            commonThreadData->k_dash = kd;
            commonThreadData->colFrom = i;
            commonThreadData->colTill = i + workLoadPerThread - 1;
            pthread_create(&myThreads[threadCounter++], NULL, rowSwapInParallel, commonThreadData);
        }
        auto *commonThreadData = new rowSwapData();
        commonThreadData->matrix = matrix;
        commonThreadData->k = k;
        commonThreadData->k_dash = kd;
        commonThreadData->colFrom = i;
        commonThreadData->colTill = i + workLoadPerThread - 1 + (globalMatSize % numberOfHWLevelConcurrentThreads);
        pthread_create(&myThreads[threadCounter++], NULL, rowSwapInParallel, commonThreadData);
    }
    if(workLoadPerThread == 0){
        numberOfHWLevelConcurrentThreads = globalMatSize;
    }
    for(int i = 0; i < numberOfHWLevelConcurrentThreads; i++){
        pthread_join(myThreads[i], NULL);
    }
}
void permutateMatrix(double * finalMat, int *pVector){
	for(int i = 0; i < globalMatSize; i++){
		for(int j = 0; j < globalMatSize; j++){
			cout<<finalMat[pVector[i] * globalMatSize + j]<<"\t";
		}
		cout<<endl;	
	}
}
int main(int argc, char **argv) {
    auto beginning = chrono::high_resolution_clock::now();
    if(argc < 4){
        cerr<<"Error: Input required"<<endl;
        return -1;
    }
    int mat_size = stoi(argv[1]);
    permutationVector = (int*)malloc(sizeof(int) * mat_size);
    originalMat = (double*)malloc(sizeof(double)*(mat_size*mat_size));
    lowerTriang = (double*)malloc(sizeof(double)*(mat_size*mat_size));
    upperTriang = (double*)malloc(sizeof(double)*(mat_size*mat_size));

    globalMatSize = mat_size;
    string pIsI(argv[3]);

    numberOfUserThreads = stoi(argv[2]);

    if(pIsI.compare(PI)==0){
        for(int i = 0; i < mat_size; i++)
            permutationVector[i] = i;
        parallelizedMatrixInput(mat_size);
    }else if(pIsI.compare(SI) == 0){
        serialRandomInput(mat_size);
    }
    if(argc == 5 && atoi(argv[4]) == 1){
	cout<<"============================="<<endl;
        cout<<"Original matrix"<<endl;
        displayMatrix(originalMat, mat_size);
    }
    for(int k=0 ; k<mat_size ; k++)
    {
        double max = 0.0;
        int kDash = -1;
        for(int i=k; i<mat_size ; i++){
            if(max < abs(originalMat[i*mat_size + k])){
                max = abs(originalMat[i*mat_size + k]);kDash = i;
            }
        }
        if(kDash == -1)
            cerr<<endl<<"Error: Singular matrix"<<endl;
        double temp1;
        temp1 = permutationVector[k];
        permutationVector[k] = permutationVector[kDash];
        permutationVector[kDash] = temp1;
        callSwapInParallel(originalMat, k, kDash);
        for(int i=0 ; i<k ; i++){
            temp1 = lowerTriang[k*mat_size + i];
            lowerTriang[k*mat_size + i] = lowerTriang[kDash * mat_size + i];
            lowerTriang[kDash * mat_size + i] = temp1;
        }
        upperTriang[k*mat_size + k] = originalMat[k*mat_size + k];

        for(int i=k+1 ; i<mat_size ; i++)        {
            lowerTriang[i*mat_size + k] = originalMat[i*mat_size + k] / upperTriang[k*mat_size + k];
            upperTriang[k*mat_size + i] = originalMat[k*mat_size + i];
        }

        int t_num = 0;
        pthread_t threads[numberOfUserThreads];
        for(int i = 0; i < numberOfUserThreads; i++){
            threadData *tData;
            tData = (threadData*)malloc(sizeof(threadData));
            tData->colFrom = k + t_num * (globalMatSize - k) / numberOfUserThreads;
            tData->colTill = k + (t_num + 1) * (globalMatSize - k) / numberOfUserThreads;
            tData->k_value = k;
            pthread_create(&threads[i], NULL, makeNonDiagonalsZero, (void *) tData);
            t_num++;
        }

        void* status[numberOfUserThreads];
        for(int i = 0; i < numberOfUserThreads; i++){
            pthread_join(threads[i],&status[i]);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto time_taken = chrono::duration_cast<chrono::milliseconds>(end - beginning);
    printf("%.05f\n", (float)time_taken.count()/1000);
    if(argc == 5 && atoi(argv[4]) == 1){
        cout<<"============================="<<endl;
        cout<<"Permutation vector\t";
        for(int i = 0; i < globalMatSize; i++){
        	cout<<permutationVector[i]<<" ";
        }cout<<endl;
	cout<<"============================="<<endl;
        permutateMatrix(multiplyMatrix(lowerTriang, upperTriang), permutationVector);
	cout<<"============================="<<endl;
    }
    return 0;
}

