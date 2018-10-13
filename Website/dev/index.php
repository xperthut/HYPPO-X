<?php
$demo = false;

function getAllCsvFiles() {
    $fList = scandir("Data/csv", 1);
    $fn = "[";

    foreach ($fList as $file) {

        if (strcmp(".", substr($file, 0, 1)) != 0) {
            if (strlen($fn) > 1) {
                $fn .= ",";
            }
            $fn .= '"' . $file . '"';
        }
    }
    $fn .= "]";

    return $fn;
}

function getAllJsonFiles() {
    $fList = scandir("Data/json", 1);
    $_f = "[";
    foreach ($fList as $file) {

        if (strcmp(".", substr($file, 0, 1)) != 0) {
            if (strlen($_f) > 1){
                $_f .= ",";
            }

            $_f .= "{'name':'$file', 'files':[";

            $jfList = scandir("Data/json/" . $file, 1);

            $_jsonFile = "";
            foreach ($jfList as $jfile) {
                if (strcmp(".", substr($jfile, 0, 1)) != 0 && strcmp("coord_", substr($jfile, 0, 6)) != 0) {
                    if (strlen($_jsonFile) > 0){
                        $_jsonFile .= ",";
                    }
                    $_jsonFile .= "'" . $jfile . "'";
                }
            }
            $_f .= $_jsonFile . "]}";
        }
    }
    $_f .= "]";

    return $_f;
}
?>

<!DOCTYPE HTML>

<html>
    <head>
        <title>Hyppo-X</title> 
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <!--[if lte IE 8]><script src="assets/js/ie/html5shiv.js"></script><![endif]-->
        <link rel="stylesheet" href=https://use.fontawesome.com/releases/v5.0.8/css/all.css?t=<?php echo rand(-1000000, 10000000); ?>" />
        <link rel="stylesheet" href="assets/css/main.css?t=<?php echo rand(-1000000, 10000000); ?>" />
        <link rel="stylesheet" href="assets/css/dropdown.css?t=<?php echo rand(-1000000, 10000000); ?>" />
        <link rel="stylesheet" href="assets/css/spectrum.css?t=<?php echo rand(-1000000, 10000000); ?>" />
        <link rel="shortcut icon" href="images/logo.png"  />
        <!--[if lte IE 8]><link rel="stylesheet" href="assets/css/ie8.css?t=<?php echo rand(-1000000, 10000000); ?>" /><![endif]-->
        <!--[if lte IE 9]><link rel="stylesheet" href="assets/css/ie9.css?t=<?php echo rand(-1000000, 10000000); ?>" /><![endif]-->
        <noscript><link rel="stylesheet" href="assets/css/noscript.css?t=<?php echo rand(-1000000, 10000000); ?>" /></noscript>
    </head>
    <body class="is-loading-0 is-loading-1 is-loading-2"> 
        <script>
            var fl = <?php echo getAllCsvFiles(); ?>;
            var jfl = <?php echo getAllJsonFiles(); ?>;
        </script>
        <form style="display: none" id="frm">
            <input type="hidden" id="type" name="type" />
            <input type="hidden" id="data" name="data" />
            <input type="hidden" id="folderName" name="folderName" />
            <input type="hidden" id="fileName" name="fileName" />
        </form>
        <!-- Drop down-->
        <nav id="top-nav">
            <ul class="dropdown-menu">
                <li><a id="pie_chart" href="javascript:void(0)">Pie chart analysis</a></li>
            </ul>
        </nav>
        <!-- Main -->
        <div id="main">
            <!-- Select csv file -->
            <section id="header">
                <div class="logohead">
                    <div class="dropdown">
                        <button class="dropbtn" id="file_select">Select a data file</button>
                        <div id="myDropdown" class="dropdown-content"></div>
                    </div>
                </div>
            </section>
            
            <!-- Select JSON file -->
            <section id="jsonheader">
                <div class="logohead">
                    <div class="dropdown">
                        <button class="dropjbtn" id="file_json_select">Select a graph file</button>
                        <div id="myJsonDropdown" class="dropdown-content"></div>
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

            <!-- subgraphs -->
            <section id="int-cc">
                <div class="cc-show show"><i class="fas fa-angle-double-up" id="fa-cc-title"></i>&nbsp;Manage subgraph</div>
                <div class="row1 row" id="cc-details"></div>
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
                    <li>Washington State University</li>

                </ul>
            </footer>
        </div>
        <!-- Scripts -->
        <script src="assets/js/jquery.min.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="assets/js/skel.min.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <!--[if lte IE 8]><script src="assets/js/ie/respond.min.js?t=<?php echo rand(-1000000, 10000000); ?>"></script><![endif]-->
        <script src="assets/js/main.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script type="text/javascript">
                        $('.toggle').click(function () {
                            $('.caption').toggle();
                        })
        </script>
        <script src="https://d3js.org/d3.v4.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="assets/js/d3/node-pie.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="assets/js/d3/graph.js?t=kajkas"></script>
        <script src="assets/js/d3-selection-multi.v1.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="assets/js/spectrum.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        
        <script src="https://unpkg.com/d3-require@0.4.5/build/d3-require.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="https://unpkg.com/d3-let@0.3.2/build/d3-let.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="https://unpkg.com/d3-view@0.9.3/build/d3-view.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="assets/js/d3/modal.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
        <script src="assets/js/d3/sankey.js?t=<?php echo rand(-1000000, 10000000); ?>"></script>
    </body>
</html>