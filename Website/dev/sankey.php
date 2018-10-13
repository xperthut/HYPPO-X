<?php

function getSankeyData(){
    $data = $_REQUEST['data'];
    /*$data = "{
        'nodes':[
        {'node':0,'name':'node0'},
        {'node':1,'name':'node1'},
        {'node':2,'name':'node2'},
        {'node':3,'name':'node3'},
        {'node':4,'name':'node4'}
        ],
        'links':[
        {'source':0,'target':2,'value':2},
        {'source':1,'target':2,'value':2},
        {'source':1,'target':3,'value':2},
        {'source':0,'target':4,'value':2},
        {'source':2,'target':3,'value':2},
        {'source':2,'target':4,'value':2},
        {'source':3,'target':4,'value':4}
        ]}";*/
    
    //$data = "{'nodes':[{'node':0,'name':'node11', 'NP':154},{'node':1,'name':'node17', 'NP':14},{'node':2,'name':'node15', 'NP':35},{'node':3,'name':'node13', 'NP':29},{'node':4,'name':'node16', 'NP':32}],'links':[{'source':0,'target':1,'value':4},{'source':0,'target':2,'value':10},{'source':0,'target':3,'value':9},{'source':0,'target':4,'value':10}]}";
    
    if(!isset($data)){
        echo '!!Bad URL!!';
        return "{}";
    }
    
    return $data;
}

?>

<!DOCTYPE html>
<meta charset="utf-8">
<title>SANKEY Experiment</title>
<style>

.node rect {
  cursor: move;
  fill-opacity: .9;
  shape-rendering: crispEdges;
}

.node text {
  pointer-events: none;
  text-shadow: 0 1px 0 #fff;
}

.link {
  fill: none;
  stroke: #000;
  stroke-opacity: .2;
}

.link:hover {
  stroke-opacity: .5;
}

</style>
<body>

<script src="https://d3js.org/d3.v4.min.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
<script src="assets/js/d3/sankey.js"></script>
<script>

var units = "Widgets";

// set the dimensions and margins of the graph
var margin = {top: 10, right: 10, bottom: 10, left: 10},
    width = 700 - margin.left - margin.right,
    height = 300 - margin.top - margin.bottom;

// format variables
var formatNumber = d3.format(",.0f"),    // zero decimal places
    format = function(d) { return formatNumber(d) + " " + units; },
    color = d3.scaleOrdinal(d3.schemeCategory20);

// append the svg object to the body of the page
var svg = d3.select("body").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
  .append("g")
    .attr("transform", 
          "translate(" + margin.left + "," + margin.top + ")");

// Set the sankey diagram properties
var sankey = d3.sankey()
    .nodeWidth(36)
    .nodePadding(40)
    .size([width, height]);

var path = sankey.link();

// load the data
//d3.json("sankey.json", function(error, graph) {
var graph = <?php echo getSankeyData();?>;
  sankey
      .nodes(graph.nodes)
      .links(graph.links)
      .layout(32);

// add in the links
  var link = svg.append("g").selectAll(".link")
      .data(graph.links)
    .enter().append("path")
      .attr("class", "link")
      .attr("d", path)
      .style("stroke-width", function(d) { return Math.max(1, d.dy); })
      .sort(function(a, b) { return b.dy - a.dy; });

// add the link titles
  link.append("title")
        .text(function(d) {
    		return d.source.name + " → " + 
                d.target.name + "\n" + format(d.value); });

// add in the nodes
  var node = svg.append("g").selectAll(".node")
      .data(graph.nodes)
    .enter().append("g")
      .attr("class", "node")
      .attr("transform", function(d) { 
		  return "translate(" + d.x + "," + d.y + ")"; })
      .call(d3.drag()
        .subject(function(d) {
          return d;
        })
        .on("start", function() {
          this.parentNode.appendChild(this);
        })
        .on("drag", dragmove));

// add the rectangles for the nodes
  node.append("rect")
      .attr("height", function(d) { return d.dy; })
      .attr("width", sankey.nodeWidth())
      .style("fill", function(d) { 
		  return d.color = color(d.name.replace(/ .*/, "")); })
      .style("stroke", function(d) { 
		  return d3.rgb(d.color).darker(2); })
    .append("title")
      .text(function(d) { 
		  return d.name + "\n" + format(d.value); });

// add in the title for the nodes
  node.append("text")
      .attr("x", -6)
      .attr("y", function(d) { return d.dy / 2; })
      .attr("dy", ".35em")
      .attr("text-anchor", "end")
      .attr("transform", null)
      .text(function(d) { return d.name; })
    .filter(function(d) { return d.x < width / 2; })
      .attr("x", 6 + sankey.nodeWidth())
      .attr("text-anchor", "start");

// the function for moving the nodes
  function dragmove(d) {
    d3.select(this)
      .attr("transform", 
            "translate(" 
               + d.x + "," 
               + (d.y = Math.max(
                  0, Math.min(height - d.dy, d3.event.y))
                 ) + ")");
    sankey.relayout();
    link.attr("d", path);
  }
//});

</script>

</body>