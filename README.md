# BD2-proyecto1: Organización de transacciones de compras en línea
Proyecto 1 de BD2

## Integrantes

**Cespedes Zevallos, Adrian Joshep**

**Chincha León, Marcelo Andres**

**Herencia Galvan, David Manfredo**

**Blanco Gutierrez, Gabriel Blanco**

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
Comenzar en la raíz y comparar el valor buscado con el valor del nodo.
Descender hacia la izquierda o derecha según si el valor es menor o mayor que el del nodo.
Repetir hasta encontrar el valor o confirmar que el valor no está en el árbol.

#### Algoritmo de Eliminación
Buscar Nodo: Localizar el nodo que se desea eliminar siguiendo el método estándar de búsqueda en árboles binarios de búsqueda.
Eliminar Nodo: Dependiendo de la estructura del nodo (sin hijos, con un hijo, con dos hijos), proceder a su eliminación.
        - Sin hijos: Simplemente eliminar el nodo.
        -Un hijo: Eliminar el nodo y conectar su hijo directamente al padre del nodo eliminado.
        -Dos hijos: Encontrar el sucesor inmediato del nodo (el más pequeño en el subárbol derecho), copiar sus datos en el nodo a eliminar y luego eliminar el sucesor.
Rebalancear: Recorrer el camino de vuelta al nodo raíz, verificando y corrigiendo cualquier desbalance mediante rotaciones.

### Técnica 2: ISAM-Sparse Index / B+ Tree
ISAM (Indexed Sequential Access Method) es una técnica de indexación que utiliza un índice disperso para mejorar la eficiencia en la búsqueda de registros en archivos de datos secuenciales, esta estructura ISAM mantiene los datos en un archivo secuencial, junto con un índice separado que almacena las claves y los punteros a los registros. Los índices ISAM son estáticos en cuanto a su estructura de árbol, pero permiten eficiencia en las búsquedas y son especialmente útiles en entornos donde las consultas son más frecuentes que las actualizaciones.

#### Algoritmo de Inserción:

Pasos:

- Buscar la posición correcta en el archivo de datos utilizando el índice.

- Si hay espacio en la página de datos, insertar el nuevo registro allí.

- Si no hay espacio, se utiliza un área de desbordamiento donde se pueden insertar registros adicionales.

#### Algoritmo de Eliminación:

Pasos:

- Localizar el registro usando el índice.
  
- Marcar el registro como eliminado; los registros no se eliminan físicamente de inmediato.
  
- Reorganizar o compactar periódicamente el archivo y el área de desbordamiento para optimizar el espacio y el rendimiento.

#### Algoritmo de Búsqueda:

Pasos:

- Utilizar la clave de búsqueda para localizar el rango en el índice.
  
- Acceder al archivo de datos directamente a través del puntero obtenido del índice.
  
- Si el registro no está en la página principal, buscar en el área de desbordamiento.


### Técnica 3: Extendible Hashing

Extendible Hashing es una estructura de datos dinámica que se utiliza para manejar las inserciones y búsquedas eficientes en una base de datos, especialmente cuando el tamaño del conjunto de datos puede cambiar con el tiempo. Esta técnica emplea un directorio de punteros a 'buckets' donde los datos están almacenados, la clave de su adaptabilidad es la capacidad de dividir los 'buckets' que se llenan, aumentando selectivamente la profundidad local de los 'buckets' afectados sin rehashing todos los datos.

#### Algoritmo de Inserción:

Pasos:

- Calcular el hash de la clave para determinar el bucket correspondiente.

- Si el bucket no está lleno, insertar el registro.

- Si el bucket está lleno, dividir el bucket:

- Incrementar la profundidad local del bucket.

- Redistribuir las entradas según el nuevo hash.

- Si es necesario, duplicar el tamaño del directorio.

#### Algoritmo de Eliminación:

Pasos:

- Calcular el hash de la clave para encontrar el bucket.
  
- Eliminar el registro del bucket.
  
- Si el bucket está vacío, considerar la posibilidad de combinarlo con un bucket hermano y disminuir el tamaño del directorio.


#### Algoritmo de Búsqueda:

Pasos:

- Calcular el hash de la clave.
  
- Seguir el puntero del directorio al bucket correspondiente.
  
- Buscar en el bucket el registro deseado.

## Análisis Comparativo Teórico

**k**= tamaño máximo de bucket

**m**=overflow de buckets

| Técnica         | Inserción (accesos a memoria) | Búsqueda (accesos a memoria) | Eliminación (accesos a memoria) |
|-----------------|-------------------------------|------------------------------|----------------------------------|
| ISAM            |  O(k*m)                       |        O(logk)               |           O(k*m)                 |
| Extendible Hash |   O(k*m)                      |       O(k*m)                 |           O(k*m)                 | 
| AVL             |   O(logk)                     |         O(logk)              |           O(logk)                |


## Resultados Experimentales

![image](https://github.com/marcelochincha/BD2-proyecto1/assets/130480550/7876190a-c920-4c19-9710-c8e3ddea1c94)


## Parser SQL

### Explicación del Proceso de Implementación

El parser SQL se implmenta en la clase database, la cual utiliza un método simple de tokenizacion (separar por espacios), los token obtenidos se cargan a un queue que permite extraer uno a uno los tokens y utilizar los pertinentes, de acuerdo a la operación requerida. No obstante, la implementación propuesta ignora por completo las keywords que no son necesarias para la acción, lo que podría conllevar a aceptar queries que no siguen la sintaxiss correcta de SQL. A pesar de esto, recurrimos a esta forma por questiones de complejidad y tiempo.

```cpp
    // Obtener nombre de la tabla
    std::string table_name = tokens.front();

    tokens.pop();
    tokens.pop();  // Remove FROM
    tokens.pop();  // Remove FILE

    // Obtener path del archivo
    std::string file_path = tokens.front();

    tokens.pop();
    tokens.pop();  // Remove USING
    tokens.pop();  // Remove INDEX

    // Obtener tipo de tabla
    std::string table_type = tokens.front();
    tokens.pop();

    // Crear tabla
    Table::mode type;
    Table* tNEW;
    if (table_type == "ISAM") {
        type = Table::mode::isam;
        tNEW = new Table(type, table_name);
    } else {
        return "Invalid table type";
    }
```

Parte de código de creación de tabla.

Comandos Disponibles:
```Mysql
CREATE TABLE datitos FROM FILE dataA.csv using index AVL
SELECT * FROM datitos WHERE CustomerID = 109318
SELECT * FROM datitos WHERE CustomerID BETWEEN 109000 AND 109888
INSERT INTO table_name VALUES (...)
DELETE FROM datitos WHERE CustomerID = 109346
```

## Conclusión

#### Resumen de Hallazgos
El proyecto ha demostrado que la elección de una estructura de datos adecuada puede tener un impacto significativo en el rendimiento de un sistema de gestión de bases de datos, especialmente en aplicaciones con un volumen alto de transacciones como las ventas minoristas en línea. A través del uso de tres técnicas de organización de archivos — AVL Files, ISAM, y Extendible Hashing — se logró abordar diferentes aspectos del manejo de datos:

AVL Files proveyeron un acceso balanceado y eficiente, ideal para operaciones que requieren una gran cantidad de inserciones y eliminaciones, mientras que,ISAM fue útil por su eficiencia en las búsquedas, especialmente en entornos donde las consultas son más frecuentes que las actualizaciones. Además el Extendible Hashing destacó en entornos dinámicos donde el tamaño de los datos puede cambiar significativamente, permitiendo una escalabilidad eficiente sin rehashing costoso.
Gracias a los resultados experimentales validaron la teoría de que cada técnica tiene sus fortalezas dependiendo de la naturaleza de las operaciones y los requisitos del sistema, como se reflejó en la variación de los tiempos de operación para las diferentes técnicas.

#### Reflexiones Finales y Mejoras Futuras
El análisis realizado sugiere que no existe una única solución óptima para todos los escenarios en la gestión de bases de datos. Cada técnica tiene particularidades que la hacen más adecuada para ciertos tipos de operaciones y menos para otras. Por lo tanto, la elección debe basarse en una comprensión detallada de los patrones de acceso a los datos y los requisitos específicos del sistema.

#### Posibles Mejoras:

- Implementación de Cachés: Implementar mecanismos de caché inteligentes podría reducir significativamente los tiempos de acceso a datos, especialmente para ISAM y Extendible Hashing donde las operaciones pueden beneficiarse de un rápido acceso a los datos recientemente utilizados.
  
- Optimización de Algoritmos de Balanceo en AVL: Mejorar los algoritmos de rotación y balanceo en los AVL Files podría ofrecer mejoras en el rendimiento de inserción y eliminación.

- Optimización de "Range_search" en ISAM: Mejorar el algoritmo de búsqueda por rango en la estructura del ISAM, para que de esta manera sea mucho más efectivo, ya que el algoritmo actual utiliza una función sencilla pero eficiente
  
- Híbridos de Estructuras de Datos: Experimentar con estructuras híbridas que combinen las fortalezas de los diferentes métodos puede ofrecer un equilibrio entre eficiencia en la inserción, eliminación y búsqueda.
  
- Ajustes Dinámicos en Extendible Hashing: Optimizar el proceso de división y fusión de buckets para adaptarse mejor a los patrones cambiantes de carga de datos.
