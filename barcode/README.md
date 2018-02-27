Persistent homology barcode
============================
The topological object generated from our `C++` has a overlapping parameter and the shape of a topological object depends on this overlapping value. In order to avoid this parametric value by making it constant, we used persistent homology barcode technique. It is trivial to identify stable overlapping value from barcode. We generated barcode in `Java` using an opensource library [javaplex-4.3.0.jar](https://github.com/appliedtopology/javaplex/releases/tag/4.3.0).

### Input
We generated the input from our `C++` code. In the `param.config` file there has a constant named `PRINT_BARCODE` which is by default `false`. By making it `true`, we enable our code to identify persistent overlappign value. Currently we have two options to identify this overlapping value: 1) Using Gudhi persistent homology package 2) Using barcode image.


To generate the simplex timeline code, we have to enable the flag named `PH_JAVA_PLEX` in the `param.config` file. After compile and run the code, it will generate simplex code and we have to dump the code segment here in `Barcode.java` file.

```Java
public void GetStreamForData(ExplicitSimplexStream stream) {
	// Place the barcode simplex timeline code generated by the C++ code
}
```

##### One can change the barcode file name using fillowing code snap in the `main` method of the `Barcode.java` file.
```Java
edu.stanford.math.plex4.api.Plex4.createBarcodePlot(intervals_index, "[FILE Name PREFIX]", 50);
```

### Compile
To compile and generate the barcode image:
1. Open terminal
2. cd /folder/location/of/your/Barcode.java file
3. Place `javaplex-4.3.0.jar` file here
4. Run following command
```C
$ cd /folder/location/of/your/Barcode.java
$ javac -cp .:javaplex-4.3.0.jar Barcode.java -d .
$ java -cp .:javaplex-4.3.0.jar barcode.Barcode
```

### Output
The output is a barcode image. The file name with suffix `X` means the barcode image generated for dimension `X`, where `X` could be any integer value starting from `0`.