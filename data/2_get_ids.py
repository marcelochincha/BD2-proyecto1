#load data a y b

import pandas as pd

dataA = pd.read_csv('dataA.csv')
dataB = pd.read_csv('dataB.csv')

#join dataA and dataB
data = pd.concat([dataA, dataB], ignore_index=True)

#get id and convert it to a array
consumer_id =  list(data['CustomerID'].values)
consumer_id.sort()

for i in consumer_id:
    print(i)

#Export it as a std::vector 
##with open('consumer_id.h', 'w') as f:
##    f.write('std::vector<int> consumer_id = {')
##    for i in range(len(consumer_id)):
##        f.write(str(consumer_id[i]))
##        if i != len(consumer_id) - 1:
##            f.write(', ')
##    f.write('};')



