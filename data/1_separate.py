#cargar data.csv y partirlo en 2 archivos de forma aleatoria

import pandas as pd
import numpy as np
import csv

data = pd.read_csv('data.csv')
print(data["StoreLocation"])
data['StoreLocation'] = data['StoreLocation'].str.replace(',', ';')
data['StoreLocation'] = data['StoreLocation'].str.replace('\n', ' ')
print(data['StoreLocation'])


#Reducir el tamaño de la muestra a 10000
data = data.iloc[0:10000]

train = data.iloc[:int(len(data)*0.5)]
test = data.iloc[int(len(data)*0.5):]

train.to_csv('dataA.csv', index=False, quoting=csv.QUOTE_MINIMAL)
test.to_csv('dataB.csv', index=False)

data.info()

print("Longitud maxima de los valores de las columnas object")
for col in data.columns:
    if data[col].dtype == np.object_:
        print(col, data[col].str.len().max())

