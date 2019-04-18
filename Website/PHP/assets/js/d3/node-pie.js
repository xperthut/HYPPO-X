var DEFAULT_OPTIONS = {
    radius: 20,
    outerStrokeWidth: 10,
    parentNodeColor: 'transparent',
    showPieChartBorder: false,
    pieChartBorderColor: 'dark grey',
    pieChartBorderWidth: '2',
    showLabelText: false,
    labelText: 'text',
    labelColor: 'blue',
    allowPattern: false
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
    var parentNodeColor = getOptionOrDefault('parentNodeColor', options);

    nodeElement.insert("circle")
            .attr("id", "parent-pie")
            .attr("r", function (d) {
                return d.Size;
            })
            .attr("fill", parentNodeColor)
            .attr("stroke", parentNodeColor)
            .attr("stroke-width", outerStrokeWidth);
}

function drawPieChartBorder(nodeElement, options) {
    var pieChartBorderColor = getOptionOrDefault('pieChartBorderColor', options);
    var pieChartBorderWidth = getOptionOrDefault('pieChartBorderWidth', options);

    nodeElement.insert("circle")
            .attr("r", function (d) {
                return d.Size;
            })
            .attr("fill", 'transparent')
            .attr("stroke", pieChartBorderColor)
            .attr("stroke-width", pieChartBorderWidth);

}

function drawPieChart(nodeElement, percentages, option) {
    var allowPattern = getOptionOrDefault('allowPattern', option);

    var percentToDraw = 0;
    var prevPercentToDraw = 0;
    for (var p in percentages) {
        percentToDraw += percentages[p].percent;
        
        if(p>0) prevPercentToDraw += percentages[p-1].percent;

        if (p > 0) {
            nodeElement.insert('circle', '#parent-pie + *')
                .attr("r", function (d) {
                    return d.Size / 2;
                })
                .attr("fill", 'transparent')
                .style('stroke', 'white')
                .style('stroke-width', function (d) {
                    return d.Size;
                })
                .style('stroke-dasharray', function (d) {
                    var halfCircumference = 2 * Math.PI * (d.Size) / 2;

                    return halfCircumference * prevPercentToDraw / 100
                            + ' '
                            + halfCircumference;
                });
        }
        nodeElement.insert('circle', '#parent-pie + *')
                .attr("r", function (d) {
                    return d.Size / 2;
                })
                .attr("fill", 'transparent')
                .style('stroke', (allowPattern===true)?percentages[p].Pattern:percentages[p].color)
                .style('stroke-width', function (d) {
                    return d.Size;
                })
                .style('stroke-dasharray', function (d) {
                    var halfCircumference = 2 * Math.PI * (d.Size) / 2;

                    return halfCircumference * percentToDraw / 100
                            + ' '
                            + halfCircumference;
                });

    }
}

function drawTitleText(nodeElement, options) {
    //var radius = getOptionOrDefault('radius', options);
    var text = getOptionOrDefault('labelText', options);
    var color = getOptionOrDefault('labelColor', options);

    nodeElement.append("text")
            .text(String(text))
            .attr("fill", color)
            .attr("dy", function (d) {
                return d.Size * 2;
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