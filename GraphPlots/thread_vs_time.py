import matplotlib.pyplot as plt
import csv
import scipy.optimize as opt
import numpy as np
from os.path import abspath

x_thread = []
y_thread = []

x_openmp = []
y_openmp = []

with open(abspath('.././OutputFiles/pThread_timings.csv'),'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=' ')
    for row in lines:
        x_thread.append(row[1])
        y_thread.append(float(row[2]))

with open(abspath('.././OutputFiles/openMP_timings.csv'),'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=' ')
    for row in lines:
        x_openmp.append(row[1])
        y_openmp.append(float(row[2]))

plt.plot(x_thread, y_thread, color = 'r', linestyle = 'solid',
         marker = 'o',label = "PThread")

plt.plot(x_openmp, y_openmp, color = 'b', linestyle = 'solid',
         marker = 'o',label = "OpenMP")

plt.xticks(rotation = 25)
plt.xlabel('Number of Threads')
plt.ylabel('Time taken in seconds')
plt.title('LU Decomposition using PThread and OpenMP', fontsize = 20)
plt.grid()
plt.legend()
plt.savefig('./Images/thread_vs_time.png')
plt.show()
