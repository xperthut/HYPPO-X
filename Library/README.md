<img src="https://github.com/xperthut/HYPPO-X/blob/master/logo.png" width="30%" style="margin:0" />

This is a hypothesis extraction tool from high dimensional phenomix dataset. This tool is mainly built in `C++`. The object that we generate from our method is a graph and we use [D3](https://d3js.org/) to visualize our object.

## Compiling
The library uses `c++14` and requires [Boost](http://www.boost.org/) with version 1.48.0 or more recent. It is a multi-platform library and compiles on Linux, Mac OSX and Visual Studio 2015.

## Run from terminal
```R
# Add boost include and lib folder path at Makefile
cd \path\to\the\folder
make all
./hyppox

# to clean all object files
make clean
```

## To see the list of parameters
```R
./hyppox -help
```

