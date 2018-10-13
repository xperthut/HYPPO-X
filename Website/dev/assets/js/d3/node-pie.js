var DEFAULT_OPTIONS = {
    radius: 20,
    outerStrokeWidth: 10,
    parentNodeColor: 'blue',
    showPieChartBorder: false,
    pieChartBorderColor: 'white',
    pieChartBorderWidth: '2',
    showLabelText: false,
    labelText: 'text',
    labelColor: 'blue'
};

function getOptionOrDefault(key, options, defaultOptions) {
    defaultOptions = defaultOptions || DEFAULT_OPTIONS;
    if (options && key in options) {
        return options[key];
    }
    return defaultOptions[key];
}

function drawParentCircle(nodeElement, options) {
    var outerStrokeWidth = getOptionOrDefault('outerStrokeWidth', options);
    //var radius = getOptionOrDefault('radius', options);
    var parentNodeColor = getOptionOrDefault('parentNodeColor', options);

    nodeElement.insert("circle")
<<<<<<< HEAD
        .attr("id", "parent-pie")
        .attr("r", function (d) {
            return d.Size;
        })
        .attr("fill", parentNodeColor)
        .attr("stroke", parentNodeColor)
        .attr("stroke-width", outerStrokeWidth);
=======
            .attr("id", "parent-pie")
            .attr("r", function (d) {
                return d.Size;
            })
            .attr("fill", parentNodeColor)
            .attr("stroke", parentNodeColor)
            .attr("stroke-width", outerStrokeWidth);
>>>>>>> template
}

function drawPieChartBorder(nodeElement, options) {
    //var radius = getOptionOrDefault('radius', options);
    var pieChartBorderColor = getOptionOrDefault('pieChartBorderColor', options);
    var pieChartBorderWidth = getOptionOrDefault('pieChartBorderWidth', options);

    nodeElement.insert("circle")
<<<<<<< HEAD
        .attr("r", function (d) {
            return d.Size;
        })
        .attr("fill", 'transparent')
        .attr("stroke", pieChartBorderColor)
        .attr("stroke-width", pieChartBorderWidth);
=======
            .attr("r", function (d) {
                return d.Size;
            })
            .attr("fill", 'transparent')
            .attr("stroke", pieChartBorderColor)
            .attr("stroke-width", pieChartBorderWidth);
>>>>>>> template
}

function drawPieChart(nodeElement, percentages, options) {
    var percentToDraw = 0;
    for (var p in percentages) {
        percentToDraw += percentages[p].percent;

        nodeElement.insert('circle', '#parent-pie + *')
<<<<<<< HEAD
            .attr("r", function (d) {
                return d.Size/2;
            })
            .attr("fill", 'transparent')
            .style('stroke', percentages[p].color)
            .style('stroke-width', function (d) {
                return d.Size;
            })
            .style('stroke-dasharray', function (d) {
                var halfCircumference = 2 * Math.PI * (d.Size)/2;;
                return halfCircumference * percentToDraw / 100
                    + ' '
                    + halfCircumference;
            })      
=======
                .attr("r", function (d) {
                    return d.Size / 2;
                })
                .attr("fill", 'transparent')
                .style('stroke', percentages[p].color)
                .style('stroke-width', function (d) {
                    return d.Size;
                })
                .style('stroke-dasharray', function (d) {
                    var halfCircumference = 2 * Math.PI * (d.Size) / 2;
                    ;
                    return halfCircumference * percentToDraw / 100
                            + ' '
                            + halfCircumference;
                })
>>>>>>> template
    }
}

function drawTitleText(nodeElement, options) {
    //var radius = getOptionOrDefault('radius', options);
    var text = getOptionOrDefault('labelText', options);
    var color = getOptionOrDefault('labelColor', options);

    nodeElement.append("text")
<<<<<<< HEAD
        .text(String(text))
        .attr("fill", color)
        .attr("dy", function (d) {
                return d.Size*2;
=======
            .text(String(text))
            .attr("fill", color)
            .attr("dy", function (d) {
                return d.Size * 2;
>>>>>>> template
            });
}

var NodePieBuilder = {
    drawNodePie: function (nodeElement, percentages, options) {
        drawParentCircle(nodeElement, options);

        if (!percentages)
            return;
        drawPieChart(nodeElement, percentages, options);

        var showPieChartBorder = getOptionOrDefault('showPieChartBorder', options);
        if (showPieChartBorder) {
            drawPieChartBorder(nodeElement, options);
        }

        var showLabelText = getOptionOrDefault('showLabelText', options);
        if (showLabelText) {
            drawTitleText(nodeElement, options);
        }
    }
};