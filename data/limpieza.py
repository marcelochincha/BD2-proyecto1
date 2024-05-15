#cargar data.csv y partirlo en 2 archivos de forma aleatoria

import pandas as pd
import numpy as np

data = pd.read_csv('data.csv')
data = data.sample(frac=1).reset_index(drop=True)

#Remover tuplas con valores nulos
data = data.dropna()

train = data.iloc[:int(len(data)*0.5)]
test = data.iloc[int(len(data)*0.5):]

train.to_csv('dataA.csv', index=False)
test.to_csv('dataB.csv', index=False)

data.info()

print("Longitud maxima de los valores de las columnas object")
for col in data.columns:
    if data[col].dtype == np.object_:
        print(col, data[col].str.len().max())

