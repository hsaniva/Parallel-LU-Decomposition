# Parallel LU decomposition
LU Decomposition (where 'LU' stands for 'lower upper') is a classical method for transforming an N x N matrix A into the product of a lower-triangular matrix L and an upper-triangular matrix U, A = LU.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Usage](#usage)
3. [Screenshots](#screenshots)

<a name="prerequisites"></a>
### Prerequisites
***
Below are the libraries/tools required in order to run the program
* Linux based OS
* C++ compiler
* Python and other libraries like matplotlib, numpy etc.
* Make

<a name="usage"></a>
## Usage
***
* make clean (For cleaning the project files such as object files, output files etc.
* make all (For compiling the program(s) and creating their respective executables.)
* ./start.sh (For executing the main program)

<a name="screenshots"></a>
## Screenshots
Menu page
![Main](./images/main.png)

pThread implementation
![ss1](./images/execution1.png)

LU decomposition verification
![ss2](./images/execution2.png)

Graphical outputs
![ss3](./images/size_vs_time.png)
![ss4](./images/size_vs_time_speedup.png)
![ss5](./images/speedup.png)
![ss6](./images/thread_vs_time.png)
![ss7](./images/thread_vs_time_speedup.png)
