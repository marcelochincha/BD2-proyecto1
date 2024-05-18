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
- Descripción de la técnica.
- Algoritmo de inserción, eliminación y búsqueda.
- Diagramas o gráficos ilustrativos.

### Técnica 3: Extendible Hashing
- Descripción de la técnica.
- Algoritmo de inserción, eliminación y búsqueda.
- Diagramas o gráficos ilustrativos.

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
