
#**************************************************************************************************
# Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
# The generated code is released under following licenses:
# GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
# --------------------------------------------------------------------------------------------------
# File name: double_filter_dap_hum.R
# Objective: View topological object for double filter function {DAP,Humidity}
# Additional information: NA
# --------------------------------------------------------------------------------------------------
# Contributors                   Date            Task details
# -------------------------    ----------      --------------------
# Md. Kamruzzaman              03/14/2017      Initial version
# 
# **************************************************************************************************/

g = read.graph(file.choose(),"gml")
id = tkplot(g, canvas.width = 1455, canvas.height = 777)

# Load data/gxe_hum_dap.csv
coord=as.matrix(read.csv(file.choose()))
coord=coord[,-1]
tk_set_coords(id, coord)