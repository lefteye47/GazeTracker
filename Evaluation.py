import math
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import pylab

#If needed then adjust the parameters
totalframe=25  #The number of calibration points used
width=1366     #The screen width
height=768     #The screen height

files=open('svm testing')
l=files.readlines()
fi=open('SVM','w')
#Adjust the range to other if needed
for i in range(20,1000,40):#This is 25 calpoints 
    fi.write(l[i])

fi.close()

a=open('SVM')   #The the output eye coordinate from the GazeTracker 
b=open('calpoints.txt')#The ground Truth of the calpoints
l=a.readlines()
line=b.readlines()
f=open('combine','w')

#Reconstruct two files together
for i in range(0,totalframe):  
    m=l[i].split()
    n=[float(x) for x in m]
    r=line[i].split()
    t=[float(x) for x in r]
    t[0]=t[0]*width
    t[1]=t[1]*height
    f.write(str(n[0]))
    f.write(str('  '))
    f.write(str(n[1]))
    f.write(str('  '))
    f.write(str(t[0]))
    f.write(str('  '))
    f.write(str(t[1]))
    f.write('\n')

f.close()

file=open('combine')
l=file.readlines()
j=0
v=0


for i in range(0,totalframe):
  n=l[i].split()
  k=[float(x) for x in n]
  a=(k[0]-k[2])*(k[0]-k[2])
  b=(k[1]-k[3])*(k[1]-k[3])
  s=math.sqrt(a+b)
  j=j+s

mean=j/totalframe
for i in range(0,totalframe):
  n=l[i].split()
  k=[float(x) for x in n]
  a=(k[0]-k[2])*(k[0]-k[2])
  b=(k[1]-k[3])*(k[1]-k[3])
  s=math.sqrt(a+b)
  v=v+(s-mean)*(s-mean)

variance=v/(totalframe-1)#Get the variance 
sigma=math.sqrt(variance)

x = np.linspace(-100,300,400) #Can change if needed to show accuracy
plt.plot(x,mlab.normpdf(x,mean,sigma))#Plot the graph
pylab.axvline(x=100)
plt.grid()

plt.show()  #Show the graph
