
/* global d3, NodePieBuilder */

$(function () {
    // Hash map class
    function HashMap() {
        this._dict = [];
    }

    HashMap.prototype = {
        _get: function (key) {
            for (var i = 0, couplet; i < this._dict.length; i++) {
                couplet = this._dict[i];
                if (couplet[0] === key) {
                    return couplet;
                }
            }

            return null;
        },
        put: function (key, value) {
            var couplet = this._get(key);
            if (couplet) {
                couplet[1] = value;
            } else {
                this._dict.push([key, value]);
            }
            return this; // for chaining
        },
        get: function (key) {
            var couplet = this._get(key);
            if (couplet) {
                return couplet[1];
            }
            return null;
        },
        getKeys: function () {
            var keys = [];
            for (var i = 0, couplet; i < this._dict.length; i++) {
                couplet = this._dict[i];
                keys.push(couplet[0]);
            }

            return keys;
        }
    };

// Graph management class
    function Graph(_fl, _jfl) {
        this.fl = _fl;
        this.jfl = _jfl;
        this.svg = d3.select("svg").on("click", function () {
            $("#top-nav").css("display", "none");
            $("#tooltip").css("display", "none");
        });
        this.width = +this.svg.attr("width");
        this.height = +this.svg.attr("height");
        this.link = null;
        this.node = null;
        this.labelText = "";
        this.coordData = null;
        this.labelIndex = 0;
        this._linkData = null;
        this._nodeData = null;
        this.edgepaths = null;
        this.edgelabels = null;
        this.intPathRank = [];
        this.totalpaths = 0;
        this.totalFlares = 0;
        this.headerNames = null;
        this.IPColors = null;
        this.IFColors = null;
        this.hideIPRank = [];
        this.hideIFRank = [];
        this.graph = null;
        this._graph = null;
        this.intFlareRank = [];
        this.__transform = null;
        this.hasPieChart = true;
        this.EdgeDirChg = false; //For pie
        this._analysis = [];
        this.sankey = null;
        this.path = null;
        this.slink = null;
        this.g = this.svg.append("g").attr("class", "everything");
        this.fileIndex = 0;
        this.fileRIndex = 0;
        this.fileCIndex = 0;
        this.dpie = false;
        this.lIndex = -1;
        this.shEdgeArrow = true;
        this.shEdgeSig = false;
        this.shEdgeRank = false;
        this.selectedFeature = [false, false];
        this.defaultEdgeColor = "#a4a4a4"; //a4a4a4
        this.IPEdgeWidth = 6;
        this.IFEdgeWidth = 6;
        this.csvData = null;
        this.zoom_handler = d3.zoom();
        this.marker = this.svg.append("defs");
        this.pallete = this.svg.append("defs");
        this.simulation = null;
        this.strength = -700;
        this.tooltipDiv = d3.select("#tooltip").append("div").style("color", "black");
        this.selectedNodeId = -1;
        this.showToolTip = false;
        this.svgBGColor = "#000";
        this.piePattern = false;
    }

    Graph.prototype = {
        constructor: Graph,

        zoom_actions: function () {
            $("#top-nav").css("display", "none");
            gInstance.__transform = d3.event.transform;
            gInstance.g.attr("transform", gInstance.__transform);
        },

        setDefault: function () {
            this.link = null;
            this.node = null;
            this.labelText = "";
            this.coordData = null;
            this.labelIndex = 0;
            this._linkData = null;
            this._nodeData = null;
            this.edgepaths = null;
            this.edgelabels = null;
            this.intPathRank = [];
            this.totalpaths = 0;
            this.totalFlares = 0;
            this.headerNames = null;
            this.IPColors = null;
            this.IFColors = null;
            this.hideIPRank = [];
            this.hideIFRank = [];
            this.graph = null;
            this._graph = null;
            this.intFlareRank = [];
            this.__transform = null;
            this.hasPieChart = true;
            this.EdgeDirChg = false; //For pie
            this._analysis = [];
            this.sankey = null;
            this.path = null;
            this.slink = null;
            this.dpie = false;
            this.lIndex = -1;
            this.shEdgeArrow = true;
            this.shEdgeSig = false;
            this.shEdgeRank = false;
            this.selectedFeature = [false, false];
            this.defaultEdgeColor = "#a4a4a4";
            this.IPEdgeWeight = 1;
            this.IFEdgeWeight = 1;
            this.csvData = null;
            this.simulation = null;
            this.strength = -700;
            d3.select("#tooltip div").remove();
            d3.select("#tooltip").style("display", "none");
            this.tooltipDiv = d3.select("#tooltip").append("div").style("color", "black");
            this.selectedNodeId = -1;
            this.showToolTip = false;
            this.piePattern = false;

            d3.select("#svg-container").style("background-color", this.svgBGColor);
            d3.select("#viewer").style("background-color", this.svgBGColor);
        },

        initPage: function (gData) {
            this.zoom_handler(this.svg);
            this.zoom_handler.scaleExtent([1 / 10, 10])
                    .on("zoom", gInstance.zoom_actions);

            $("#int-cc").css("display", "none");
            $("#int-path").css("display", "none");
            $("#int-flare").css("display", "none");
            $("#pie-legend").css("display", "none");

            this.setDefault();

            this.graph = gData;
            this._graph = gData;

            //this.defaultEdgeColor = "#a4a4a4";
            this.totalpaths = this.graph.IPC.length;
            this.totalFlares = this.graph.IFC.length;
            this.headerNames = this.graph.HN;
            this.IPColors = this.graph.IPC;
            this.IFColors = this.graph.IFC;

            this._nodeData = $.extend(true, [], this.graph.nodes);
            this._linkData = $.extend(true, [], this.graph.links);

            // Set flare color based on rank
            // Top rank will get priority
            for (var i = 0; i < this._linkData.length; i++) {
                if (this._linkData[i].FR[0] === 0) {
                    this._linkData[i].FC = gInstance.defaultEdgeColor;
                    this._linkData[i].FW = 2;
                } else {
                    this._linkData[i].FC = this.IFColors[this._linkData[i].FR[0] - 1];
                    this._linkData[i].FW = 12;
                }
            }

            this.createButtons();
            this.createAttributes();
            this.draw(0);
            this.createSubGraphLegends();
        },

        showHideSubGraph: function () {
            var _nid = [],
                    _show = $(this).prop("checked"),
                    c = parseInt($(this).attr("r"));

            this._nodeData = [];

            // Filter links and nodes
            if (_show === true) {

                // add missing link
                for (var i = 0; i < gInstance._graph.links.length; i++) {
                    if (gInstance._graph.links[i].CC === c) {
                        gInstance._linkData.push($.extend(true, {}, gInstance._graph.links[i]));
                    }
                }

            } else {
                // remove link
                var dc = [];
                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (gInstance._linkData[i].CC === c) {
                        dc.push(i);
                    }
                }

                for (var i = 0, j = 0; i < dc.length; i++, j++) {
                    gInstance._linkData.splice(dc[i] - j, 1);
                }
            }

            // Retrieve nodes for existing links only
            for (var i = 0; i < gInstance._linkData.length; i++) {
                if (_nid.indexOf((gInstance._linkData[i].source.Id) ? gInstance._linkData[i].source.Id : gInstance._linkData[i].source) === -1) {
                    _nid.push((gInstance._linkData[i].source.Id) ? gInstance._linkData[i].source.Id : gInstance._linkData[i].source);
                }
                if (_nid.indexOf((gInstance._linkData[i].target.Id) ? gInstance._linkData[i].target.Id : gInstance._linkData[i].target) === -1) {
                    _nid.push((gInstance._linkData[i].target.Id) ? gInstance._linkData[i].target.Id : gInstance._linkData[i].target);
                }
            }

            for (var i = 0; i < gInstance._graph.nodes.length; i++) {
                if (_nid.indexOf(gInstance._graph.nodes[i].Id) !== -1) {
                    gInstance._nodeData.push($.extend(true, {}, gInstance._graph.nodes[i]));
                }
            }

            if (gInstance.dpie)
                gInstance.drawPie();
            else
                gInstance.draw(gInstance.labelIndex);
        },

        createSubGraphLegends: function () {
            $("#int-cc").css("display", "block");

            $("#cc-details").html("");
            $("#cc-details").html("<fieldset><legend>Connected subgraphs&nbsp;</legend><ul class='cc_legend'></ul></fieldset>");

            var sg = [];
            for (var li = 0; li < this._linkData.length; li++) {
                if (sg.indexOf(this._linkData[li].CC) === -1) {
                    sg.push(this._linkData[li].CC);
                }
            }

            sg.sort(function (a, b) {
                return a - b;
            });

            var s = "";
            for (var i = 0; i < sg.length; i++) {
                s += "<li><input type='checkbox' r='" + sg[i] + "' name='cc' id='cc_" + sg[i] +
                        "' checked /><label class='fa' for='cc_" + sg[i] + "'>Subgraph-" + sg[i] + "</label></li>";
            }

            $(".cc_legend").html(s);

            for (var i = 0; i < sg.length; i++) {
                d3.select("#cc_" + sg[i]).on("change", gInstance.showHideSubGraph);
            }
        },

        selectButton: function (holder, e) {
            $("#" + holder + " button").removeClass("button_bc");
            $("#" + e.attr("id")).addClass("button_bc");
        },

        saveImage: function () {
            gInstance.selectButton("other-btn", $(this));

            var html = d3.select("svg")
                    .attr("version", 1.1)
                    .attr("xmlns", "http://www.w3.org/2000/svg")
                    .node().parentNode.innerHTML;
            var imgsrc = 'data:image/svg+xml;base64,' + btoa(html);

            var canvas = document.querySelector("canvas"),
                    context = canvas.getContext("2d");

            var image = new Image;
            image.onload = function () {
                context.clearRect(0, 0, canvas.width, canvas.height);
                context.drawImage(image, 0, 0);
                var canvasdata = canvas.toDataURL("image/png");

                d3.select("canvasdata_save").remove();
                var a = document.createElement("a");
                a.download = "sample_" + (new Date().getTime()) + ".png";
                a.href = canvasdata;
                a.id = "canvasdata_save";
                document.body.appendChild(a);

                a.click();
            };
            image.src = imgsrc;
        },

        changeIndvcolor: function (e, col) {
            var c = parseInt($(e).attr("r"));

            gInstance._graph.color[c - 1] = col;

            gInstance.drawPie();
        },

        changeFlarecolor: function (e, col) {
            var c = parseInt($(e).attr("r"));

            gInstance._graph.IFC[c - 1] = col;

            for (var i = 0; i < gInstance._linkData.length; i++) {
                if (gInstance._linkData[i].FR[0] === c) {
                    gInstance._linkData[i].FC = gInstance._graph.IFC[c - 1];
                }
            }

            for (var i = 0; i < gInstance._graph.links.length; i++) {
                if (gInstance._graph.links[i].FR[0] === c) {
                    gInstance._graph.links[i].FC = gInstance._graph.IFC[c - 1];
                }
            }

            if (gInstance.dpie)
                gInstance.drawPie();
            else
                gInstance.draw(gInstance.labelIndex);
        },

        // Show only flare edges
        showIFs: function () {

            if ($(this).prop("checked") === true) {
                var activeFlares = 0;
                gInstance.hideIFRank = [];

                // Checked all interesting paths
                for (var i = 1; i <= gInstance.totalFlares; i++) {
                    if ($("#if_" + i).prop("checked") === true)
                        activeFlares++;
                    else
                        gInstance.hideIFRank.push(i);
                }

                if (activeFlares > 0) {
                    if (gInstance.hideIFRank.length > 0)
                        gInstance.hideIFRank.sort(function (a, b) {
                            return a - b;
                        });
                }


                // Checked all interesting paths
                /*for (var i = 0; i < gInstance.intFlareRank.length; i++) {
                 $("#if_" + gInstance.intFlareRank[i]).prop("checked", true);
                 }*/

                gInstance._nodeData = [];
                gInstance._linkData = [];

                var _nid = [];

                // Filter links and nodes

                // Copy links which has rank > 0
                for (var i = 0; i < gInstance._graph.links.length; i++) {
                    if (gInstance._graph.links[i].FR[0] > 0) {
                        gInstance._linkData.push($.extend(true, {}, gInstance._graph.links[i]));
                    }
                }

                // Remove ranks from link which are not selected
                if (gInstance.hideIFRank.length > 0) {
                    var thr = [];
                    for (var i = 0; i < gInstance._linkData.length; i++) {
                        for (var j = 0; j < gInstance.hideIFRank.length; j++) {
                            var k = gInstance._linkData[i].FR.indexOf(gInstance.hideIFRank[j]);
                            if (k >= 0) {
                                gInstance._linkData[i].FR.splice(k, 1);
                            }
                        }

                        if (gInstance._linkData[i].FR.length === 0)
                            thr.push(i);
                    }

                    // Remove link which has no FR
                    if (thr.length > 0) {
                        for (var i = 0; i < thr.length; i++) {
                            gInstance._linkData.splice(thr[i], 1);
                        }
                    }
                }

                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (_nid.indexOf((gInstance._graph.links[i].source.Id) ? gInstance._graph.links[i].source.Id : gInstance._graph.links[i].source) === -1) {
                        _nid.push((gInstance._graph.links[i].source.Id) ? gInstance._graph.links[i].source.Id : gInstance._graph.links[i].source);
                    }
                    if (_nid.indexOf((gInstance._graph.links[i].target.Id) ? gInstance._graph.links[i].target.Id : gInstance._graph.links[i].target) === -1) {
                        _nid.push((gInstance._graph.links[i].target.Id) ? gInstance._graph.links[i].target.Id : gInstance._graph.links[i].target);
                    }
                }

                for (var i = 0; i < gInstance._graph.nodes.length; i++) {
                    if (_nid.indexOf(gInstance._graph.nodes[i].Id) !== -1) {
                        gInstance._nodeData.push($.extend(true, {}, gInstance._graph.nodes[i]));
                    }
                }
            } else {
                gInstance._nodeData = $.extend(true, [], gInstance._graph.nodes);
                gInstance._linkData = $.extend(true, [], gInstance._graph.links);

                if (gInstance.hideIFRank.length > 0) {
                    for (var i = 0; i < gInstance._linkData.length; i++) {
                        if (gInstance._linkData[i].FR[0] > 0) {
                            for (var j = 0; j < gInstance.hideIFRank.length; j++) {
                                var a = gInstance._linkData[i].FR.indexOf(gInstance.hideIFRank[i]);
                                if (a >= 0)
                                    gInstance._linkData[i].FR.splice(a, 1);
                                if (gInstance._linkData[i].FR.length === 0)
                                    gInstance._linkData[i].FR.push(0);
                            }
                        }
                    }
                }

            }

            // Set flare color based on rank
            // Top rank will get priority
            for (var i = 0; i < gInstance._linkData.length; i++) {
                if (gInstance._linkData[i].FR.indexOf(0) >= 0) {
                    gInstance._linkData[i].FC = gInstance.defaultEdgeColor;
                    gInstance._linkData[i].FW = 2;
                } else {
                    gInstance._linkData[i].FC = gInstance.IFColors[gInstance._linkData[i].FR[0] - 1];
                    gInstance._linkData[i].FW = 12;
                }
            }

            if (gInstance.dpie)
                gInstance.drawPie();
            else
                gInstance.draw(gInstance.labelIndex);
        },

        showAllFlares: function () {

            var _allIntFlares = $("#saf").prop("checked");

            gInstance.selectedFeature[0] = gInstance.selectedFeature[1] = false;

            gInstance.hideIFRank = [];

            if (_allIntFlares === true) {
                gInstance.selectedFeature[1] = true;

                //if (gInstance.intFlareRank.length < gInstance.totalFlares) {
                // Filter links and nodes
                gInstance._nodeData = $.extend(true, [], gInstance.graph.nodes);
                gInstance._linkData = $.extend(true, [], gInstance.graph.links);
                //}

                // Checked all interesting paths
                for (var i = 1; i <= gInstance.totalFlares; i++) {
                    $("#if_" + i).prop("checked", true);
                }

                // Assign max ranked color
                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (gInstance._linkData[i].FR.indexOf(0) >= 0) {
                        gInstance._linkData[i].FC = gInstance.defaultEdgeColor;
                        gInstance._linkData[i].FW = 2;
                    } else {
                        gInstance._linkData[i].FC = gInstance.IFColors[gInstance._linkData[i].FR[0] - 1];
                        gInstance._linkData[i].FW = 12;
                    }
                }
            } else {
                // Checked all interesting paths
                for (var i = 1; i <= gInstance.totalFlares; i++) {
                    $("#if_" + i).prop("checked", false);
                    gInstance.hideIFRank.push(i);
                }

                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (gInstance._linkData[i].FR.indexOf(0) === -1) {
                        gInstance._linkData[i].FC = gInstance.defaultEdgeColor;
                        gInstance._linkData[i].FW = 2;
                    }
                }

                gInstance.hideIFRank.sort(function (a, b) {
                    return a - b;
                });
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showHideIntFlare: function () {
            var _nid = [],
                    _show = $(this).prop("checked"),
                    c = parseInt($(this).attr("r")),
                    _intFlare = $("#ifs").prop("checked"); // Show only interesting flares, no other edges; Also delete nodes which has normal edges only
            //_allFlares = $("#saf").prop("checked");

            if (_intFlare || _show) {
                gInstance.selectedFeature[0] = false;
                gInstance.selectedFeature[1] = true;
            }

            // Copy original links
            gInstance._linkData = $.extend(true, [], gInstance.graph.links);

            if (_intFlare === false) {

                if (_show === true) {
                    if (gInstance.hideIFRank.indexOf(c) >= 0)
                        gInstance.hideIFRank.splice(gInstance.hideIFRank.indexOf(c), 1);

                } else {
                    if (gInstance.hideIFRank.indexOf(c) === -1)
                        gInstance.hideIFRank.push(c);
                }

                var showIFRank = [];
                for (var i = 1; i <= gInstance.totalFlares; i++) {
                    if ($("#if_" + i).prop("checked") === true)
                        showIFRank.push(i);
                }

                showIFRank.sort(function (a, b) {
                    return a - b;
                });


                for (var i = 0; i < gInstance._linkData.length; i++) {
                    var m = false;
                    for (var j = 0; j < showIFRank.length; j++) {
                        var a = gInstance._linkData[i].FR.indexOf(showIFRank[j]);
                        if (a >= 0) {
                            gInstance._linkData[i].FC = gInstance.IFColors[showIFRank[j] - 1];
                            gInstance._linkData[i].FW = 12;

                            m = true;
                            break;
                        }
                    }

                    if (!m) {
                        gInstance._linkData[i].FC = gInstance.defaultEdgeColor;
                        gInstance._linkData[i].FW = 2;
                    }
                }
            } else {
                gInstance._nodeData = [];

                var nfs = [];
                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (gInstance._linkData[i].FR.indexOf(0) >= 0)
                        nfs.push(i);
                }

                if (_show === true) {

                    // Remove from hide list
                    if (gInstance.hideIFRank.indexOf(c) >= 0)
                        gInstance.hideIFRank.splice(gInstance.hideIFRank.indexOf(c), 1);

                } else {
                    if (gInstance.hideIFRank.indexOf(c) === -1)
                        gInstance.hideIFRank.push(c);

                    // remove link
                    /*var dc = [];
                     for (var i = 0; i < gInstance._linkData.length; i++) {
                     if (gInstance._linkData[i].FR.indexOf(c) >= 0) {
                     // Delete the link if it contains only one rank other wise remove this rank from the list
                     if (gInstance._linkData[i].FR.length === 1)
                     dc.push(i);
                     else
                     gInstance._linkData[i].FR.splice(gInstance._linkData[i].FR.indexOf(c) - 1);
                     }
                     }
                     
                     for (var i = 0, j = 0; i < dc.length; i++, j++) {
                     gInstance._linkData.splice(dc[i] - j, 1);
                     }*/


                }

                if (gInstance.hideIFRank.length > 0) {
                    for (var i = 0; i < gInstance._linkData.length; i++) {
                        if (gInstance._linkData[i].FR.indexOf(0) === -1) {
                            for (var j = 0; j < gInstance.hideIFRank.length; j++) {
                                var a = gInstance._linkData[i].FR.indexOf(gInstance.hideIFRank[j]);
                                if (a >= 0)
                                    gInstance._linkData[i].FR.splice(a, 1);
                                if (gInstance._linkData[i].FR.length === 0)
                                    nfs.push(i);
                            }
                        }
                    }
                }

                // Remove links which has either rank=0 or rank={unselected rank set} 
                for (var i = 0; i < nfs.length; i++) {
                    gInstance._linkData.splice(nfs[i], 1);
                }

                for (var i = 0; i < gInstance._linkData.length; i++) {
                    //if (gInstance._linkData[i].FR.indexOf(c) >= 0) {
                    gInstance._linkData[i].FC = gInstance.IFColors[gInstance._linkData[i].FR[0] - 1];
                    gInstance._linkData[i].FW = 12;
                    //}
                }

                // Retrieve nodes for existing links only
                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (_nid.indexOf((gInstance._linkData[i].source.Id) ? gInstance._linkData[i].source.Id : gInstance._linkData[i].source) === -1) {
                        _nid.push((gInstance._linkData[i].source.Id) ? gInstance._linkData[i].source.Id : gInstance._linkData[i].source);
                    }
                    if (_nid.indexOf((gInstance._linkData[i].target.Id) ? gInstance._linkData[i].target.Id : gInstance._linkData[i].target) === -1) {
                        _nid.push((gInstance._linkData[i].target.Id) ? gInstance._linkData[i].target.Id : gInstance._linkData[i].target);
                    }
                }

                for (var i = 0; i < gInstance._graph.nodes.length; i++) {
                    if (_nid.indexOf(gInstance._graph.nodes[i].Id) !== -1) {
                        gInstance._nodeData.push($.extend(true, {}, gInstance._graph.nodes[i]));
                    }
                }
            }

            var activeFlares = 0;
            // Checked all interesting paths
            for (var i = 1; i <= gInstance.totalFlares; i++) {
                if ($("#if_" + i).prop("checked") === true)
                    activeFlares++;
            }

            if (activeFlares === gInstance.totalFlares) {
                $("#saf").prop("checked", true);
            } else {
                $("#saf").prop("checked", false);
            }

            if (gInstance.dpie)
                gInstance.drawPie();
            else
                gInstance.draw(gInstance.labelIndex);
        },

        createFlareLegends: function () {
            $("#int-flare").css("display", "block");

            $("#flare-details").html("");
            $("#flare-details").html("<ul class='flare_options'></ul><fieldset><legend>Flare color&nbsp;</legend><ul class='flare_legend'></ul></fieldset>");

            var map = new HashMap(),
                    pc_map = new HashMap();

            this._linkData = $.extend(true, [], this.graph.links);
            this._nodeData = $.extend(true, [], this.graph.nodes);
            this.intFlareRank = [];

            for (var li = 0; li < this._linkData.length; li++) {
                this._linkData[li].FC = (this._linkData[li].FR[0] > 0) ? this.IFColors[this._linkData[li].FR[0] - 1] : this.defaultEdgeColor;
                var d = this._linkData[li];
                //var r = parseInt(d.FR);
                for (var pr = 0; pr < d.FR.length; pr++) {
                    var r = d.FR[pr];

                    if (r > 0 && map.get(r) === null) {
                        map.put(r, "<input type='text' class='color_pick' r='" + r + "' id='f_color_" + r + "' value='" + gInstance.IFColors[r - 1] + "'/>Flare:" + r);
                        this.intFlareRank.push(r);
                        pc_map.put(r, gInstance.IFColors[r - 1]);
                    }
                }

            }

            this.intFlareRank.sort(function (a, b) {
                return a - b;
            });

            var sack = "";
            if (this.intFlareRank.length === this.totalFlares) {
                sack = " checked";
            }

            var s = "<li><label id='idl'>Flare width:</label>&nbsp<input type='range' min='1' max='100' value='50' id='fw' />&nbsp;<label id='fwl'>50%</label></li>" +
                    "<li><input type='checkbox' id='ifs' /><label class='fa' for='ifs'>Show only interesting flares</label></li>" +
                    "<li><input type='checkbox' id='saf'" + sack + " /><label class='fa' for='saf'>Select all</label></li>";

            $(".flare_options").html(s);

            s = "";
            for (var i = 0; i < this.intFlareRank.length; i++) {
                s += "<li><input type='checkbox' r='" + this.intFlareRank[i] + "' name='pc' id='if_" + this.intFlareRank[i] +
                        "' checked /><label class='fa' for='if_" + this.intFlareRank[i] + "'>" + map.get(this.intFlareRank[i]) + "</label></li>";
            }

            $(".flare_legend").html(s);

            for (var i = 0; i < this.intFlareRank.length; i++) {
                $("#f_color_" + this.intFlareRank[i]).spectrum({color: pc_map.get(this.intFlareRank[i])});
                $("#f_color_" + this.intFlareRank[i]).spectrum({
                    change: function (c) {
                        gInstance.changeFlarecolor(this, c.toHexString());
                    }
                });
            }

            d3.select("#fw").on("change", gInstance.changeIFWidth);
            d3.select("#ifs").on("change", gInstance.showIFs);
            d3.select("#saf").on("change", gInstance.showAllFlares);
            for (var i = 0; i < this.intFlareRank.length; i++) {
                d3.select("#if_" + this.intFlareRank[i]).on("change", gInstance.showHideIntFlare);
            }

            if (this.dpie)
                this.drawPie();
            else
                this.draw(this.labelIndex);

        },

        changeIFWidth: function () {
            var val = $("#fw").val();
            $("#fwl").html(val + "%");

            gInstance.IFEdgeWeight = val / 100;

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showHidePattern: function () {
            gInstance.piePattern = false;
            if ($(this).prop("checked") === true) {
                gInstance.piePattern = true;
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        createPieColorLegend: function () {
            gInstance.selectButton("attr-btn", $(this));

            //$("#int-path").css("display", "none");
            $("#pie-legend").css("display", "block");
            //$("#int-flare").css("display", "none");

            $("#pie-legend .legendhead").html("");
            $("#pie-legend .legendhead").html("<fieldset><legend>Pie chart node color&nbsp;</legend><ul class='pattern'></ul><ul class='legend'></ul></fieldset>");
            var s = "";
            for (var i = 0; i < gInstance._graph.indv.length; i++) {
                if (gInstance._graph.indv[i].indexOf("#") > -1) {
                    var gl = gInstance._graph.indv[i].split("#");
                    var gen = gl[0];
                    var loc = gl[1];
                    s += " <li><input type='text' class='color_pick' r='" + (i + 1) + "' id='fcolor_" + (i + 1) + "' value='" + gInstance._graph.color[i] + "'/>" + gen + "&nbsp;in&nbsp;" + loc + "</li>";
                } else {
                    var gl = gInstance._graph.indv[i];
                    s += " <li><input type='text' class='color_pick' r='" + (i + 1) + "' id='fcolor_" + (i + 1) + "' value='" + gInstance._graph.color[i] + "'/>" + gInstance._graph.indv[i] + "</li>";
                }
            }
            $(".legend").html(s);
            $(".pattern").html("<li><input type='checkbox' name='pattern' id='pattern' /><label class='fa' for='pattern'>Show as a pattern</label></li>");

            for (var i = 0; i < gInstance._graph.indv.length; i++) {
                $("#fcolor_" + (i + 1)).spectrum({color: gInstance._graph.color[i]});
                $("#fcolor_" + (i + 1)).spectrum({
                    change: function (c) {
                        gInstance.changeIndvcolor(this, c.toHexString());
                    }
                });
            }

            d3.select("#pattern").on("click", gInstance.showHidePattern);
            $("#pattern").prop("checked", gInstance.piePattern);

            gInstance.drawPie();

        },

        getFontColor: function (d, i) {
            var c = d.Color[i];
            if (c[5] === 'f' && c[6] === 'f') {
                if (c[1] === '0' && c[2] <= '6' && c[3] <= '8')
                    return "yellow";
            }

            if (c[3] <= '8' && c[5] === '0' && c[6] === '0') {
                if (c[1] === 'f' && c[2] === 'f')
                    return "dark gray";
            }
            return "black";
        },

        resetFontSize: function () {
            var t = $(".labels").find("*");
            t.each(function (i) {
                var _a = $(t[i]),
                        r = parseFloat(_a.attr("r")),
                        a = this.getComputedTextLength(),
                        b = (2 * r - 8) / a * 24,
                        s = Math.min(r, b) + "px";
                _a.css("font-size", s);
            });
        },

        changeNodeColor: function (index, e) {
            gInstance.selectButton("attr-btn", e);

            gInstance.lIndex = parseInt(index);

            var dr = false;
            if (gInstance.dpie) {
                gInstance.draw(index);
                dr = true;
            } else {
                gInstance.labelIndex = gInstance.lIndex;
            }

            $("#pie-legend").css("display", "none");
            $("#pie-legend .legendhead").html("");
            $("#top-nav").css("display", "none");

            this.node.attr("fill", function (d) {
                return d.Color[index];
            });
            this.labelText.text(function (d) {
                return d.Label[index];
            })
                    .style("fill", function (d) {
                        return gInstance.getFontColor(d, index);
                    });
            if (!dr)
                gInstance.resetFontSize();
        },

        getNodeCoordinate: function () {
            d3.select("#type").attr("value", "get-coordinate");
            d3.select("#data").attr("value", "");
            d3.select("#folderName").attr("value", gInstance.fl[gInstance.fileIndex].split(".")[0]);
            d3.select("#fileName").attr("value", gInstance.jfl[gInstance.fileRIndex].files[gInstance.fileCIndex]);
            var form = $("#frm");

            $.ajax({
                type: "POST",
                url: "./datahandler/getcoord?t=" + new Date().getTime(),
                data: form.serialize(),
                success: function (json) {
                    if (json === "404") {
                        gInstance.coordData = null;
                    } else {
                        gInstance.coordData = JSON.parse(json);

                        gInstance.node.each(function (d) {
                            var __coord = gInstance.coordData[0]["nid_" + d.Id];
                            if (__coord) {
                                d.fx = __coord[0];
                                d.fy = __coord[1];
                            }
                        });
                    }
                },
                error: function (request, err, ex) {
                    alert("Error to post data")
                }
            });
        },

        getCoordinates: function () {

            if (gInstance.coordData) {
                gInstance.node.each(function (d) {
                    var __coord = gInstance.coordData[0]["nid_" + d.Id];
                    if (__coord) {
                        d.fx = __coord[0];
                        d.fy = __coord[1];
                    }
                });
            } else {
                gInstance.getNodeCoordinate();
            }
        },

        clickGetCoordinates: function () {
            gInstance.selectButton("other-btn", $(this));
            gInstance.getCoordinates();
        },

        setCoordinates: function () {
            gInstance.selectButton("other-btn", $(this));

            var nPos = "{";
            gInstance.node.each(function (d) {
                if (nPos.length > 1)
                    nPos += ",";
                nPos += "\"nid_" + d.Id + "\":[" + d.x + "," + d.y + "]";
            });
            nPos += "}";

            d3.select("#type").attr("value", "set-coordinate");
            d3.select("#data").attr("value", nPos);
            d3.select("#folderName").attr("value", gInstance.fl[gInstance.fileIndex].split(".")[0]);
            d3.select("#fileName").attr("value", gInstance.jfl[gInstance.fileRIndex].files[gInstance.fileCIndex]);
            var form = $("#frm");

            $.ajax({
                type: "POST",
                url: "./datahandler/setcoord?t=" + new Date().getTime(),
                data: form.serialize(),
                success: function (json) {
                    var a = 10;
                },
                error: function (request, err, ex) {
                    var a = 10;
                }
            });
        },

        saveColors: function () {
            gInstance.selectButton("other-btn", $(this));

            var s = JSON.stringify(gInstance._graph);

            d3.select("#type").attr("value", "set-color");
            d3.select("#data").attr("value", s);
            d3.select("#folderName").attr("value", gInstance.fl[gInstance.fileIndex].split(".")[0]);
            d3.select("#fileName").attr("value", gInstance.jfl[gInstance.fileRIndex].files[gInstance.fileCIndex]);
            var form = $("#frm");

            $.ajax({
                type: "POST",
                url: "./datahandler/setcolor?t=" + new Date().getTime(),
                data: form.serialize(),
                success: function (json) {
                    var a = 10;
                },
                error: function (request, err, ex) {
                    var a = 10;
                }
            });
        },

        createColorBar: function () {
            gInstance.selectButton("other-btn", $(this));

            var pos = [];
            var posC = [];

            gInstance.node.each(function (d) {
                pos.push([d.x, d.Id]);
            });

            pos.sort(function (a, b) {
                return a[0] - b[0];
            });

            var _int = parseInt(pos.length / 5 > 10 ? pos.length / 10 : pos.length / 5);

            var _ic = 0;
            for (; _ic < pos.length; _ic += _int) {
                posC.push([pos[_ic][1], "", 0.0]);
            }

            if (_ic === pos.length && _ic - _int < pos.length - 1) {
                posC.push([pos[pos.length - 1][1], "", 0.0]);
                _int++;
            }

            gInstance.node.each(function (d) {
                for (var _i = 0; _i < posC.length; _i++) {
                    if (posC[_i][0] === d.Id && posC[_i][1].length === 0) {
                        posC[_i][1] = d.Color[1];
                        posC[_i][2] = parseFloat(d.Label[1]);
                        break;
                    }
                }
            });

            var w = $(window).width(), //pos[pos.length - 1][0] - pos[0][0], 
                    h = 70;

            if (d3.selectAll("#legends")) {
                d3.selectAll("#legends").remove();
                d3.selectAll("linearGradient").remove();
            }
            var _g = gInstance.svg.append("g").attr("id", "legends");

            var legend = gInstance.marker
                    .append("svg:linearGradient")
                    .attr("id", "gradient")
                    .attr("x1", "0%")
                    .attr("y1", "100%")
                    .attr("x2", "100%")
                    .attr("y2", "100%")
                    .attr("spreadMethod", "pad")
                    .attr('fill', 'none')
                    .style('stroke', '#000');

            for (var i = 0; i < posC.length; i++) {
                var _perc = i * 100 / (posC.length - 1);

                legend.append("stop")
                        .attr("offset", "" + _perc + "%")
                        .attr("stop-color", posC[i][1])
                        .attr("stop-opacity", 1);
            }

            _g.append("rect")
                    .attr("width", w)
                    .attr("height", h - 40)
                    .style("fill", "url(#gradient)");

            var y = d3.scaleLinear()
                    .range([w, 0])
                    .domain([posC[posC.length - 1][2], posC[0][2]]);

            var yAxis = d3.axisBottom()
                    .scale(y)
                    .ticks(_int);

            _g.attr("class", "y axis")
                    .attr("transform", "translate(0," + ($(window).height() - 40) + ")")
                    .call(yAxis)
                    .append("text")
                    .attr("transform", "rotate(-90)")
                    .attr("y", 0)
                    .attr("dy", ".71em")
                    .style("text-anchor", "end")
                    .text("")
                    .style("text-anchor", "middle")
                    .style("fill", "#ff9189")
                    .style("font-family", "Arial")
                    .style("font-weight", "bold")
                    .style("font-size", "20px");

            d3.selectAll("#legends text").style("fill", "#000")
                    .style("font-family", "Arial")
                    .style("font-weight", "bold")
                    .style("font-size", "20px");
        },

        createButtons: function () {

            $("#thumbnails").css("display", "flex");

            var label = [];

            for (var i = 0; i < this._graph.btn.length; i++) {
                if (label.length === 0)
                    label.push(this._graph.btn[i]);
                else if (label.indexOf(this._graph.btn[i]) === -1)
                    label.push(this._graph.btn[i]);
            }


            for (var i = 0; i < label.length; i++) {
                label[i] = label[i].replace(".", "_");
                label[i] = label[i].replace(" ", "_");
                label[i] = label[i].replace("+", "_");
                label[i] = label[i].replace("-", "_");
                label[i] = label[i].replace("$", "_");
            }

            this.hasPieChart = false;
            var s = "<fieldset><legend>Node attributes&nbsp;</legend>";
            if (this._graph.nodes[0].pie.length > 0) {
                s += "<button id='btn_Pie' >Pie chart</button>&nbsp;";
                this.hasPieChart = true;
            }

            for (var l in label) {
                s += "<button id='btn_" + label[l] + "' seq='" + l + "'>" + label[l].replace("_", " ") + "</button>&nbsp;";
            }
            $("#attr-btn").html(s + "</fieldset>");
            s = "<fieldset><legend>Actions&nbsp;</legend><button id='save_image'>Save image</button>&nbsp;";

            // dev version
            s += "<button id='get_coord' title='Assign saved node position to the graph'>Restore nodes</button>&nbsp;" +
                    "<button id='set_coord' title='Save node positions'>Save nodes</button>&nbsp;" +
                    "<button id='set_color' title='Save colors whatever you changed'>Save colors</button>&nbsp;" +
                    "<button id='color_bar' title='Color bar'>Color bar</button>&nbsp;" +
                    "<button id='node_analysis' title='Explore nodes' style='display:none;'>Analysis</button>&nbsp;";

            $("#other-btn").html(s + "</fieldset>");

            d3.select("#save_image").style("color", "wheat").on("click", gInstance.saveImage);
            d3.select("#btn_Pie").style("color", "wheat").on("click", gInstance.createPieColorLegend);

            for (var l in label) {
                if (parseInt(l) === 0) {
                    $("#btn_" + label[l]).addClass("button_bc");
                }
                $("#btn_" + label[l]).css("color", "wheat").on("click", function () {
                    gInstance.changeNodeColor($(this).attr("seq"), $(this));
                });
            }

            // For dev version
            d3.select("#get_coord").style("color", "wheat").on("click", gInstance.clickGetCoordinates);
            d3.select("#set_coord").style("color", "wheat").on("click", gInstance.setCoordinates);
            d3.select("#set_color").style("color", "wheat").on("click", gInstance.saveColors);
            d3.select("#color_bar").style("color", "wheat").on("click", gInstance.createColorBar);
            d3.select("#node_analysis").style("color", "wheat").on("click", gInstance.analyzeNodes);
            d3.select("#wdtf-btn").style("color", "wheat").on("click", gInstance.writeDataToFile);
            d3.select("#clearNIDs").style("color", "wheat").on("click", gInstance.removeSelection);

            // 
            s = "";
            for (var i = 0; i < this._graph.HN.length; i++) {
                //s += "<a href='javascript:void(0)' class='file-select' seq='" + i + "'>" + this.fl[i] + "</a>";
                if (this._graph.HN[i].length > 0)
                    s += "<li><input type='checkbox' class='attr-select' id='ancb_" + (i + 1) + "' seq='" + (i + 1) + "' /><label class='fa' for='ancb_" + (i + 1) + "'>" + this._graph.HN[i] + "</label></li>";
            }

            $("#colDropdown ul.items").html(s);
            $(".attr-select").on("click", function () {
                var val = $(this).attr('seq');

                if ($("ancb_" + val).prop("checked") === true)
                    $("ancb_" + val).prop("checked", false);
                else
                    $("ancb_" + val).prop("checked", true);

                gInstance.adjustSelectedAttr();
            });
        },

        changeEdgeColor: function (col) {

            for (var i = 0; i < gInstance._linkData.length; i++) {
                if (gInstance._linkData[i].W === 2) {
                    gInstance._linkData[i].C = col;
                }
            }

            for (var i = 0; i < gInstance._graph.links.length; i++) {
                if (gInstance._graph.links[i].W === 2) {
                    gInstance._graph.links[i].C = col;
                }
            }

            gInstance.defaultEdgeColor = col;

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }

        },

        showEdgeArrow: function () {
            gInstance.shEdgeArrow = false;
            if ($(this).prop("checked") === true) {
                gInstance.shEdgeArrow = true;
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showEdgeRank: function () {
            gInstance.shEdgeRank = false;
            gInstance.shEdgeSig = false;
            if ($(this).prop("checked") === true) {
                gInstance.shEdgeRank = true;
                $("#eSig").prop("checked", false);
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showEdgeSig: function () {
            gInstance.shEdgeSig = false;
            gInstance.shEdgeRank = false;
            if ($(this).prop("checked") === true) {
                gInstance.shEdgeSig = true;
                $("#eRank").prop("checked", false);
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        changePathcolor: function (e, col) {
            var c = parseInt($(e).attr("r"));

            this.IPColors[c - 1] = col;

            for (var i = 0; i < this._linkData.length; i++) {
                if (this._linkData[i].R[0] === c) {
                    this._linkData[i].C = this.IPColors[c - 1];
                }
            }

            for (var i = 0; i < this._graph.links.length; i++) {
                if (this._graph.links[i].R[0] === c) {
                    this._graph.links[i].C = this.IPColors[c - 1];
                }
            }

            if (this.dpie) {
                this.drawPie();
            } else {
                this.draw(this.labelIndex);
            }

        },

        showAllEdges: function () {
            if ($(this).prop("checked") === true) {
                $("#ips").prop("checked", false);
            }

            gInstance._nodeData = $.extend(true, [], gInstance._graph.nodes);
            gInstance._linkData = $.extend(true, [], gInstance._graph.links);

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showIPs: function () {
            if ($(this).prop("checked") === true) {
                $("#sa").prop("checked", false);
            }

            // Checked all interesting paths
            for (var i = 0; i < gInstance.intPathRank.length; i++) {
                $("#ip_" + gInstance.intPathRank[i]).prop("checked", true);
            }

            gInstance._nodeData = [];
            gInstance._linkData = [];
            var _nid = [];

            // Filter links and nodes
            for (var i = 0; i < gInstance._graph.links.length; i++) {
                if (gInstance._graph.links[i].R[0] > 0) {
                    gInstance._linkData.push($.extend(true, {}, gInstance._graph.links[i]));

                    if (_nid.indexOf((gInstance._graph.links[i].source.Id) ? gInstance._graph.links[i].source.Id : gInstance._graph.links[i].source) === -1) {
                        _nid.push((gInstance._graph.links[i].source.Id) ? gInstance._graph.links[i].source.Id : gInstance._graph.links[i].source);
                    }
                    if (_nid.indexOf((gInstance._graph.links[i].target.Id) ? gInstance._graph.links[i].target.Id : gInstance._graph.links[i].target) === -1) {
                        _nid.push((gInstance._graph.links[i].target.Id) ? gInstance._graph.links[i].target.Id : gInstance._graph.links[i].target);
                    }
                }
            }

            for (var i = 0; i < gInstance._linkData.length; i++) {
                if (gInstance._linkData[i].R[0] > 0) {
                    gInstance._linkData[i].C = gInstance.IPColors[gInstance._linkData[i].R[0] - 1];
                }
            }

            for (var i = 0; i < gInstance._graph.nodes.length; i++) {
                if (_nid.indexOf(gInstance._graph.nodes[i].Id) !== -1) {
                    gInstance._nodeData.push($.extend(true, {}, gInstance._graph.nodes[i]));
                }
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showAllIPs: function () {
            var _allIntPaths = $("#sap").prop("checked");

            gInstance.selectedFeature[0] = gInstance.selectedFeature[1] = false;
            if (_allIntPaths === true) {
                gInstance.selectedFeature[0] = true;

                //if (gInstance.intPathRank.length < gInstance.totalpaths) {
                // Filter links and nodes
                gInstance._nodeData = $.extend(true, [], gInstance.graph.nodes);
                gInstance._linkData = $.extend(true, [], gInstance.graph.links);
                //}

                // Checked all interesting paths
                for (var i = 1; i <= gInstance.totalpaths; i++) {
                    $("#ip_" + i).prop("checked", true);
                }

                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (gInstance._linkData[i].R[0] > 0) {
                        gInstance._linkData[i].C = gInstance.IPColors[gInstance._linkData[i].R[0] - 1];
                    }
                }
            } else {
                // Checked all interesting paths
                for (var i = 1; i <= gInstance.totalpaths; i++) {
                    $("#ip_" + i).prop("checked", false);
                }

                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (gInstance._linkData[i].R[0] > 0) {
                        gInstance._linkData[i].C = gInstance.defaultEdgeColor;
                        gInstance._linkData[i].W = 2;
                    }
                }
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showHideIntPath: function () {
            var _nid = [],
                    _show = $(this).prop("checked"),
                    c = parseInt($(this).attr("r")),
                    _allPaths = $("#sa").prop("checked"),
                    _intPaths = $("#ips").prop("checked");
            //_allIntPaths = $("#sap").prop("checked");

            if (_intPaths || _show) {
                gInstance.selectedFeature[0] = true;
                gInstance.selectedFeature[1] = false;
            }

            // Filter links and nodes
            if (_allPaths === true) {

                if (_show === true) {
                    for (var i = 0; i < gInstance._linkData.length; i++) {
                        if (gInstance._linkData[i].R[0] === c) {
                            gInstance._linkData[i].C = gInstance.IPColors[c - 1];
                            gInstance._linkData[i].W = 12;
                        }
                    }
                } else {
                    for (var i = 0; i < gInstance._linkData.length; i++) {
                        if (gInstance._linkData[i].R[0] === c) {
                            gInstance._linkData[i].C = gInstance.defaultEdgeColor;
                            gInstance._linkData[i].W = 2;
                        }
                    }
                }

            } else if (_intPaths === true) {
                gInstance._nodeData = [];

                if (_show === true) {

                    // add missing link
                    for (var i = 0; i < gInstance._graph.links.length; i++) {
                        if (gInstance._graph.links[i].R[0] === c) {
                            gInstance._linkData.push($.extend(true, {}, gInstance._graph.links[i]));
                        }
                    }

                    for (var i = 0; i < gInstance._linkData.length; i++) {
                        if (gInstance._linkData[i].R[0] > 0) {
                            gInstance._linkData[i].C = gInstance.IPColors[gInstance._linkData[i].R[0] - 1];
                        }
                    }

                } else {
                    // remove link
                    var dc = [];
                    for (var i = 0; i < gInstance._linkData.length; i++) {
                        if (gInstance._linkData[i].R[0] === c) {
                            dc.push(i);
                        }
                    }

                    for (var i = 0, j = 0; i < dc.length; i++, j++) {
                        gInstance._linkData.splice(dc[i] - j, 1);
                    }
                }

                // Retrieve nodes for existing links only
                for (var i = 0; i < gInstance._linkData.length; i++) {
                    if (_nid.indexOf((gInstance._linkData[i].source.Id) ? gInstance._linkData[i].source.Id : gInstance._linkData[i].source) === -1) {
                        _nid.push((gInstance._linkData[i].source.Id) ? gInstance._linkData[i].source.Id : gInstance._linkData[i].source);
                    }
                    if (_nid.indexOf((gInstance._linkData[i].target.Id) ? gInstance._linkData[i].target.Id : gInstance._linkData[i].target) === -1) {
                        _nid.push((gInstance._linkData[i].target.Id) ? gInstance._linkData[i].target.Id : gInstance._linkData[i].target);
                    }
                }

                for (var i = 0; i < gInstance._graph.nodes.length; i++) {
                    if (_nid.indexOf(gInstance._graph.nodes[i].Id) !== -1) {
                        gInstance._nodeData.push($.extend(true, {}, gInstance._graph.nodes[i]));
                    }
                }
            }

            var activePaths = 0;
            // Checked all interesting paths
            for (var i = 1; i <= gInstance.totalpaths; i++) {
                if ($("#ip_" + i).prop("checked") === true)
                    activePaths++;
            }

            if (activePaths === gInstance.totalpaths) {
                $("#sap").prop("checked", true);
            } else {
                $("#sap").prop("checked", false);
            }

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        changeIPWidth: function () {
            var val = $("#pw").val();
            $("#pwl").html((val - 100) + "%");

            gInstance.IPEdgeWeight = val / 100;

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        getAllRowIdsOfANode: function (nodeID) {
            var rIDs = [];
            gInstance.node.each(function (d) {
                if (d.Id === nodeID) {
                    var _pt = d.Ph;
                    for (var j = 0; j < _pt.length; j++) {
                        for (var k = _pt[j][0]; k <= _pt[j][1]; k++) {
                            rIDs.push(k);
                        }
                    }
                }
            });

            return rIDs;
        },

        getAllRowIdsOfAPath: function (pathRank) {
            var rIDs = [];
            var nc = [];
            gInstance.link.each(function (d) {
                if (d.R === pathRank) {
                    var s = (d.source.Id) ? d.source.Id : d.source;
                    var t = (d.target.Id) ? d.target.Id : d.target;

                    if (!nc.includes(s)) {
                        nc.push(s);

                        var tIDs = gInstance.getAllRowIdsOfANode(s);
                        for (var i = 0; i < tIDs.length; i++) {
                            if (!rIDs.includes(tIDs[i])) {
                                rIDs.push(tIDs[i]);
                            }
                        }
                    }

                    if (!nc.includes(t)) {
                        nc.push(t);

                        var tIDs = gInstance.getAllRowIdsOfANode(t);
                        for (var i = 0; i < tIDs.length; i++) {
                            if (!rIDs.includes(tIDs[i])) {
                                rIDs.push(tIDs[i]);
                            }
                        }
                    }
                }
            });

            return rIDs;
        },

        pickData: function (rawIDs) {
            rawIDs.sort();
            var _data = [];
            var _file = "Data/csv/" + gInstance.fl[gInstance.fileIndex] + "?t=" + (new Date).getTime();
            d3.csv(_file, function (error, data) {
                if (error)
                    throw error;

                gInstance.csvData = data;
                for (var i = 0; i < data.length; i++) {
                    var _id = Number(eval("data[i]." + "pID"));
                    var _cov = eval("data[i]." + "PlantID");
                    if (rawIDs.includes(_id)) {
                        if (!_data.includes(_cov)) {
                            _data.push(_cov);
                        }
                    }
                }

            });

            if (_data.length === 0 && gInstance.csvData !== null) {
                for (var i = 0; i < gInstance.csvData.length; i++) {
                    var _id = Number(eval("gInstance.csvData[i]." + "pID"));
                    var _cov = eval("gInstance.csvData[i]." + "PlantID");
                    if (rawIDs.includes(_id)) {
                        if (!_data.includes(_cov)) {
                            _data.push(_cov);
                        }
                    }
                }
            }

            return _data;
        },

        pathAnalysis: function () {
            var _l = $("#txtPA").val();
            if (_l.toUpperCase() === 'ALL') {
                _l = "";
                for (var i = 1; i <= gInstance.totalpaths; i++) {
                    if (_l.length > 0)
                        _l += ",";
                    _l += i;
                }
            }
            var IP_ids = eval("[" + _l + "]");
            if (IP_ids.length === 0)
                return false;

            var rIDs, cov, s = "";
            for (var i = 0; i < IP_ids.length; i++) {
                rIDs = gInstance.getAllRowIdsOfAPath(IP_ids[i]);

                cov = gInstance.pickData(rIDs);

                if (i > 0)
                    s += "<br /><br />";

                s += "<span>Interesting Path rank:" + IP_ids[i] + "</span><br /><span>Individuals: ";
                var _tc = "";
                for (var j = 0; j < cov.length; j++) {
                    if (_tc.length > 0)
                        _tc += ",";
                    _tc += cov[j];
                }
                s += _tc + "</span>";
            }
            $("#pa-result").html(s);
            $("#pa-result").css("color", "wheat");
        },

        createPathLegends: function () {
            $("#int-path").css("display", "block");

            $("#path-details").html("");
            $("#path-details").html("<ul class='path_options'></ul><fieldset><legend>Path color&nbsp;</legend><ul class='path_legend'></ul></fieldset>" +
                    "<fieldset><legend>Path analysis&nbsp;</legend><input type='text' id='txtPA' value='' style='color:wheat' />" +
                    "<input type='button' id='pa-btn' value='Analysis' style='color:wheat' /><label id='pa-result'></label></fieldset>");

            var map = new HashMap(),
                    pc_map = new HashMap();

            this._linkData = $.extend(true, [], this.graph.links);
            this._nodeData = $.extend(true, [], this.graph.nodes);
            this.intPathRank = [];

            for (var li = 0; li < this._linkData.length; li++) {
                this._linkData[li].C = (this._linkData[li].R[0] > 0) ? this.IPColors[this._linkData[li].R[0] - 1] : this.defaultEdgeColor;
                var d = this._linkData[li];
                var r = parseInt(d.R[0]);
                if (r > 0 && map.get(r) === null) {
                    map.put(r, "<input type='text' class='color_pick' r='" + r + "' id='color_" + r + "' value='" + d.C + "'/>Path:" + r + ", Sig:" + d.L);
                    this.intPathRank.push(r);
                    pc_map.put(r, this.IPColors[r - 1]);
                }
            }

            if (this.intPathRank.length === 0) {
                $("#int-path").css("display", "none");
                return 0;
            }

            this.intPathRank.sort(function (a, b) {
                return a - b;
            });

            var sack = "";
            if (this.intPathRank.length === this.totalpaths) {
                sack = " checked";
            }

            var s = "<li><label id='idl'>Path width:</label>&nbsp<input type='range' min='101' max='200' value='150' id='pw' />&nbsp;<label id='pwl'>50%</label></li>" +
                    "<li><input type='radio' id='sa' name='esr' checked /><label class='fa' for='sa'>Show all edges</label></li>" +
                    "<li><input type='radio' id='ips' name='esr' /><label class='fa' for='ips'>Show only interesting paths</label></li>" +
                    "<li><input type='checkbox' id='sap'" + sack + " /><label class='fa' for='sap'>Show all paths</label></li>";

            $(".path_options").html(s);

            s = "";
            for (var i = 0; i < this.intPathRank.length; i++) {
                s += "<li><input type='checkbox' r='" + this.intPathRank[i] + "' name='pc' id='ip_" + this.intPathRank[i] +
                        "' checked /><label class='fa' for='ip_" + this.intPathRank[i] + "'>" + map.get(this.intPathRank[i]) + "</label></li>";
            }

            $(".path_legend").html(s);

            for (var i = 0; i < this.intPathRank.length; i++) {
                $("#color_" + this.intPathRank[i]).spectrum({color: pc_map.get(this.intPathRank[i])});
                $("#color_" + this.intPathRank[i]).spectrum({
                    change: function (c) {
                        gInstance.changePathcolor(this, c.toHexString());
                    }
                });
            }

            d3.select("#pw").on("change", gInstance.changeIPWidth);
            d3.select("#sa").on("change", gInstance.showAllEdges);
            d3.select("#ips").on("change", gInstance.showIPs);
            d3.select("#sap").on("change", gInstance.showAllIPs);
            for (var i = 0; i < this.intPathRank.length; i++) {
                d3.select("#ip_" + this.intPathRank[i]).on("change", gInstance.showHideIntPath);
            }

            //d3.select("#txtPA").on("click", gInstance.pathAnalysis);
            d3.select("#pa-btn").on("click", gInstance.pathAnalysis);

            if (this.dpie) {
                this.drawPie();
            } else {
                this.draw(this.labelIndex);
            }
        },

        disableFeatures: function () {
            $("#int-path").css("display", "none");
            //$("#pie-legend").css("display", "none");
            $("#int-flare").css("display", "none");

            gInstance.selectedFeature[0] = false;
            gInstance.selectedFeature[1] = false;

            if (gInstance.dpie) {
                gInstance.drawPie();
            } else {
                gInstance.draw(gInstance.labelIndex);
            }
        },

        showInterestingPaths: function () {
            $("#int-path").css("display", "block");
            //$("#pie-legend").css("display", "none");
            $("#int-flare").css("display", "none");

            gInstance.selectedFeature[0] = true;
            gInstance.selectedFeature[1] = false;

            gInstance.EdgeDirChg = false;

            gInstance.createPathLegends();
        },

        showInterestingFlares: function () {
            $("#int-path").css("display", "none");
            //$("#pie-legend").css("display", "block");
            $("#int-flare").css("display", "block");

            gInstance.selectedFeature[0] = false;
            gInstance.selectedFeature[1] = true;

            gInstance.EdgeDirChg = true;

            gInstance.createFlareLegends();
        },

        showHideToolTip: function () {
            gInstance.showToolTip = false;
            d3.select("#tooltip").style("display", "none");

            if ($(this).prop("checked") === true) {
                gInstance.showToolTip = true;
            }
        },

        createAttributes: function () {
            $("#attr-ctrl").css("display", "flex");
            $("#attr-details").html("");
            $("#attr-details").html("<fieldset><legend>View attributes&nbsp;</legend><ul class='view_attr_legend'></ul></fieldset>" +
                    "<fieldset><legend>Tooltip&nbsp;</legend><ul class='node_attr_legend'></ul></fieldset>" +
                    "<fieldset><legend>Edge attributes&nbsp;</legend><ul class='attr_legend'></ul></fieldset>" +
                    "<fieldset><legend>Features&nbsp;</legend><ul class='feature_legend'></ul></fieldset>");

            $(".view_attr_legend").html("<li><input type='text' class='color_pick' id='viewBC' value='#000'/>&nbsp;Background color</li>");

            $("#viewBC").spectrum({color: this.svgBGColor});
            $("#viewBC").spectrum({
                change: function (c) {
                    d3.select("#svg-container").style("background-color", c.toHexString());
                    d3.select("#viewer").style("background-color", c.toHexString());
                    gInstance.svgBGColor = c.toHexString();
                }
            });

            $(".node_attr_legend").html("<li><input type='checkbox' name='shTT' id='shTT' /><label class='fa' for='shTT'>Show tooltip</label></li>");

            d3.select("#shTT").on("change", gInstance.showHideToolTip);

            var s = "<li><input type='text' class='color_pick' id='edgeDC' value='" + this.defaultEdgeColor + "'/>&nbsp;Edge color</li>" +
                    "<li><input type='checkbox' name='eArrow' id='eArrow' checked /><label class='fa' for='eArrow'>Show edge direction</label></li>" +
                    "<li><input type='checkbox' name='eRank' id='eRank' /><label class='fa' for='eRank'>Show edge rank</label></li>" +
                    "<li><input type='checkbox' name='eSig' id='eSig' /><label class='fa' for='eSig'>Show edge signature</label></li>";

            $(".attr_legend").html(s);

            $("#edgeDC").spectrum({color: this.defaultEdgeColor});
            $("#edgeDC").spectrum({
                change: function (c) {
                    gInstance.changeEdgeColor(c.toHexString());
                }
            });

            d3.select("#eArrow").on("change", gInstance.showEdgeArrow);
            d3.select("#eRank").on("change", gInstance.showEdgeRank);
            d3.select("#eSig").on("change", gInstance.showEdgeSig);

            s = "<li><input type='radio' name='feature' id='no-feature' value='no-feature' checked  /><label class='fa' for='no-feature'>Show graph (without feature) </label></li>" +
                    "<li><input type='radio' name='feature' id='fpath' value='fpath'  /><label class='fa' for='fpath'>Show interesting paths</label></li>";
            if (this.hasPieChart) {
                s += "<li><input type='radio' name='feature' id='fflare' value='fflare' /><label class='fa' for='fflare'>Show interesting flares</label></li>";
            }

            $(".feature_legend").html(s);

            d3.select("#no-feature").on("change", gInstance.disableFeatures);
            d3.select("#fpath").on("change", gInstance.showInterestingPaths);
            if (this.hasPieChart) {
                d3.select("#fflare").on("change", gInstance.showInterestingFlares);
            }
        },

        writeDataToFile: function () {
            var rIDs = [];

            for (var i = 0; i < gInstance._analysis.length; i++) {
                var tIDs = gInstance.getAllRowIdsOfANode(gInstance._analysis[i]);
                for (var j = 0; j < tIDs.length; j++) {
                    if (rIDs.indexOf(tIDs[j]) < 0) {
                        rIDs.push(tIDs[j]);
                    }
                }
            }

            if (rIDs.length > 0) {
                rIDs.sort(function (a, b) {
                    return a - b;
                });
            }

            var cols = [];
            for (var i = 1; i <= gInstance._graph.HN.length; i++) {
                if ($("#ancb_" + i).prop("checked") === true) {
                    cols.push(i);
                }
            }

            if (cols.length === 0) {
                alert("Please select columns.");
            } else {
                //var compVal = LZW.compress(rIDs.toString());
                d3.select("#type").attr("value", cols.toString());
                d3.select("#data").attr("value", rIDs.toString());
                d3.select("#folderName").attr("value", gInstance.fl[gInstance.fileIndex]);
                d3.select("#fileName").attr("value", "");
                var form = $("#frm");

                $.ajax({
                    type: "POST",
                    url: "./datahandler/writedata?t=" + new Date().getTime(),
                    data: form.serialize(),
                    success: function (json, d) {
                        if (json !== "404") {
                            if (json.indexOf("PHP Error") >= 0) {
                                alert("Error to write data in file. Check file permission.");
                            } else {
                                var _json = JSON.parse(json);
                                if (_json[0].code === 200) {
                                    alert("File saved at:" + _json[0].fn);
                                } else if (_json[0].code === 404) {
                                    alert("Error occured:\n" + _json[0].Error);
                                }
                            }

                        } else {
                            alert("Requested URL is missing. Possible cause could be server down or incorrect url.")
                        }
                    },
                    error: function (request, err, ex) {
                        alert("Error to post data")
                    }
                });
            }
        },

        adjustSelectedAttr: function () {
            var s = "";
            for (var i = 1; i <= gInstance._graph.HN.length; i++) {
                if ($("#ancb_" + i).prop("checked") === true) {
                    if (s.length > 0)
                        s += "<li><label class='fa'>,</label></li>";
                    s += "<li><label class='fa'>" + gInstance._graph.HN[i - 1] + "</label></li>";
                }
            }

            if (s.length > 0) {
                $("#attrList span").css("display", "block");
                $("#attrList ul").html(s);
            } else
                $("#attrList span").css("display", "none");
        },

        adjustSelectedNodes: function () {
            var s = "";
            for (var i = 0; i < gInstance._analysis.length; i++) {
                if (s.length > 0)
                    s += ", ";
                s += gInstance._analysis[i];
            }

            d3.select("#nIDs").html(s);
            if (s.length === 0) {
                d3.select("#clearNIDs").style("display", "none");
            } else {
                d3.select("#clearNIDs").style("display", "block");
            }
        },

        nodeRightClick: function (d) {
            d3.event.preventDefault();

            d3.select("#top-nav").style("display", "none");

            d3.selectAll("#top-nav ul li").remove();
            d3.select("#top-nav ul").append("li").append("a")
                    .attr("href", "javascript:void(0)")
                    .attr("id", "add-node")
                    .text("Add node for analysis");
            d3.select("#top-nav ul").append("li").append("a")
                    .attr("href", "javascript:void(0)")
                    .attr("id", "remove-node")
                    .text("Remove node for analysis");

            var x = 0.0, y = 0.0, k = 1.0;
            if (gInstance.__transform) {
                x = gInstance.__transform.x;
                y = gInstance.__transform.y;
                k = gInstance.__transform.k;
            }

            d3.select("#top-nav").style("display", "block")
                    .style("position", "absolute")
                    .style("top", y + (d.y * k) + "px")
                    .style("left", x + (d.x * k) + "px");

            d3.select("#add-node").attr("d", d.Id)
                    .on("click", function () {
                        var _id = parseInt($(this).attr("d"));
                        if (gInstance._analysis.indexOf(_id) < 0) {
                            gInstance._analysis.push(_id);

                            var _cid = "#node_" + _id;

                            d3.select(_cid).style("stroke", "#ff0000")
                                    .style("stroke-width", "2px");

                            d3.select("#int-nodes").style('display', 'block');
                            gInstance.adjustSelectedNodes();
                        }

                        d3.select("#top-nav").style("display", "none");

                        //displayAnalysis();
                    });

            d3.select("#remove-node").attr("d", d.Id)
                    .on("click", function () {
                        var _id = parseInt($(this).attr("d"));
                        if (gInstance._analysis.indexOf(_id) >= 0) {
                            gInstance._analysis.splice(gInstance._analysis.indexOf(_id), 1);

                            var _cid = "#node_" + _id;
                            d3.select(_cid).style("stroke", "")
                                    .style("stroke-width", "0px");

                            if (gInstance._analysis.length === 0) {
                                d3.select("#int-nodes").style('display', 'none');
                            }

                            gInstance.adjustSelectedNodes();
                        }

                        d3.select("#top-nav").style("display", "none");

                        //displayAnalysis();
                    });

            $("#top-nav").css("zIndex", "9999");
        },

        removeSelection: function () {
            for (var i = 0; i < gInstance._analysis.length; i++) {
                var _cid = "#node_" + gInstance._analysis[i];
                d3.select(_cid).style("stroke", "")
                        .style("stroke-width", "0px");
            }

            gInstance._analysis = [];
            d3.select("#int-nodes").style('display', 'none');
            gInstance.adjustSelectedNodes();
        },

        ticked: function () {
            gInstance.link
                    .attr("x1", function (d) {
                        var a = d.source;
                        return d.source.x;
                    })
                    .attr("y1", function (d) {
                        return d.source.y;
                    })
                    .attr("x2", function (d) {
                        return d.target.x;
                    })
                    .attr("y2", function (d) {
                        return d.target.y;
                    });
            d3.selectAll("circle")
                    .attr("cx", function (d) {
                        return d.x;
                    })
                    .attr("cy", function (d) {
                        return d.y;
                    });
            d3.selectAll(".labels text")
                    .attr("x", function (d) {
                        return d.x;
                    })
                    .attr("y", function (d) {
                        return d.y;
                    });

            if (gInstance.shEdgeRank || gInstance.shEdgeSig) {
                gInstance.edgepaths.attr('d', function (d) {
                    return 'M ' + d.source.x + ' ' + d.source.y + ' L ' + d.target.x + ' ' + d.target.y;
                });

                gInstance.edgelabels.attr('transform', function (d) {
                    if (d.target.x < d.source.x) {
                        var bbox = this.getBBox(),
                                rx = bbox.x + bbox.width / 2,
                                ry = bbox.y + bbox.height / 2;
                        return 'rotate(180 ' + rx + ' ' + ry + ')';
                    } else {
                        return 'rotate(0)';
                    }
                });
            }

        },

        dragstarted: function (d) {
            $("#top-nav").css("display", "none");
            if (!d3.event.active)
                gInstance.simulation.alphaTarget(0.3).restart();
            d.fx = d.x;
            d.fy = d.y;
        },

        dragged: function (d) {
            $("#top-nav").css("display", "none");
            d.fx = d3.event.x;
            d.fy = d3.event.y;
        },

        dragended: function (d) {
            $("#top-nav").css("display", "none");
            if (!d3.event.active)
                gInstance.simulation.alphaTarget(0);
            // Comment to freez the node position after move
            if (gInstance.coordData === null) {
                d.fx = null;
                d.fy = null;
            }
        },

        createPallete: function () {
            var _piCol = gInstance._graph.color;
            var _patternPath = [
                'M 0 0 L 10 10 M 9 -1 L 11 1 M -1 9 L 1 11',
                'M 0 10 L 10 0 M -1 1 L 1 -1 M 9 11 L 11 9',
                'M 3 0 L 3 10 M 8 0 L 8 10',
                'M 0 3 L 10 3 M 0 8 L 10 8',
                'M 0 3 L 5 3 L 5 0 M 5 10 L 5 7 L 10 7',
                'M 3 3 L 8 3 L 8 8 L 3 8 Z',
                'M 5 5 m -4 0 a 4 4 0 1 1 8 0 a 4 4 0 1 1 -8 0',
                'M 10 3 L 5 3 L 5 0 M 5 10 L 5 7 L 0 7',
                'M 2 5 L 5 2 L 8 5 L 5 8 Z',
                'M 0 0 L 5 10 L 10 0'
            ];

            var _sw = [6, 6, 3, 3, 6, 6, 6, 6, 6, 6];

            d3.selectAll("pattern").remove();

            for (var i = 0; i < _piCol.length; i++) {
                gInstance.pallete.append("pattern")
                        .attr("id", "pattern_" + (i + 1))
                        .attr("patternUnits", "userSpaceOnUse")
                        .attr("width", 10)
                        .attr("height", 10)
                        .append("svg:path")
                        .attr("d", _patternPath[i % _patternPath.length])
                        .attr('fill', "transparent")
                        .style('stroke', _piCol[i])
                        .style('stroke-width', _sw[i]);
            }
        },

        createLinkArrow: function () {
            d3.selectAll("marker").remove();
            var i = 1;
            gInstance.link.each(function (d) {
                var c = (d.target.Id) ? d.target.Id : d.target;
                var r = parseFloat(d3.select("#node_" + c).attr("r"));
                var w = gInstance.getFeatureWidth(d);
                var co = gInstance.getFeatureColor(d);
                gInstance.marker.append("marker")
                        .attr("id", "arrowhead_" + i)
                        .attr("viewBox", "-0 -5 10 10")
                        .attr("refX", (w < 3) ? (r / 2) + 10 : (Math.abs((r / 2) - 5) > 15 ? Math.abs((r / 2) - 5) : Math.abs((r / 2) + 5)) - 2)
                        .attr("refY", 0)
                        .attr("markerUnits", "userSpaceOnUse")
                        .attr("markerWidth", (w < 3) ? 20 : 50)
                        .attr("markerHeight", (w < 3) ? 20 : 50)
                        .attr("orient", "auto")
                        .attr("xoverflow", "visible")
                        .append("svg:path")
                        .attr("d", "M0,-5L10,0L0,5")
                        .attr('fill', co)
                        .style('stroke', 'none');

                d3.select(this).attr("marker-end", "url(#arrowhead_" + i + ")");
                i++;
            });
        },

        nodeMouseOver: function (d) {

            if (!gInstance.showToolTip) {
                return false;
            }

            gInstance.selectedNodeId = d.Id;
            var x = 0.0, y = 0.0, k = 1.0;
            if (gInstance.__transform) {
                x = gInstance.__transform.x;
                y = gInstance.__transform.y;
                k = gInstance.__transform.k;
            }

            d3.select("#tooltip").style("display", "none")
                    .style("left", (x + (d.x * k) + 100) + "px")
                    .style("top", (y + (d.y * k) - 28) + "px");

            gInstance.tooltipDiv.html("");

            var tIDs = gInstance.getAllRowIdsOfANode(d.Id);
            d3.select("#type").attr("value", gInstance._graph.cols.Perf.toString());
            d3.select("#data").attr("value", tIDs.toString());
            d3.select("#folderName").attr("value", gInstance.fl[gInstance.fileIndex]);
            d3.select("#fileName").attr("value", "");
            var form = $("#frm");

            $.ajax({
                type: "POST",
                url: "./datahandler/getperf?t=" + new Date().getTime(),
                data: form.serialize(),
                success: function (json, d) {
                    if (json !== "404") {
                        gInstance.tooltipDiv.transition()
                                .duration(200);

                        var s = "";
                        var _jData = JSON.parse(json);
                        for (var i = 0; i < gInstance._graph.cols.Perf.length; i++) {
                            if (s.length > 0)
                                s += "<br />";
                            s += "<strong>" + gInstance._graph.btn[i].toString() + ":</strong>&nbsp;";

                            var k = [];
                            for (var j = 0; j < _jData.length; j++) {
                                k.push(parseFloat(_jData[j][i]));
                            }
                            k.sort();

                            s += k.toString().replace(/,/g, ", ");
                            ;
                        }

                        d3.select("#tooltip").style("display", "block");
                        gInstance.tooltipDiv.html("<strong>Node id:</strong>&nbsp;" + gInstance.selectedNodeId + "<br /><strong>Total points:</strong>&nbsp;" + tIDs.length +
                                "<br />" + s);
                    }
                },
                error: function (request, err, ex) {
                    alert("Error to post data")
                }
            });
        },

        nodeMouseOut: function () {
            if (!gInstance.showToolTip) {
                return false;
            }

            d3.select("#tooltip").style("display", "none");
            gInstance.tooltipDiv.transition()
                    .duration(500);
            gInstance.tooltipDiv.html("");
            //gInstance.selectedNodeId = -1;
        },

        getFeatureWidth: function (d) {
            if (gInstance.selectedFeature[0]) {
                if (d.W > 2)
                    return (2 + ((d.W - 2) * gInstance.IPEdgeWeight));
                return d.W;
            } else if (gInstance.selectedFeature[1]) {
                if (d.FW > 2)
                    return (2 + ((d.FW - 2) * gInstance.IFEdgeWeight));
                return d.FW;
            }

            return 2;
        },

        getFeatureColor: function (d) {
            if (gInstance.selectedFeature[0]) {
                if (d.W > 2)
                    return d.C;
            } else if (gInstance.selectedFeature[1]) {
                if (d.FW > 2)
                    return d.FC;
            }

            return gInstance.defaultEdgeColor;
        },

        draw: function (index) {
            this.lIndex = index;
            this.dpie = false;
            this.labelIndex = index;

            if (d3.selectAll("#legends")) {
                d3.selectAll("#legends").remove();
                d3.selectAll("linearGradient").remove();
            }

            this.g.remove();
            this.g = this.svg.append("g").attr("class", "everything");

            for (var i = 0; i < this._linkData.length; i++) {
                if (this.EdgeDirChg && this._linkData[i].ED === 0) {
                    var tmp = this._linkData[i].source;
                    this._linkData[i].source = this._linkData[i].target;
                    this._linkData[i].target = tmp;
                    this._linkData[i].ED = 2;
                } else if (!this.EdgeDirChg && this._linkData[i].ED === 2) {
                    var tmp = this._linkData[i].source;
                    this._linkData[i].source = this._linkData[i].target;
                    this._linkData[i].target = tmp;
                    this._linkData[i].ED = 0;
                }
            }

            this.link = this.g.append("g")
                    .attr("class", "links")
                    .selectAll("line")
                    .data(gInstance._linkData)
                    .enter().append("line")
                    .attr("stroke-width", function (d) {
                        return gInstance.getFeatureWidth(d);
                    })
                    .style("stroke", function (d) {
                        return gInstance.getFeatureColor(d);
                    });

            this.node = this.g.append("g")
                    .attr("class", "nodes")
                    .selectAll("circle")
                    .data(gInstance._nodeData)
                    .enter().append("circle")
                    .attr("r", function (d) {
                        return d.Size;
                    })
                    .attr("fill", function (d) {
                        return d.Color[index];
                    })
                    .attr("id", function (d) {
                        return "node_" + d.Id;
                    })
                    .style("cursor", "pointer")
                    .on('contextmenu', function (d) {
                        gInstance.nodeRightClick(d);
                    })
                    .on("mouseover", function (d) {
                        gInstance.nodeMouseOver(d);
                    })
                    .on("mouseout", function (d) {
                        gInstance.nodeMouseOut();
                    })
                    .call(d3.drag()
                            .on("start", gInstance.dragstarted)
                            .on("drag", gInstance.dragged)
                            .on("end", gInstance.dragended));

            /*this.node.append("title")
             .text(function (d) {
             return d.Id + ":" + d.NP;
             });*/

            this.labelText = this.g.append("g")
                    .attr("class", "labels")
                    .selectAll(".mytext")
                    .data(gInstance._nodeData)
                    .enter()
                    .append("text")
                    .text(function (d) {
                        return d.Label[index];
                    })
                    .attr("r", function (d) {
                        return d.Size;
                    })
                    .style("text-anchor", "middle")
                    .style("fill", function (d) {
                        return gInstance.getFontColor(d, index);
                    })
                    .style("font-family", "Arial")
                    .style("font-weight", "bold")
                    .style("cursor", "pointer")
                    .style("font-size", function (d) {
                        return d.Size + "px";
                    })
                    .attr("dy", ".35em")
                    .on("mouseover", function (d) {
                        gInstance.nodeMouseOver(d);
                    })
                    .on("mouseout", function (d) {
                        gInstance.nodeMouseOut();
                    })
                    .on('contextmenu', function (d) {
                        gInstance.nodeRightClick(d);
                    })
                    .call(d3.drag()
                            .on("start", gInstance.dragstarted)
                            .on("drag", gInstance.dragged)
                            .on("end", gInstance.dragended));

            // Enable below code to see edge label
            if (this.shEdgeRank || this.shEdgeSig) {
                this.edgepaths = this.g.append("g")
                        .attr("class", "edgepath").selectAll(".edgepath")
                        .data(gInstance._linkData)
                        .enter()
                        .append('path')
                        .attrs({
                            'class': 'edgepath',
                            'fill-opacity': 0,
                            'stroke-opacity': 0,
                            'id': function (d, i) {
                                return 'edgepath' + i;
                            }
                        })
                        .style("pointer-events", "none");

                this.edgelabels = this.g.append("g")
                        .attr("class", "edgelabel").selectAll(".edgelabel")
                        .data(gInstance._linkData)
                        .enter()
                        .append('text')
                        .style("pointer-events", "none")
                        .attrs({
                            'class': 'edgelabel',
                            'id': function (d, i) {
                                return 'edgelabel' + i;
                            },
                            'font-size': 18,
                            'fill': '#fff'
                        });

                this.edgelabels.append('textPath')
                        .attr('xlink:href', function (d, i) {
                            return '#edgepath' + i;
                        })
                        .style("text-anchor", "middle")
                        .style("pointer-events", "none")
                        .attr("startOffset", "50%")
                        .text(function (d) {
                            return (gInstance.shEdgeRank) ? d.R : d.L;
                        });
            }

            if (this.shEdgeArrow) {
                this.createLinkArrow();
            }

            gInstance.simulation = d3.forceSimulation().nodes(gInstance._nodeData);

            var link_force = d3.forceLink(gInstance._linkData)
                    .id(function (d) {
                        return d.Id;
                    });

            var charge_force = d3.forceManyBody()
                    .strength(gInstance.strength);

            var center_force = d3.forceCenter(gInstance.width / 2, gInstance.height / 2);

            gInstance.simulation
                    .force("charge_force", charge_force)
                    .force("center_force", center_force)
                    .force("links", link_force)
                    ;

            gInstance.simulation.on("tick", gInstance.ticked);

            /*gInstance.simulation.force("link")
             .links(gInstance._linkData);
             
             gInstance.simulation.velocityDecay(0.07);*/

            if (this.__transform) {
                this.g.attr("transform", this.__transform);
            }

            gInstance.simulation.alpha(1).restart();

            this.getCoordinates();

            this.resetFontSize();
        },

        getPieData: function (d) {
            var pc = [];
            var kl = "";
            for (var i = 0; i < d.pie.length; i++) {
                var index = d.pie[i][0] - 1;
                var percentage = d.pie[i][1];

                if (kl.length > 0)
                    kl += ",";
                kl += this._graph.indv[index];
                pc.push({"color": this._graph.color[index], "percent": percentage, "Pattern": "url(#pattern_" + (index + 1) + ")"});
            }

            return [kl, pc];
        },

        drawPie: function () {
            for (var i = 0; i < this._linkData.length; i++) {
                if (this.EdgeDirChg && this._linkData[i].ED === 0) {
                    var tmp = this._linkData[i].source;
                    this._linkData[i].source = this._linkData[i].target;
                    this._linkData[i].target = tmp;
                    this._linkData[i].ED = 2;
                } else if (!this.EdgeDirChg && this._linkData[i].ED === 2) {
                    var tmp = this._linkData[i].source;
                    this._linkData[i].source = this._linkData[i].target;
                    this._linkData[i].target = tmp;
                    this._linkData[i].ED = 0;
                }
            }
            this.dpie = true;

            this.g.remove();
            this.g = this.svg.append("g").attr("class", "everything");

            this.link = this.g.append("g")
                    .attr("class", "links")
                    .selectAll("line")
                    .data(this._linkData)
                    .enter().append("line")
                    .attr("stroke-width", function (d) {
                        return gInstance.getFeatureWidth(d);
                    })
                    /*.attr("marker-end", function (d) {
                     return "url(#arrowhead_" + ((d.W < 5) ? 1 : 2) + ")";
                     })*/
                    .style("stroke", function (d) {
                        return gInstance.getFeatureColor(d);
                    });
            this.node = this.g.append("g")
                    .attr("class", "nodes")
                    .selectAll("g")
                    .data(this._nodeData)
                    .enter().append("g")
                    .attr("r", function (d) {
                        return d.Size;
                    })
                    .attr("id", function (d) {
                        return "node_" + d.Id;
                    })
                    .on('contextmenu', function (d) {
                        gInstance.nodeRightClick(d);
                    })
                    .on("mouseover", function (d) {
                        gInstance.nodeMouseOver(d);
                    })
                    .on("mouseout", function (d) {
                        gInstance.nodeMouseOut();
                    })
                    .call(d3.drag()
                            .on("start", gInstance.dragstarted)
                            .on("drag", gInstance.dragged)
                            .on("end", gInstance.dragended));
            this.node.append("title")
                    .text(function (d) {
                        return d.Label[0];
                    });

            // Enable below code to see edge label
            if (this.shEdgeRank || this.shEdgeSig) {
                this.edgepaths = this.g.append("g")
                        .attr("class", "edgepath").selectAll(".edgepath")
                        .data(this._linkData)
                        .enter()
                        .append('path')
                        .attrs({
                            'class': 'edgepath',
                            'fill-opacity': 0,
                            'stroke-opacity': 0,
                            'id': function (d, i) {
                                return 'edgepath' + i;
                            }
                        })
                        .style("pointer-events", "none");

                this.edgelabels = this.g.append("g")
                        .attr("class", "edgelabel").selectAll(".edgelabel")
                        .data(this._linkData)
                        .enter()
                        .append('text')
                        .style("pointer-events", "none")
                        .attrs({
                            'class': 'edgelabel',
                            'id': function (d, i) {
                                return 'edgelabel' + i;
                            },
                            'font-size': 18,
                            'fill': '#fff'
                        });

                this.edgelabels.append('textPath')
                        .attr('xlink:href', function (d, i) {
                            return '#edgepath' + i;
                        })
                        .style("text-anchor", "middle")
                        .style("pointer-events", "none")
                        .attr("startOffset", "50%")
                        .text(function (d) {
                            return (d.W > 6) ? ((gInstance.shEdgeRank) ? d.R : d.L) : "";
                        });
            }

            if (this.shEdgeArrow) {

                this.createLinkArrow();
            }

            gInstance.simulation = d3.forceSimulation().nodes(gInstance._nodeData);

            var link_force = d3.forceLink(gInstance._linkData)
                    .id(function (d) {
                        return d.Id;
                    });

            var charge_force = d3.forceManyBody()
                    .strength(gInstance.strength);

            var center_force = d3.forceCenter(gInstance.width / 2, gInstance.height / 2);

            gInstance.simulation
                    .force("charge_force", charge_force)
                    .force("center_force", center_force)
                    .force("links", link_force)
                    ;

            gInstance.simulation.on("tick", gInstance.ticked);

            if (this.__transform) {
                this.g.attr("transform", this.__transform);
            }

            /* Draw the respective pie chart for each node */
            gInstance.createPallete();
            this.node.each(function (d) {
                var pieData = gInstance.getPieData(d);

                NodePieBuilder.drawNodePie(d3.select(this), pieData[1], {
                    parentNodeColor: "#fff", //d.Color[0],
                    outerStrokeWidth: 1,
                    showLabelText: false,
                    labelText: pieData[0],
                    labelColor: gInstance._graph.color[0],
                    allowPattern: gInstance.piePattern
                });
            });

            gInstance.simulation.alpha(1).restart();
            this.getCoordinates();
        },

        loadData: function () {

            d3.select("#type").attr("value", "loaddata");
            d3.select("#data").attr("value", "");
            d3.select("#folderName").attr("value", gInstance.fl[gInstance.fileIndex].split(".")[0]);
            d3.select("#fileName").attr("value", gInstance.jfl[gInstance.fileRIndex].files[gInstance.fileCIndex]);
            var form = $("#frm");

            $.ajax({
                type: "POST",
                url: "./datahandler/loaddata?t=" + new Date().getTime(),
                data: form.serialize(),
                success: function (json) {
                    if (json === "404") {
                        gInstance.coordData = null;
                    } else {
                        gInstance.initPage(JSON.parse(json));
                    }
                },
                error: function (request, err, ex) {
                    alert("Error to load data")
                }
            });
        },

        loadDD: function (filename) {
            $("#top-nav").css("display", "none");
            $("#jsonheader").css("display", "block");
            var s = "";
            for (var i = 0; i < this.jfl.length; i++) {
                if (this.jfl[i].name === filename) {
                    s = "";
                    for (var j = 0; j < jfl[i].files.length; j++) {
                        //s += "<a href='javascript:void(0)' class='file-json-select' row='" + i + "' seq='" + j + "' title='" + this.jfl[i].files[j] + "'>" + this.jfl[i].files[j] + "</a>";
                        s += "<option value='[" + i + "," + j + "]' class='file-json-select' title='" + this.jfl[i].files[j] + "'>&nbsp; " + this.jfl[i].files[j] + "</option>";
                    }

                    break;
                }
            }
            if (s.length > 0) {
                s = "<option class='file-select' value=''>&#xf039; &nbsp; Select a graph file</option>" + s;
            }
            $("#myJsonDropdown").html(s);
            $("#myJsonDropdown").on("change", function () {
                //$("#myJsonDropdown a").removeClass("seldw");
                var $opt = $(this).find('option:selected');
                var atr = eval($opt.attr('value'));
                gInstance.fileCIndex = atr[1];//$opt.attr('seq');
                gInstance.fileRIndex = atr[0];//$opt.attr('row');
                //$(this).addClass("seldw");
                //var _file = gInstance.jfl[gInstance.fileRIndex].files[gInstance.fileCIndex];
                //$("#file_json_select").html(_file);
                //$("#myJsonDropdown").hide();
                gInstance.loadData();
            });
        },

        loadFiles: function () {
            $("#top-nav").css("display", "none");
            var s = "";
            for (var i = 0; i < this.fl.length; i++) {
                //s += "<a href='javascript:void(0)' class='file-select' seq='" + i + "'>" + this.fl[i] + "</a>";
                s += "<option value='" + i + "' class='file-select' seq='" + i + "'>&nbsp; " + this.fl[i] + "</option>";
            }
            if (s.length > 0) {
                s = "<option class='file-select' value=''>&#xf039; &nbsp; Select a data file</option>" + s;
            }
            $("#myDropdown").html(s);
            $("#myDropdown").on("change", function () {
                //$("#myDropdown a").removeClass("seldw");
                var $opt = $(this).find('option:selected');
                gInstance.fileIndex = $opt.attr('value');
                //$(this).addClass("seldw");
                var _file = gInstance.fl[gInstance.fileIndex];
                //$("#file_select").html(_file);
                var _fileName = _file.split(".")[0];
                //$("#myDropdown").hide();
                gInstance.loadDD(_fileName);
            });
        }

    };

// Close the dropdown menu if the user clicks outside of it
    window.onclick = function (event) {
        if (!event.target.matches('.dropbtn')) {

            var dropdowns = $(".dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {
                    openDropdown.classList.remove('show');
                }
            }
        }
    };

    $("#colDropdown span.anchor").on("click", function () {
        $("#colDropdown ul.items").toggle();
    });

    /*$(".dropbtn").on("click", function () {
     $("#myJsonDropdown").hide();
     $("#myDropdown").toggle();
     });
     
     $(".dropjbtn").on("click", function () {
     $("#myDropdown").hide();
     $("#myJsonDropdown").toggle();
     //$("#myJsonDropdown .dropdown-content").toggleClass("show");
     });*/

    $(".path-show").click(function (e) {
        e.preventDefault();
        $("#int-path .row").not("#int-path .row1").slideUp();
        $("#int-path .row1").slideToggle("slow", function () {
            if ($(this).css("display") === "none") {
                $("#fa-path-title").removeClass('fa-angle-double-down');
                $("#fa-path-title").addClass('fa-angle-double-up');
            } else {
                $("#fa-path-title").removeClass('fa-angle-double-up');
                $("#fa-path-title").addClass('fa-angle-double-down');
            }

        });
    });

    $(".flare-show").click(function (e) {
        e.preventDefault();
        $("#int-flare .row").not("#int-flare .row1").slideUp();
        $("#int-flare .row1").slideToggle("slow", function () {
            if ($(this).css("display") === "none") {
                $("#fa-flare-title").removeClass('fa-angle-double-down');
                $("#fa-flare-title").addClass('fa-angle-double-up');
            } else {
                $("#fa-flare-title").removeClass('fa-angle-double-up');
                $("#fa-flare-title").addClass('fa-angle-double-down');
            }

        });
    });

    $(".cc-show").click(function (e) {
        e.preventDefault();
        $("#int-cc .row").not("#int-cc .row1").slideUp();
        $("#int-cc .row1").slideToggle("slow", function () {
            if ($(this).css("display") === "none") {
                $("#fa-cc-title").removeClass('fa-angle-double-down');
                $("#fa-cc-title").addClass('fa-angle-double-up');
            } else {
                $("#fa-cc-title").removeClass('fa-angle-double-up');
                $("#fa-cc-title").addClass('fa-angle-double-down');
            }

        });
    });

    $(".node-show").click(function (e) {
        e.preventDefault();
        $("#int-nodes .row").not("#int-nodes .row1").slideUp();
        $("#int-nodes .row1").slideToggle("slow", function () {
            if ($(this).css("display") === "none") {
                $("#fa-node-title").removeClass('fa-angle-double-down');
                $("#fa-node-title").addClass('fa-angle-double-up');
            } else {
                $("#fa-node-title").removeClass('fa-angle-double-up');
                $("#fa-node-title").addClass('fa-angle-double-down');
            }

        });
    });

    var gInstance = new Graph(fl, jfl);
    gInstance.loadFiles();

});

/*
 * The following code copied from: 
 * http://rosettacode.org/wiki/LZW_compression#JavaScript
 */
var LZW = {
    compress: function (uncompressed) {
        "use strict";
        // Build the dictionary.
        var i,
                dictionary = {},
                c,
                wc,
                w = "",
                result = [],
                dictSize = 256;
        for (i = 0; i < 256; i += 1) {
            dictionary[String.fromCharCode(i)] = i;
        }

        for (i = 0; i < uncompressed.length; i += 1) {
            c = uncompressed.charAt(i);
            wc = w + c;
            //Do not use dictionary[wc] because javascript arrays 
            //will return values for array['pop'], array['push'] etc
            // if (dictionary[wc]) {
            if (dictionary.hasOwnProperty(wc)) {
                w = wc;
            } else {
                result.push(dictionary[w]);
                // Add wc to the dictionary.
                dictionary[wc] = dictSize++;
                w = String(c);
            }
        }

        // Output the code for w.
        if (w !== "") {
            result.push(dictionary[w]);
        }
        return result;
    },

    decompress: function (compressed) {
        "use strict";
        // Build the dictionary.
        var i,
                dictionary = [],
                w,
                result,
                k,
                entry = "",
                dictSize = 256;
        for (i = 0; i < 256; i += 1) {
            dictionary[i] = String.fromCharCode(i);
        }

        w = String.fromCharCode(compressed[0]);
        result = w;
        for (i = 1; i < compressed.length; i += 1) {
            k = compressed[i];
            if (dictionary[k]) {
                entry = dictionary[k];
            } else {
                if (k === dictSize) {
                    entry = w + w.charAt(0);
                } else {
                    return null;
                }
            }

            result += entry;

            // Add w+entry[0] to the dictionary.
            dictionary[dictSize++] = w + entry.charAt(0);

            w = entry;
        }
        return result;
    }
};

/*
 * 
 
 
 
 
 
 
 
 
 
 
 
 function displayAnalysis() {
 if (_analysis.length > 0) {
 d3.select("#node_analysis").style("display", "block");
 } else {
 d3.select("#node_analysis").style("display", "none");
 }
 }
 
 function intersection(a1, a2) {
 var a = [];
 
 if (a1.length === 0 || a2.length === 0)
 return a;
 
 for (var i = 0; i < a1.length; i++) {
 if (a2.indexOf(a1[i]) >= 0 && a.indexOf(a1[i]) < 0) {
 a.push(a1[i]);
 }
 }
 
 return a;
 }
 
 function getPoints(_id) {
 var a = [];
 for (var i = 0; i < _graph.nodes.length; i++) {
 if (_graph.nodes[i].Id === _id) {
 var _pt = _graph.nodes[i].Ph;
 for (var j = 0; j < _pt.length; j++) {
 for (var k = _pt[j][0]; k <= _pt[j][1]; k++) {
 a.push(k);
 }
 }
 }
 }
 return a;
 }
 
 function AnalyzeNodes() {
 if (_analysis.length > 0) {
 var _ptHash = new HashMap();
 var _json = {'nodes': [], 'links': []};
 
 for (var i = 0; i < _analysis.length; i++) {
 var _lbl = null, _col = null;
 for (var g = 0; g < _graph.nodes.length; g++) {
 if (_graph.nodes[g].Id === _analysis[i]) {
 _lbl = _graph.nodes[g].Label;
 _col = _graph.nodes[g].Color;
 break;
 }
 }
 
 var ptVal = getPoints(_analysis[i]);
 
 if (_ptHash.get(_analysis[i]) === null) {
 _ptHash.put(_analysis[i], ptVal);
 }
 
 _json.nodes.push({'node': i, 'name': "node" + _analysis[i], 'NP': _ptHash.get(_analysis[i]).length, 'label': _lbl, 'Color': _col});
 
 }
 
 for (var i = 0; i < _analysis.length; i++) {
 for (var j = i + 1; j < _analysis.length; j++) {
 var a = intersection(_ptHash.get(_analysis[i]), _ptHash.get(_analysis[j]));
 
 if (a.length > 0) {
 _json.links.push({'source': i, 'target': j, 'value': a.length});
 }
 }
 }
 
 //alert(_json);
 
 //return 0;
 
 //d3.select("#data").attr("value", _json);
 //var form = $("#frm");
 
 if (d3.select("#modal-init")) {
 d3.select("#modal-init").style("display", "none");
 }
 
 vm.model.$modal();
 loadSankeyJS(_json);
 }
 
 return false;
 }
 
 function loadSankeyJS(graph) {
 if (graph === null)
 return;
 
 $("#iframe-container").html("");
 
 var units = "points";
 
 // set the dimensions and margins of the graph
 var margin = {top: 10, right: 10, bottom: 10, left: 10},
 _width = (width * .8) - margin.left - margin.right,
 _height = (height * .7) - margin.top - margin.bottom;
 
 // format variables
 var formatNumber = d3.format(",.0f"), // zero decimal places
 format = function (d) {
 return formatNumber(d) + " " + units;
 },
 color = d3.scaleOrdinal(d3.schemeCategory20);
 
 // append the svg object to the body of the page
 var sSvg = d3.select("#iframe-container").append("svg")
 .attr("width", _width + margin.left + margin.right)
 .attr("height", _height + margin.top + margin.bottom)
 .attr("id", "tSVG")
 .append("g")
 .attr("transform",
 "translate(" + margin.left + "," + margin.top + ")");
 
 // Set the sankey diagram properties
 sankey = d3.sankey()
 .nodeWidth(36)
 .nodePadding(40)
 .size([_width, _height]);
 
 path = sankey.link();
 
 // load the data
 //d3.json("sankey.json", function(error, graph) {
 
 sankey
 .nodes(graph.nodes)
 .links(graph.links)
 .layout(32);
 
 // add in the links
 slink = sSvg.append("g").selectAll(".link")
 .data(graph.links)
 .enter().append("path")
 .attr("class", "link")
 .attr("d", path)
 .style("stroke-width", function (d) {
 return Math.max(1, d.dy);
 })
 .style("fill", "none")
 .style("stroke", "#000")
 .style("stroke-opacity", "0.2")
 .sort(function (a, b) {
 return b.dy - a.dy;
 });
 
 // add the link titles
 slink.append("title")
 .text(function (d) {
 return d.source.name + " → " +
 d.target.name + "\n" + format(d.value);
 });
 
 // add in the nodes
 var node = sSvg.append("g").selectAll(".node")
 .data(graph.nodes)
 .enter().append("g")
 .attr("class", "node")
 .attr("transform", function (d) {
 return "translate(" + d.x + "," + d.y + ")";
 })
 .call(d3.drag()
 .subject(function (d) {
 return d;
 })
 .on("start", function () {
 this.parentNode.appendChild(this);
 })
 .on("drag", dragmoveSankey));
 
 // add the rectangles for the nodes
 node.append("rect")
 .attr("height", function (d) {
 return d.dy <= 0 ? 75 : d.dy;
 })
 .attr("width", sankey.nodeWidth())
 .style("fill", function (d) {
 return d.Color[lIndex];
 })
 .style("stroke", function (d) {
 return d3.rgb(d.Color[lIndex]).darker(2);
 })
 .style("cursor", "move")
 .style("fill-opacity", "0.9")
 .style("shape-rendering", "crispEdges")
 .append("title")
 .text(function (d) {
 return d.name + "\n" + format(d.NP);
 });
 
 // add in the title for the nodes
 node.append("text")
 .attr("x", -6)
 .attr("y", function (d) {
 return d.dy / 2;
 })
 .attr("dy", ".35em")
 .attr("text-anchor", "end")
 .attr("transform", null)
 .style("pointer-events", "none")
 .style("text-shadow", "0 1px 0 #fff")
 .text(function (d) {
 return d.name + "(" + d.label[lIndex] + ")";
 })
 .filter(function (d) {
 return d.x < _width / 2;
 })
 .attr("x", 6 + sankey.nodeWidth())
 .attr("text-anchor", "start");
 
 
 $("#tsvg .link").hover(function () {
 $(this).css("stroke-opacity", "0.5");
 });
 
 }
 
 // the function for moving the nodes
 function dragmoveSankey(d) {
 d3.select(this)
 .attr("transform",
 "translate("
 + d.x + ","
 + (d.y = Math.max(
 0, Math.min(height - d.dy, d3.event.y))
 ) + ")");
 sankey.relayout();
 slink.attr("d", path);
 }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 */