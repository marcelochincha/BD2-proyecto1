import math

datos = [i for i in range(0,2048)]
n = len(datos)


MI = 4
MAX_DEPTH = 3

FILE = []

#NIVEL 1
indexfile1 = datos[0:n:n//MI]
FILE.append(indexfile1)

#NIVEL 2
indexfile2 = []
for i in range(0,MI**1):
    STEP = n//MI
    left =  STEP * i
    right = STEP * (i +1)
    step = (right - left) // 4
    indexfile2 .append(datos[left:right:step])
FILE.append(indexfile2)

#Nivel 3
indexfile3 = []
for i in range(0,MI**2):
    STEP = n// (MI ** 2)
    left =  STEP * i
    right = STEP * (i +1)
    step = (right - left) // 4
    indexfile3.append(datos[left:right:step])
FILE.append(indexfile3)

print(FILE)


#print nice
"""
for i in range(MAX_DEPTH):
    print("Depth: ",i + 1)
    for j in range(0,len(indexTree[i])):
        print(indexTree[i][j])
"""
