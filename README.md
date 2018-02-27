HYPPO-X (Version 1.18.0)
=========================

This is a hypothesis extraction tool from high dimensional phenomix dataset. This tool is mainly built in `C++`. The object that we generate from our method is a graph and we use [D3](https://d3js.org/) to visualize our object.

## Demo
Our demo is available at [http://mhmethun.com/Hyppo-X/demo](http://mhmethun.com/Hyppo-X/demo)

## Compiling
The library uses c++11 and requires [Boost](http://www.boost.org/) with version 1.48.0 or more recent. It is a multi-platform library and compiles on Linux, Mac OSX and Visual Studio 2015.

## Run from terminal
```R
# Add boost include and lib folder path at Makefile
cd \path\to\the\folder
make all
./main

# to clean all object files
make clean
```

## Data file
We used two datafiles named input.csv and input_B.csv. `input.csv` file contains datapoints for both locations (Kansas, Nebraska) and both genotypes (A and B), whereas `input_B.csv` file contains datapoints for both locations of genotype `B` only.

## Change settings
We have a file named `param.config`, which contains all the configurable variables. We can configure our code using these variables.

### Old version
Our old version code is [here](https://github.com/xperthut/HYPPO-X/tree/V1.17/V1.17).