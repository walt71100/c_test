#!/usr/bin/env python

import numpy as np
import datetime


t = datetime.datetime.now()

with open('TEST.PRN', 'r') as in_f:
    inputdata = np.loadtxt('TEST.PRN', delimiter='/n') #, usecols=range(5))

with open('test_output.txt','r') as out_f:
    outdata = np.loadtxt('test_output.txt')#, delimiter=' ')

k = 0
c = len(outdata) -1
doublesII = [[0 for x in xrange(c+1)] for x in xrange(c+1)]    #np.asarray() 

temp_accum = 0

for k in range(k,c+1): 
    for j in range(0,c+1): 
	for i in range(c,len(inputdata)):     
	   temp_accum += inputdata[i-k]*inputdata[i-j];
	doublesII[k][j] = temp_accum;
	temp_accum = 0;
    
myarray = np.array(doublesII)
print("")
print("")

print("Test Runtime: " + t.strftime('%m/%d/%Y'))
print("Calculation Parameters:")
print("c = " + str(c))
print("Output from UUT:")
print(outdata)
print("")
print("Calculated by Unit Test Script:")
print(myarray)
print("")


print("checking UUT output against internally calculated value")
error_count = 0 
for k in range(c+1):
    for j in range(c+1):
        if (abs(outdata[k,j] - myarray[k,j])) < 1E-6:
            print("element " + str(k) + "," + str(j) + " passed")
        else:
            print("element " + str(k) + "," + str(j) + " out of range!")
            error_count += 1
            
if(error_count == 0):
    print("Test Passed")
else:
    print("Test Failed")    