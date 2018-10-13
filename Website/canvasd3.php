<?php
function getAllFiles() {
    $fList = scandir("assets/Data", 1);
    $fn = "[";
    foreach ($fList as $file) {
        if (strcmp(".", substr($file, 0, 1)) != 0) {
            if(strlen($fn)>1) $fn .= ",";
            $fn .= '"'.$file.'"';
        }
    }
    $fn .="]";
    
    return $fn;
}
?>

<!DOCTYPE HTML>

<html>
    <head>
        <title>HyppoX</title>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <!--[if lte IE 8]><script src="assets/js/ie/html5shiv.js"></script><![endif]-->
        <link rel="stylesheet" href="assets/css/main.css" />
        <link rel="stylesheet" href="assets/css/dropdown.css" />
        <link rel="shortcut icon" href="favicon.ico"  />
        <!--[if lte IE 8]><link rel="stylesheet" href="assets/css/ie8.css" /><![endif]-->
        <!--[if lte IE 9]><link rel="stylesheet" href="assets/css/ie9.css" /><![endif]-->
        <noscript><link rel="stylesheet" href="assets/css/noscript.css" /></noscript>
    </head>
    <body class="is-loading-0 is-loading-1 is-loading-2"> 
        <script>
            var fl = <?php echo getAllFiles(); ?>;
        </script>
        <!-- Main -->
        <div id="main">

            <!-- Header -->
            <header id="header">
                <div class="logohead">
                    <div class="dropdown">
                        <button class="dropbtn">Select file</button>
                        <div id="myDropdown" class="dropdown-content"></div>
                    </div>
                </div>
            </header>

            <!-- Thumbnail -->
            <section id="thumbnails">Thumbnail images are here.</section>

            <!-- Footer -->
            <footer id="footer">
                <ul class="copyright text-center">
                    <li>&copy; 2018&ndash;<script>document.write((new Date()).getFullYear());</script> Methun</li>
                    <li>Washington State University</li>

                </ul>
            </footer>
        </div>
        <!-- Scripts -->
        <script src="assets/js/jquery.min.js"></script>
        <script src="assets/js/skel.min.js"></script>
        <!--[if lte IE 8]><script src="assets/js/ie/respond.min.js"></script><![endif]-->
        <script src="assets/js/main.js"></script>
        <script type="text/javascript">
                        $('.toggle').click(function () {
                            $('.caption').toggle();
                        })
        </script>
        <script src="assets/js/d3/d3.min.js"></script>
        <script src="assets/js/d3/canvas.js"></script>
    </body>
</html>