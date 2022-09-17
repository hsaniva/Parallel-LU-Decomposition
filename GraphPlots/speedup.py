import matplotlib.pyplot as plt
import csv
import scipy.optimize as opt
import numpy as np
from os.path import abspath

x_thread = []
y_thread = []

x_openmp = []
y_openmp = []

y_seq = []

res_thread=[]
res_openmp=[]

with open(abspath('.././OutputFiles/sequential_timings.csv'),'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=' ')
    for row in lines:
        y_seq.append(float(row[1]))

with open(abspath('.././OutputFiles/pThread_constThread_timings.csv'),'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=' ')
    for row in lines:
        x_thread.append(row[0])
        y_thread.append(float(row[2]))

with open(abspath('.././OutputFiles/openMP_constThread_timings.csv'),'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=' ')
    for row in lines:
        x_openmp.append(row[0])
        y_openmp.append(float(row[2]))

res_thread = [i / j for i, j in zip(y_seq, y_thread)]

res_openmp = [i / j for i, j in zip(y_seq, y_openmp)]

plt.plot(x_thread, res_thread, color = 'r', linestyle = 'solid',
         marker = 'o',label = "PThread")

plt.plot(x_openmp, res_openmp, color = 'b', linestyle = 'solid',
         marker = 'o',label = "OpenMP")

plt.xticks(rotation = 25)
plt.xlabel('Size of matrix')
plt.ylabel('Speed up')
plt.title('Speed up', fontsize = 20)
plt.grid()
plt.legend()
plt.savefig('./Images/speedup.png')
plt.show()
