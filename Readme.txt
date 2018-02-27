
This software is written by Methun Kamruzzaman (methun@eecs.wsu.edu).
It is under GPL 3.0 license.

This software is used to generate topological object from high dimensional phenomics dataset. We also generate subpopulation in the form of interesting paths.

Simulator Options:
------------------
Details instruction of installation and simulation is given in git readme file.

Class details:
-------------
cluster.cpp: This class contains methods used in clustering process.
clusterInformation.cpp: This class stores cluster related information.
config.cpp: Configuration file. This file sets the variable after reading the corresponding values from param.config.
customCollection.cpp: Custom list, set, unordered_set, unordered_map, map class.
dataPoint.cpp: A datapoint is the cooridinate point where coordinate dectates by a filter.
dbScan.cpp: DBScan clustering related functionalities.
fileHandler.cpp: Methods for reading/writing contents from/to a file.
graph.cpp: Graph related methods including constructing a graph, search interesting paths, print graphs as json file, etc
individual.cpp: Holds an individual's information
interface.h: Under construction
library.cpp: Under construction
main.cpp: Main file, starts mapper from here
mapper.cpp: This class starts the mapper construction process and interesting path generation process.
param.config: Configuration file. Change configuration for required changes.
phenotype.cpp: Store a phenotypic information
quadNode.cpp: Node of a quad tree
quadTree.cpp: Quad tree construction
quickSort.cpp: Implementation of quick sort
RGBColor.cpp: This class used to generate RGB color.
simplicialComplex.cpp: This class used for persistent homology.
unionFindWithPathCompression.cpp: This class is used to find all connected components using union-find process.

History
-------
Version 2.1.0 (released 2/27/2018)
- Initial release
