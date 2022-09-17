/**
 * @author Avinash Gaikwad, Neha Sarnaik
 * @rollno MIT2021019, MIT2021085
 */
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <chrono>
using namespace std;
void displayMatrix(double* matrix, int dimentions){
    for(int i=0 ; i < dimentions ; i++) {
        for (int j = 0; j < dimentions; j++)
            printf("%.02lf\t", matrix[i * dimentions + j]);
        cout<<endl;
    }
}
double* multiplyMatrix(double * matOne, double * matTwo, int globalMatSize){
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
int main(int argc, char *argv[])
{
    auto start = chrono::high_resolution_clock::now();
    string n_str(argv[1]);
    int sizeOfMatrix = stoi(n_str);
    double* originalMatrix = (double*)malloc(sizeof(double) * (sizeOfMatrix * sizeOfMatrix));
    double* lowerMatrix = (double*)malloc(sizeof(double) * (sizeOfMatrix * sizeOfMatrix));
    double* upperMatrix = (double*)malloc(sizeof(double) * (sizeOfMatrix * sizeOfMatrix));
    int* pivotVector;
    pivotVector = (int*)malloc(sizeof(int) * (sizeOfMatrix));
    srand(time(0));
    for (int i=0 ; i < sizeOfMatrix ; i++){
        pivotVector[i] = i;
        for (int j=0 ; j < sizeOfMatrix ; j++){
            originalMatrix[i * sizeOfMatrix + j] = ((double)(rand() % 1000)) / 100.0;
            if(j>i){
                upperMatrix[i * sizeOfMatrix + j] = originalMatrix[i * sizeOfMatrix + j];
                lowerMatrix[i * sizeOfMatrix + j] = 0.0;
            }
            else if(j==i){
                upperMatrix[i * sizeOfMatrix + j] = originalMatrix[i * sizeOfMatrix + j];
                lowerMatrix[i * sizeOfMatrix + j] = 1.0;
            }
            else{
                upperMatrix[i * sizeOfMatrix + j] = 0.0;
                lowerMatrix[i * sizeOfMatrix + j] = originalMatrix[i * sizeOfMatrix + j];
            }
        }
    }
    //displayMatrix(originalMatrix, sizeOfMatrix);
    for(int k=0 ; k < sizeOfMatrix ; k++){
        double maxElement = 0.0;
        int kDash = -1;
        for(int i=k; i < sizeOfMatrix ; i++){
            if(maxElement < abs(originalMatrix[i * sizeOfMatrix + k])){
                maxElement = abs(originalMatrix[i * sizeOfMatrix + k]);
                kDash = i;
            }
        }
        if(kDash == -1){
            cerr<<endl<<"Singular matrix ERROR"<<endl<<"Program terminated with code 1";
            cout<<k<<endl;
            return 1;
        }

        int temp0;
        temp0 = pivotVector[k];
        pivotVector[k] = pivotVector[kDash];
        pivotVector[kDash] = temp0;

        double temp1;
        for(int i=0 ; i < sizeOfMatrix ; i++){
            temp1 = originalMatrix[k * sizeOfMatrix + i];
            originalMatrix[k * sizeOfMatrix + i] = originalMatrix[kDash * sizeOfMatrix + i];
            originalMatrix[kDash * sizeOfMatrix + i] = temp1;
        }

        for(int i=0 ; i<k ; i++){
            temp1 = lowerMatrix[k * sizeOfMatrix + i];
            lowerMatrix[k * sizeOfMatrix + i] = lowerMatrix[kDash * sizeOfMatrix + i];
            lowerMatrix[kDash * sizeOfMatrix + i] = temp1;
        }
        upperMatrix[k * sizeOfMatrix + k] = originalMatrix[k * sizeOfMatrix + k];
        for(int i=k+1 ; i < sizeOfMatrix ; i++){
            lowerMatrix[i * sizeOfMatrix + k] = originalMatrix[i * sizeOfMatrix + k] / upperMatrix[k * sizeOfMatrix + k];
            upperMatrix[k * sizeOfMatrix + i] = originalMatrix[k * sizeOfMatrix + i];
        }

        for(int i=k ; i < sizeOfMatrix ; i++){
            for(int j=k ; j < sizeOfMatrix ; j++){
                originalMatrix[i * sizeOfMatrix + j] -= lowerMatrix[i * sizeOfMatrix + k] * upperMatrix[k * sizeOfMatrix + j];
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto time_taken = chrono::duration_cast<chrono::milliseconds>(end - start);
    //cout<<"=============================="<<endl;
    //displayMatrix(multiplyMatrix(lowerMatrix, upperMatrix, sizeOfMatrix), sizeOfMatrix);
    printf("%.05f\n", (float)time_taken.count()/1000);
    return 0;
}


