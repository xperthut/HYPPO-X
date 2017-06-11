GxE-TDA
=======

This is a high dimensional data visualization tool. This tool is mainly built in `c++`. The object that we generated from our method is a graph and we used `igraph` package of `R` to visually represent the graph. We also use `HTML canvas` to represent the graph more meaningfully using pie chart and colors.

## Contents
- [TDA](#tda)
- [Single filter function](#single-filter-function)
- [Double filter function](#double-filter-function)
- [Persistent homology](#persistent-homology)
- [Graph plotting](#graph-plotting)
- [Visual representation](#visual-representation)


## TDA
Topological data ananlysis (TDA) uses [algebraic topology](https://en.wikipedia.org/wiki/Algebraic_topology) (a branch of mathematics) which deals with point clouds and generate the shape of point cloud data in a coordinateless fashion. There has three steps through which TDA generates the shape of a dataset and these are:

- [Filtering](#filtering)
- [Partial clustering](#partial-clustering)
- [Topological object construction](#topological-object-construction)


## Filtering
The filter is a reference to represent any attribute of a point cloud. For instance, the attribute we want to measure of our dataset is the plant phenotype (growth rate) and we can represent it by any other referene like time. If we represent the phenotypic measurement of plants using time as a single filter function then the interpretation will be, how the plant phenotypic value changes with the change of time. We also can use multiple filters. For instance, in our experiment, we used both time and environmental attribute humidity as two filter function and the interpretation is, how the plant phenotype changes with respect to the change of both time and humidity. We have option to use more than two filter function but in this release, we used only [Single filter function](#single_filter_function) and [Double filter function](#double_filter_function).


## Single filter function
We used time as a single filter function and the time value available in our dataset is `days after planting (DAP)`. We also used environmental attribute `humidity`, `temperature` and `solar radiation` at a time as a single filter function. We implemented the code of single filter function using `C++`.


## Double filter function
Our dataset is temporal, because of this, we used DAP as one of the two filters in double filter function. The environmental attribute humidity is used as the other filter. We also built topological object using DAP and temperature as a double filter function and DAP and solar radiation as a double filter function. We implemented the code of double filter function using `C++`.


## Partial clustering
Clustering is used to group all similar phenotypic individuals. We created buckets along each filter attribute and apply clustering mechanism to the points in a bucket. We used density based clustering and in our experiment, we implemented [DBSCAN](https://en.wikipedia.org/wiki/DBSCAN) in `C++`. The overlapping buckets generate share points between two clusters.


## Topological object construction
We treated each cluster as a node and if two clusters have overlapping points then we consider an edge between two nodes corresponding these two clusters. In this fashion, we build our topological object which is a graph and save it in a `gml` formatted file.


## Persistent homology
The shape of a topological object changes by changing the bucket overlapping value. We used persistent homology to stabilize the shape of a topological object by making the overlapping value constant. The barcode is used to indicate the stabilize situation and the choice of overlap value for this stable object. The barcode generation code for persistent homology is writen in `JAVA` using opensource library [javaplex-4.3.0.jar](https://github.com/appliedtopology/javaplex/releases/tag/4.3.0).


## Graph plotting
We implemented the topological object construction process using both `single filter function` and `double filter function` in `C++` and saved the object (graph) in a `gml` formatted file. In order to visualize the graph, we used `igraph` package of `R` to read the saved `gml` formatted file and visualize in a `XQuartz` panel. In this panel, we can easily rearrange the graph orientation by changing node position using mouse pointer. The rearranged graph is saved in a `eps` formatted file, which we converted into a `png` formatted file to use in `HTML canvas`.

## Visual representation
We used different color and pie chart to represent the graph more meaningfully. To do this, we used `HTML canvas` and the data source is a JSON formated data, which is generated from our main `topological object construction` code that implemented in `C++`.
