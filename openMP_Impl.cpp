/**
 * @author Avinash Gaikwad, Neha Sarnaik
 * @rollno MIT2021019, MIT2021085
 */
#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>
#include <math.h>
#include <omp.h>
using namespace std;
double* matrixMultiplication(double * mat1, double * mat2, int size)
{
    double* resultMatrix = (double*) malloc(sizeof(double )*(size*size));
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            resultMatrix[i*size+j] = 0.00;
            for (int k = 0; k < size; k++){
                resultMatrix[i*size+j] += mat1[i*size+k] * mat2[k*size+j];
            }
        }
    }
    return resultMatrix;
}

void printMatrix(double* matrix, int dim){
    for(int i=0 ; i<dim ; i++){
        for (int j = 0; j < dim; j++)
            printf("%.02lf\t", matrix[i * dim + j]);
        cout<<endl;
    }
}

void permutateMatrix(double * finalMat, int *pVector, int size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            cout<<finalMat[pVector[i] * size + j]<<"\t";
        }
        cout<<endl;
    }
}

int main(int argc,char *argv[]){
    auto startTime=chrono::high_resolution_clock::now();
    string size_str(argv[1]);
    int size=stoi(size_str);
    string thread_str(argv[2]);
    int thread=stoi(thread_str);
    double *Amat =(double*)malloc(sizeof(double)*(size*size));
    int *Pvec = (int*)malloc(sizeof(int)*(size));
    double *Umat = (double*)malloc(sizeof(double)*(size*size));
    double *Lmat = (double*)malloc(sizeof(double)*(size*size));
    int temp;
    double tempA,tempL;
    #pragma omp parallel
    {
        srand(omp_get_thread_num());
        for(int i=0;i<size;i++)
        {
            Pvec[i]=i;
            #pragma omp parallel for num_threads(thread)
            for(int j=0;j<size;j++){
                Amat[i*size+j]=((double)(rand()%1000)) / 100.0;
                if(j==i){
                    Lmat[i*size+j]=1.0;
                    Umat[i*size+j]=Amat[i*size+j];
                }
                else if(j>i){
                    Lmat[i*size+j]=0.0;
                    Umat[i*size+j]=Amat[i*size+j];
                }
                else{
                    Umat[i*size+j]=0.0;
                    Lmat[i*size+j]=Amat[i*size+j];
                }
            }
        }
    }

    if(argc == 4 && atoi(argv[3]) == 1){
        cout<<"============================="<<endl;
        cout<<"Original matrix"<<endl;
        printMatrix(Amat, size);
    }
    for(int k=0;k<size;k++){
        double maximum=0.0;
        int kDash=-1;
        for(int i=k;i<size;i++){
            if(maximum<(abs(Amat[i*size+k]))){
                kDash=i;
                maximum=abs(Amat[i*size+k]);
            }
        }
        if(kDash==-1){
            cout<<"\nMatrix is singular"<<endl;
            return -1;
        }
        temp=Pvec[k];
        Pvec[k]=Pvec[kDash];
        Pvec[kDash]=temp;
        #pragma omp for schedule(dynamic) nowait
        for(int i=0;i<size;i++)
        {
            tempA=Amat[k*size+i];
            Amat[k*size+i]=Amat[kDash*size+i];
            Amat[kDash*size+i]=tempA;
        }
        #pragma omp for schedule(dynamic)
        for(int i=0;i<k;i++){
            tempL=Lmat[k*size+i];
            Lmat[k*size+i]=Lmat[kDash*size+i];
            Lmat[kDash*size+i]=tempL;
        }
        Umat[k*size+k]=Amat[k*size+k];
        for(int i=k+1;i<size;i++){
            Lmat[i*size+k]=Amat[i*size+k]/Umat[k*size+k];
            Umat[k*size+i]=Amat[k*size+i];
        }

        #pragma omp parallel for num_threads(thread) schedule(guided) collapse(2) shared(k, Amat, Lmat, Umat, size)
        for(int i=k;i<size;i++){
            for(int j=k;j<size;j++){
                Amat[i*size+j]=Amat[i*size+j]-Lmat[i*size+k]*Umat[k*size+j];
            }
        }
    }
    auto endTime = chrono::high_resolution_clock::now();
    auto total_time = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    if(argc == 4 && atoi(argv[3]) == 1){
        cout<<"============================="<<endl;
        cout<<"Permutation vector\t";
        for(int i = 0; i < size; i++){
            cout<<Pvec[i]<<" ";
        }cout<<endl;
        cout<<"============================="<<endl;
        permutateMatrix(matrixMultiplication(Lmat, Umat, size), Pvec, size);
        cout<<"============================="<<endl;
    }
    printf("%.05f\n", (float)total_time.count()/1000);
    return 0;
}

