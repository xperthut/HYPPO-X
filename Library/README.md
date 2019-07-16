<img src="https://github.com/xperthut/HYPPO-X/blob/master/images/logo_hypen.png" width="30%" style="margin:0" />

This is a header only library built in C++ to extract the insight of high dimensional complex dataset using algebraic topology. It is an instance of Mapper framework.`

## Compiling
The library uses `c++14` and requires [Boost](http://www.boost.org/) with version 1.48.0 or more recent. It is a multi-platform library and compiles on Linux, Mac OSX and Visual Studio 2015.

## Install from terminal
```R
# To install run the following command
sudo make install

# To uninstall run the following command
sudo make uninstall
```

## Integration with your code base
```R
# Add the header
#include <hyppox.h>

# Create instance of Hyppox class like below
hyppox::HInterface* _hyppox = new hyppox::Hyppox();

# Call the following function and pass the argument list as parameters
# This function will generate JSON file as output in the directory which you 
# have set as -WD parameter. If you don't set it then it will create the file 
# in the source directory
_hyppox->getD3GraphObject(argc, argv);

# Finally remove the object from memory
delete _hyppox;
```
## See parameter list
```R
If your executable file is `xyz` then simply run the following command:
./xyz -h
```
