Topological object construction using single filter function
============================================================

We implemented the process of generating a `topological object` using `single filter function` in `C++` [see [How to run the code](#How-to-run-the-code)].
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

## How to run the code
Step #1: One can change the filter function using following flag in `config.h` file. Our program accepts four values `0-3`.
```cpp
#define FILTER_1 VALUE
```

Step #2: Set the link of your source data file in the `config.h` file. Currently, our program accepts only comma `,` separated `csv` formatted data file.
```cpp
// Specify full path of the data file
#define DATA_FILE_NAME "FILENAME.csv"
```

Step #3: Set the number of windows along the filter function using the following constant in `config.h` file.
```cpp
#define WINDOW_X VALUE
``` 



