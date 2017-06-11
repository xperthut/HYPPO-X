R
==

`R` is a software package which is very handy for plotting chart and graph. We used [igraph](http://igraph.org/r/) package of R to visualize the graph. Here is the details instruction of how to draw a graph

# Load gml formatted graph
```
load(igraph)
g = read.graph(file.choose(),"gml")
```

# Plot graph
```
id = tkplot(g, canvas.width = 1455, canvas.height = 777)
```

# Get node coordinates of graph
```
coord = tk_coords(id)
```

# Convert node coordinate to html adjustable coordinate and save the coordinates in a csv file, i.e. test_coord.csv
```
graph_coordinate_to_html_coordinate(g,coord,"test_coord.csv","[absolute path of this file]/")
```

# Save the graph image in a `eps` formatted file.