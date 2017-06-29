R
==

`R` is a software package which is very handy for plotting chart and graph. We used [igraph](http://igraph.org/r/) package of R to visualize the graph. Here is the details instruction of how to draw a graph

### Install or load required library
```R
if(require(tidyr)==F){
  install.packages("tidyr")
}
if(require(dplyr)==F){
  install.packages("dplyr")
}
if(require(readr)==F){
  install.packages("readr")
}
if(require(lubridate)==F){
  install.packages("lubridate")
}
if(require(magrittr)==F){
  install.packages("magrittr")
}
if(require(ggplot2)==F){
  install.packages("ggplot2")
}
if(require(lme4)==F){
  install.packages("lme4")
}
if(require(graphics)==F){
  install.packages("graphics")
}
if(require(drc)==F){
  install.packages("drc")
}
if(require(scales)==F){
  install.packages("scales")
}
if(require(stats)==F){
  install.packages("stats")
}
if(require(readr)==F){
  install.packages("readr")
}
if(require(pheatmap)==F){
  install.packages("pheatmap")
}
if(require(igraph)==F){
  install.packages("igraph")
}
```

### Load gml formatted graph
```R
load(igraph)
g = read.graph(file.choose(),"gml")
```

### Plot graph
```R
id = tkplot(g, canvas.width = 1455, canvas.height = 777)
```

### Get node coordinates of graph
```R
coord = tk_coords(id)
```

### Convert node coordinate to html adjustable coordinate and save the coordinates in a csv file, i.e. test_coord.csv
```R
graph_coordinate_to_html_coordinate(g,coord,"test_coord.csv","[absolute path of this file]/")
```

### Save the graph image in a `eps` formatted file.