
#**************************************************************************************************
# Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
# The generated code is released under following licenses:
# GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
# --------------------------------------------------------------------------------------------------
# File name: get_node_coordinates.R
# Objective: Convert coordinates to fit for html canvas
# Additional information: NA
# --------------------------------------------------------------------------------------------------
# Contributors                   Date            Task details
# -------------------------    ----------      --------------------
# Md. Kamruzzaman              03/16/2017      Initial version
# 
# **************************************************************************************************/

## g is an igraph object
## coord is the node coordinate of graph g
## file_name is the the name of a csv file with extension .csv
## file_path is the the absolute path of the directory

graph_coordinate_to_html_coordinate <- function(g,coord,file_name,file_path){
	G <- g
	L <- coord
	vs <- V(G)
	es <- as.data.frame(get.edgelist(G))
	Nv <- length(vs)
	Ne <- length(es[1]$V1)
	Xn <- L[,1]
	Yn <- L[,2]

	v=c()
	for(i in 1:Ne) {
  		v0 <- es[i,]$V1
  		v1 <- es[i,]$V2
  
  		v=unique(c(v,v0,v1))
	}

	cd=data.frame(V=V(g)[v[1]]$id, x=L[v[1],1], y=L[v[1],2])
	for(i in 2:length(v)){
  		cd = rbind(cd, data.frame(V=V(g)[v[i]]$id, x=L[v[i],1], y=L[v[i],2]))
	}

	write.csv(cd, paste(file_path,file_name))
}
