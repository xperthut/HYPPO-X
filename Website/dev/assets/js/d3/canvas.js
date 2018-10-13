$(function () {
    var canvas = document.querySelector("canvas"),
            context = canvas.getContext("2d"),
            width = canvas.width,
            height = canvas.height;

    //add zoom capabilities 
    var zoom_handler = d3.zoom()
            .scaleExtent([1 / 5, 10])
            .on("zoom", zoom_actions);

    zoom_handler(canvas);

    //var color = d3.scaleOrdinal(d3.schemeCategory20);

    var simulation = d3.forceSimulation()
            .velocityDecay(0.08)
            .force("link", d3.forceLink().id(function (d) {
                return d.Id;
            }).distance(50).strength(1))
            .force("charge", d3.forceManyBody().strength(-240))
            .force("center", d3.forceCenter(width / 2, height / 2))
            .force("x", d3.forceX(width / 2).strength(.05))
            .force("y", d3.forceY(height / 2).strength(.05));

    draw("assets/Data/" + fl[0]);
    function draw(f) {

        d3.json(f, function (error, graph) {
            if (error)
                throw error;

            simulation
                    .nodes(graph.nodes)
                    .on("tick", ticked);

            simulation.force("link")
                    .links(graph.links);

            d3.select(canvas)
                    .call(d3.drag()
                            .container(canvas)
                            .subject(dragsubject)
                            .on("start", dragstarted)
                            .on("drag", dragged)
                            .on("end", dragended));

            function ticked() {
                context.clearRect(0, 0, width, height);

                context.beginPath();
                graph.links.forEach(drawLink);
                context.strokeStyle = "#aaa";
                context.stroke();

                context.beginPath();
                graph.nodes.forEach(drawNode);
                context.fill();
                context.strokeStyle = "#fff";
                context.stroke();
            }

            function dragsubject() {
                return simulation.find(d3.event.x, d3.event.y);
            }
        });
    }

    function zoom_actions() {
        canvas.attr("transform", d3.event.transform)
    }

    function dragstarted() {
        if (!d3.event.active)
            simulation.alphaTarget(0.3).restart();
        d3.event.subject.fx = d3.event.subject.x;
        d3.event.subject.fy = d3.event.subject.y;
    }

    function dragged() {
        d3.event.subject.fx = d3.event.x;
        d3.event.subject.fy = d3.event.y;
    }

    function dragended() {
        if (!d3.event.active)
            simulation.alphaTarget(0);
        d3.event.subject.fx = null;
        d3.event.subject.fy = null;
    }

    function drawLink(d) {
        context.moveTo(d.source.x, d.source.y);
        context.lineTo(d.target.x, d.target.y);
    }

    function drawNode(d) {
        context.moveTo(d.x + 3, d.y);
        context.arc(d.x, d.y, 3, 0, 2 * Math.PI);
    }

    function loadGraph(f) {
        g.remove();
        draw(f);
    }

    function loadDD() {
        var s = "";
        for (var i = 0; i < fl.length; i++) {
            s += "<a href='javascript:void(0)' class='file-select' seq='" + i + "'>" + fl[i] + "</a>";
        }
        $("#myDropdown").html(s);

        $(".file-select").on("click", function () {
            var index = $(this).attr('seq');
            loadGraph("assets/Data/" + fl[index]);
        });
    }

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
    }

    $(".dropbtn").on("click", function () {
        $("#myDropdown").toggleClass("show");
    });

    loadDD();
});