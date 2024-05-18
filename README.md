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
- Descripción de la técnica.
- Algoritmo de inserción, eliminación y búsqueda.
- Diagramas o gráficos ilustrativos.
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
