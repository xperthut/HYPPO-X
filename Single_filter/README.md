Topological object construction using single filter function
============================================================

We implemented the process of generating a `topological object` using `single filter function` in `C++` [see [Change settings](#change-settings) to make custom changes in the setting. Also see [Build object](#build-object)].
The source codes are in the `src` folder and the test dataset `(gxe_pat.csv)` is in the `data` folder.

## Filter functions
Based on our dataset, we can use following attribute as a filter function.
1. `DAP`
2. `Humidity`
3. `Temperature`
4. `Solar radiation`

## Input
The input csv file contains following columns. One can change the column name and the value but can't expand the columns or change the type of the value. i.e. `Time` column contains integer values like `1,2,...` which does not allow any text here.
1. `Time, integer type. i.e. Days After Planting (DAP)`
2. `Genotype, string/text type`
3. `Location, string/text type`
4. `Individual id, see in` [Individual id](#individual-id)
5. `Date, string/text type`
6. `Phenotypic value, float/real type`
7. `Environmental attribute 1, float/real type, i.e. temperature`
8. `Environmental attribute 2, float/real type, i.e. solar radiation`
9. `Environmental attribute 3, float/real type, i.e. humidity`

## Output
Output is a `gml` formatted file with prefix `graph_COMPOSITE`.

## Individual id
The default value is an integer. If there have multiple individuals under same `genotype` and `location` combination then one can place it by ` # ` separator, i.e. `1 # 3 # 4`.

## Configuration file
The file named `config.h` is responsible for all types of configuration. One can change the value of the configuration file to make change in the code.

## Change settings
The `config.h` is the setting page. Necessary settings are here: 

Change filter function using following constant. Our program accepts four values `0-3`.
```cpp
#define FILTER_1 VALUE
```

Set the link of your source data file. Currently, our program accepts only comma `(,)` separated `csv` formatted data file.
```cpp
// Specify full path of the data file
#define DATA_FILE_NAME "FILENAME.csv"
```

Set the number of windows along the filter function using the following constant.
```cpp
#define WINDOW_X VALUE
``` 

Set the cluster radius. It accepts `real/float` value.
```cpp
#define CLUSTER_RADIUS VALUE
```

Set the overlap value. It accepts `real/float` value.
```cpp
#define OVERLAP VALUE
```

Enable flag to print simplex timeline statements. Copy these statements to our barcode generated `Barcode.java` file to generate barcode image.
```cpp
#define PRINT_BARCODE true
```

Enable flag to print `Javascript` code which is used to create `pie chart` and coloring nodes based on different measuring attribute `i.e. time, phenotype, environment`.
```cpp
#define PIE_CHART_CODE true
```

Adjust the node size of the generated topological object using following constants.
```cpp
#define NODE_SIZE_MAX MAX_SIZE
#define NODE_SIZE_MIN MIN_SIZE
```

## Build object
1. `Configure [DATA_FILE_NAME]`
2. `Configure [FILTER_1]`
3. `Configure [WINDOW_X]`
4. `Configure [CLUSTER_RADIUS]`
5. `Set barcode flag [PRINT_BARCODE]`
6. `Run the program to generate simplex timeline in Java. For instance, you will get something like this:`
```java
/************ Start from here *************/
stream.addVertex(1, 0.000);
stream.addVertex(2, 0.000);
...
stream.addElement(new int[]{53,54},  45.00);
/************ End here *************/
```
7. `Copy this code block and paste it in the Homology/src/Barcode.java file at following position:`
```java
public void GetStreamForData(ExplicitSimplexStream stream) {
   /*
        Add the simplex code generated from our main project writen in C++
    */
}
```
8. `Run Homology/src/Barcode.java file to generate barcode and identify the persistent value indicating along X axis` and configure [OVERLAP] with this value.
9. `Unset barcode flag [PRINT_BARCODE]`
10. `Run the program to generate the output which is a .gml formatted file with prefix 'graph_COMPOSITE'`
11. `Open this .gml file using R. The command is as below:`
```R
library(igraph)
g = read.graph(file.choose(),"gml")
id = tkplot(g, canvas.width = 1455, canvas.height = 777)
```
`The file will open as a graph in 'XQuartz' terminal where you can change the node position. After rearranging the graph nodes, run the following codes in R console to generate and save the list of coordinates of the nodes in a csv formatted file.`
```R
coord = tkplot.getcoords(id)
tk_set_coords(id, coord)
coord = tkplot.getcoords(id)
coord[,2]=coord[,2]+X // Here X can be replaced with any numerical value to adjust the graph's vertical position in XQuartz
coord = tkplot.getcoords(id)

// File name is any csv formated file
// File path is the absolute path of the above file
graph_coordinate_to_html_coordinate(g,coord,[file name],[file path]) 
```
`The tab 'view' has option to show or hide the labels in the graph. Hide all the labels and save image. The image will save in '.eps' format.` 


