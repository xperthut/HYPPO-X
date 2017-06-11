Persistent homology
====================
The topological object generated from our `C++` has a overlapping parameter and the shape of a topological object depends on this overlapping value. In order to avoid this parametric value by making it constant, we used persistent homology technique. We implemented the code in `Java` using opensource library [javaplex-4.3.0.jar](https://github.com/appliedtopology/javaplex/releases/tag/4.3.0).

### Input
We generated the input from our `C++` code. In the `config.h` file there has a constant named `PRINT_BARCODE` which is by default `false`. By making it `true`, we can generate the codes used for persistent homology. The code is responsible for creating barcode.

### Output
The output of is a barcode image. The barcode specifies the overlapping value after which the shape of the topological object will not change.