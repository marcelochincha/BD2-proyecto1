# BD2-proyecto1
Proyecto 1 de BD2

## Datos: Retail Transaction Dataset

### Fuente
https://www.kaggle.com/datasets/fahadrehman07/retail-transaction-dataset

### Forma de registro 
```c          
    int     CustomerID //PRIMARY KEY
    char    ProductID
    int     Quantity
    float   Price
    char    TransactionDate[16]
    char    PaymentMethod[11]
    char    StoreLocation[68]
    char    ProductCategory[11]
    float   DiscountApplied
    float   TotalAmount   
``` 

## Técnicas de Organización de Archivos Utilizadas

### Técnica 1: Sequential File / AVL File
Los AVL Files utilizan un Árbol AVL (Adelson-Velsky y Landis) para la organización de archivos. Un Árbol AVL es un árbol binario de búsqueda balanceado que mantiene sus alturas de subárboles balanceadas a lo más por uno de sus principales beneficio de usar AVL Files es la garantía de tiempos de búsqueda, inserción y eliminación eficientes en orden logarítmico, lo cual es crucial para operaciones de acceso a bases de datos grandes, como la que estamos trabajando.

#### Algoritmo de Inserción

1. Insertar el elemento en la posición que correspondería en un Árbol Binario de Búsqueda.
2. Revisar el balance del árbol comenzando desde el nodo insertado hacia la raíz.
3. Aplicar rotaciones simples o dobles según sea necesario para asegurar que cada nodo cumpla con la propiedad de balance AVL.

#### Algoritmo de Búsqueda
1.Comenzar en la raíz y comparar el valor buscado con el valor del nodo.
2.Descender hacia la izquierda o derecha según si el valor es menor o mayor que el del nodo.
3.Repetir hasta encontrar el valor o confirmar que el valor no está en el árbol.

#### Algoritmo de Eliminación
1. Buscar Nodo: Localizar el nodo que se desea eliminar siguiendo el método estándar de búsqueda en árboles binarios de búsqueda.
2. Eliminar Nodo: Dependiendo de la estructura del nodo (sin hijos, con un hijo, con dos hijos), proceder a su eliminación.
        - Sin hijos: Simplemente eliminar el nodo.
        -Un hijo: Eliminar el nodo y conectar su hijo directamente al padre del nodo eliminado.
        -Dos hijos: Encontrar el sucesor inmediato del nodo (el más pequeño en el subárbol derecho), copiar sus datos en el nodo a eliminar y luego eliminar el sucesor.
3. Rebalancear: Recorrer el camino de vuelta al nodo raíz, verificando y corrigiendo cualquier desbalance mediante rotaciones.

### Técnica 2: ISAM-Sparse Index / B+ Tree
ISAM (Indexed Sequential Access Method) es una técnica de indexación que utiliza un índice disperso para mejorar la eficiencia en la búsqueda de registros en archivos de datos secuenciales. La estructura ISAM mantiene los datos en un archivo secuencial, junto con un índice separado que almacena las claves y los punteros a los registros. Los índices ISAM son estáticos en cuanto a su estructura de árbol, pero permiten eficiencia en las búsquedas y son especialmente útiles en entornos donde las consultas son más frecuentes que las actualizaciones.

#### Algoritmo de Inserción:

Buscar la posición correcta en el archivo de datos utilizando el índice.
Si hay espacio en la página de datos, insertar el nuevo registro allí.
Si no hay espacio, se utiliza un área de desbordamiento donde se pueden insertar registros adicionales.

#### Algoritmo de Eliminación:

Localizar el registro usando el índice.
Marcar el registro como eliminado; los registros no se eliminan físicamente de inmediato.
Reorganizar o compactar periódicamente el archivo y el área de desbordamiento para optimizar el espacio y el rendimiento.

#### Algoritmo de Búsqueda:

Utilizar la clave de búsqueda para localizar el rango en el índice.
Acceder al archivo de datos directamente a través del puntero obtenido del índice.
Si el registro no está en la página principal, buscar en el área de desbordamiento.

![image](https://github.com/marcelochincha/BD2-proyecto1/assets/130480550/1e117ebf-174d-489e-bcd8-07d0ae3fbfaf)


Diagrama ilustrativo de la estructura de ISAM-Sparse Index que muestra cómo se relaciona el archivo de datos secuencial con el índice disperso y el área de desbordamiento

### Técnica 3: Extendible Hashing

Extendible Hashing es una estructura de datos dinámica que se utiliza para manejar las inserciones y búsquedas eficientes en una base de datos, especialmente cuando el tamaño del conjunto de datos puede cambiar con el tiempo. Esta técnica emplea un directorio de punteros a 'buckets' donde los datos están almacenados. La clave de su adaptabilidad es la capacidad de dividir los 'buckets' que se llenan, aumentando selectivamente la profundidad local de los 'buckets' afectados sin rehashing todos los datos.

#### Algoritmo de Inserción:

Calcular el hash de la clave para determinar el bucket correspondiente.
Si el bucket no está lleno, insertar el registro.
Si el bucket está lleno, dividir el bucket:
Incrementar la profundidad local del bucket.
Redistribuir las entradas según el nuevo hash.
Si es necesario, duplicar el tamaño del directorio.

#### Algoritmo de Eliminación:

Calcular el hash de la clave para encontrar el bucket.
Eliminar el registro del bucket.
Si el bucket está vacío, considerar la posibilidad de combinarlo con un bucket hermano y disminuir el tamaño del directorio.


#### Algoritmo de Búsqueda:

Calcular el hash de la clave.
Seguir el puntero del directorio al bucket correspondiente.
Buscar en el bucket el registro deseado.

![image](https://github.com/marcelochincha/BD2-proyecto1/assets/130480550/f3bff377-fcfe-4975-9160-5ca733613d25)


Diagrama ilustrativo de la estructura de Extendible Hashing, que muestra cómo funciona el directorio, los buckets, y la división y expansión de estos al llenarse

## Análisis Comparativo Teórico

- Comparación teórica de las técnicas implementadas basándose en accesos a memoria secundaria para inserción, búsqueda y eliminación.
- Optimización de código en el manejo de memoria secundaria.

## Resultados Experimentales

- Cuadros y/o gráficos comparativos del desempeño de las técnicas sobre el dominio de datos.
- Métricas evaluadas: total de accesos a disco duro (read & write) y tiempo de ejecución en milisegundos.
- Discusión y análisis de los resultados obtenidos.

## Pruebas de Uso y Presentación

- Descripción de las pruebas de uso realizadas para validar la funcionalidad del aplicativo.
- Capturas de pantalla de la interfaz gráfica.
- Enlace al video demostrativo (incluir el enlace directo al video).

## Parser SQL

- Explicación del proceso de implementación del parser de SQL.
- Ejemplos de sentencias SQL manejadas y cómo se procesan en el sistema.

## Conclusión

- Resumen de los hallazgos más importantes del proyecto.
- Reflexiones finales y posibles mejoras o continuaciones del proyecto.
