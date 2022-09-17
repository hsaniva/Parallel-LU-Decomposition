import matplotlib.pyplot as plt
import csv
import scipy.optimize as opt
import numpy as np
from os.path import abspath

x_thread = []
y_thread = []

x_openmp = []
y_openmp = []

x_seq = []
y_seq = []

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

with open(abspath('.././OutputFiles/sequential_timings.csv'),'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=' ')
    for row in lines:
        x_seq.append(row[0])
        y_seq.append(float(row[1]))

plt.plot(x_thread, y_thread, color = 'r', linestyle = 'solid',
         marker = 'o',label = "PThread")

plt.plot(x_openmp, y_openmp, color = 'b', linestyle = 'solid',
         marker = 'o',label = "OpenMP")

plt.plot(x_seq, y_seq, color = 'g', linestyle = 'solid',
         marker = 'o',label = "Sequential")

plt.xticks(rotation = 25)
plt.xlabel('Size of matrix')
plt.ylabel('Time taken in seconds')
plt.title('LU Decomposition', fontsize = 20)
plt.grid()
plt.legend()
plt.savefig('./Images/size_vs_time.png')
plt.show()
