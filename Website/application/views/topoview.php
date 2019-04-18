<!DOCTYPE HTML>

<html>
    <head>
        <title>Hyppo-X</title> 
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <!--[if lte IE 8]><script src="<?php echo assets_url(); ?>js/ie/html5shiv.js"></script><![endif]-->
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.0.8/css/all.css" />
        <link rel="stylesheet" href="<?php echo assets_url(); ?>css/main.css?t=<?php echo $rand; ?>" />
        <link rel="stylesheet" href="<?php echo assets_url(); ?>css/dropdown.css?t=<?php echo $rand; ?>" />
        <link rel="stylesheet" href="<?php echo assets_url(); ?>css/spectrum.css?t=<?php echo $rand; ?>" />
        <link rel="shortcut icon" href="<?php echo images_url(); ?>logo.png"  />
        <!--[if lte IE 8]><link rel="stylesheet" href="<?php echo assets_url(); ?>css/ie8.css?t=<?php echo $rand; ?>" /><![endif]-->
        <!--[if lte IE 9]><link rel="stylesheet" href="<?php echo assets_url(); ?>css/ie9.css?t=<?php echo $rand; ?>" /><![endif]-->
        <noscript><link rel="stylesheet" href="<?php echo assets_url(); ?>css/noscript.css?t=<?php echo $rand; ?>" /></noscript>
    </head>
    <body class="is-loading-0 is-loading-1 is-loading-2"> 
        <script>
            var fl = <?php echo $fileList; ?>;
            var jfl = <?php echo $toFileList; ?>;
        </script>
        <form style="display: none" id="frm">
            <input type="hidden" id="type" name="type" />
            <input type="hidden" id="data" name="data" />
            <input type="hidden" id="folderName" name="folderName" />
            <input type="hidden" id="fileName" name="fileName" />
        </form>
        <!-- Drop down-->
        <nav id="top-nav" style="display: none">
            <ul class="dropdown-menu">
                <li><a id="pie_chart" href="javascript:void(0)">Pie chart analysis</a></li>
            </ul>
        </nav>
        <!-- Mouse over-->
        <div id="tooltip"></div>
        <!-- Main -->
        <div id="main">
            <!-- Select csv file -->
            <section id="header">
                <div class="logohead">
                    <div class="dropdown">
                        <!--<button class="dropbtn" id="file_select">Select a data file</button>
                        <div id="myDropdown" class="dropdown-content"></div> -->
                        <select class="dropdown-content" name="myDropdown" id="myDropdown" style='font-family:Arial, FontAwesome;'></select>
                    </div>
                </div>
            </section>

            <!-- Select JSON file -->
            <section id="jsonheader">
                <div class="logohead">
                    <div class="dropdown">
                        <!--<button class="dropjbtn" id="file_json_select">Select a graph file</button>
                        <div id="myJsonDropdown" class="dropdown-content"></div>-->
                        <select class="dropdown-content" name="myJsonDropdown" id="myJsonDropdown" style='font-family:Arial, FontAwesome;'></select>
                    </div>
                </div>
            </section>

            <!-- pie chart legends -->
            <section id="pie-legend">
                <div class="legendhead"></div>
            </section>

            <!-- buttons -->
            <section id="thumbnails">
                <div id="attr-btn"></div>
                <div id="other-btn"></div>
            </section>

            <!-- attribute controller -->
            <section id="attr-ctrl">
                <div id="attr-details"></div>
            </section>

            <!-- interesting paths -->
            <section id="int-path">
                <div class="path-show show"><i class="fas fa-angle-double-up" id="fa-path-title"></i>&nbsp;Manage paths</div>
                <div class="row1 row" id="path-details"></div>
            </section>

            <!-- interesting paths -->
            <section id="int-flare">
                <div class="flare-show show"><i class="fas fa-angle-double-up" id="fa-flare-title"></i>&nbsp;Manage flares</div>
                <div class="row1 row" id="flare-details"></div>
            </section>

            <!-- subgraphs -->
            <section id="int-cc">
                <div class="cc-show show"><i class="fas fa-angle-double-up" id="fa-cc-title"></i>&nbsp;Manage subgraph</div>
                <div class="row1 row" id="cc-details"></div>
            </section>

            <!-- Node analysis -->
            <section id="int-nodes">
                <div class="node-show show"><i class="fas fa-angle-double-up" id="fa-node-title"></i>&nbsp;Selected Nodes</div>
                <div class="row1 row" id="node-details">
                    <fieldset>
                        <legend>Selected Node IDs</legend>
                        <span id="nIDs"></span>
                        <button id="clearNIDs" style="display: none;">Clear selection</button>
                    </fieldset>
                    <fieldset>
                        <legend>Selected attributes</legend>
                        <div class="dropdown">
                            <div id="colDropdown" class="dropdown-check-list">
                                <span class='anchor'>Select data attributes</span><ul class='items'></ul>
                            </div>
                        </div>
                        <div id="attrList"><span>Selected attributes are: </span><ul></ul></div>
                    </fieldset>
                    <fieldset>
                        <legend>Selected an action</legend>
                        <button id="wdtf-btn">Write data to file</button>
                    </fieldset>
                </div>
            </section>

            <section id="instruction">
                <div class="details">
                    <span>Instructions</span>
                    <ol>
                        <li>Select a CSV data file</li>
                        <li>Select a graph file to visualize</li>
                        <li>Mark/unmark to show/hide a connected component</li>
                        <li>Mark/unmark to show/hide a paths</li>
                        <li>Mark/unmark to show/hide a flares</li>
                        <li>Click on color box to change the color</li>
                        <li>Use mouse scroll or touch pad for pan and zoom.</li>
                        <li>Mouse left click on a node and drag to change it's position.</li>
                        <li>Mouse left click on a outside the graph and drag to change it's position.</li>
                        <li>Mouse right click on a node to add/remove that node for further analysis.</li>
                    </ol>
                </div>
            </section>

            <!-- Footer -->
            <footer id="footer">
                <ul class="copyright text-center">
                    <li>&copy; 2018&ndash;<script>document.write((new Date()).getFullYear());</script> Methun</li>
                    <li>Kamruzzaman</li>

                </ul>
            </footer>
        </div>
        <!-- Scripts -->
        <script src="<?php echo assets_url(); ?>js/jquery.min.js?t=<?php echo $rand; ?>"></script>
        <script src="<?php echo assets_url(); ?>js/skel.min.js?t=<?php echo $rand; ?>"></script>
        <!--[if lte IE 8]><script src="<?php echo assets_url(); ?>js/ie/respond.min.js?t=<?php echo $rand; ?>"></script><![endif]-->
        <script src="<?php echo assets_url(); ?>js/main.js?t=<?php echo $rand; ?>"></script>
        <script type="text/javascript">
                        $('.toggle').click(function () {
                            $('.caption').toggle();
                        })
        </script>
        <script src="https://d3js.org/d3.v4.js"></script>
        <script src="<?php echo assets_url(); ?>js/d3/node-pie.js?t=<?php echo $rand; ?>"></script>
        <script src="<?php echo assets_url(); ?>js/d3/graph.js?t=<?php echo $rand; ?>"></script>
        <script src="<?php echo assets_url(); ?>js/d3-selection-multi.v1.js?t=<?php echo $rand; ?>"></script>
        <script src="<?php echo assets_url(); ?>js/spectrum.js?t=<?php echo $rand; ?>"></script>

        <script src="https://unpkg.com/d3-require@0.4.5/build/d3-require.js"></script>
        <script src="https://unpkg.com/d3-let@0.3.2/build/d3-let.js"></script>
        <script src="https://unpkg.com/d3-view@0.9.3/build/d3-view.js"></script>
        <script src="<?php echo assets_url(); ?>js/d3/modal.js?t=<?php echo $rand; ?>"></script>
        <script src="<?php echo assets_url(); ?>js/d3/sankey.js?t=<?php echo $rand; ?>"></script>
    </body>
</html>