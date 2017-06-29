Canvas
======
The `HTML 5` object named `canvas` is used here to represent graph more meaningfully using `pie chart` and color. 

### Input
The output of our graph plotting project using `R` is saved in an `eps` formatted image file. Later, we converted this image file from `eps` to `png`, which is used as a background image of this project. We plotted pie chart on the node position of the background image. The `JSON` data to generate `pie chart` was generated from our main `C++` project, where the constant in the configuration file named `PIE_CHART_CODE` controlls whether the code will generate `JSON` data or not.

### Output
Run the `HTML` file in any `HTML 5` supported browser `Opera, Firefox, and Safari`. One can save the image in a `png` formatted file.


##### HTML template to generate `Pie-chart`
```html
<html>
	<head>
		<script type="text/javascript">
			function drawText(_canvas, text, position, style){
				var context = _canvas.getContext('2d');

				context.font = style.font;//'bold 10pt Calibri';
				context.fillStyle = style.color;
  				context.fillText(text, (position.x1+position.x2)/2, (position.y1+position.y2)/2);
			}

			function drawPie(_canvas, position, data, _id){
				var total = 0;
				  for (obj of data) {
				    total += obj.value;
				  }

				  var ctx = _canvas.getContext('2d');
				  var previousRadian;
				  var middle = {
				    x: position.x,
				    y: 777-position.y,
				    radius: position.rad,
				  };
				  
				   //background
				  ctx.beginPath();
				  ctx.arc(middle.x, middle.y, middle.radius, 0, 2 * Math.PI);
				  ctx.closePath();
				  ctx.stroke();
				  ctx.fillStyle = "black";
				  ctx.fill();
				   //end of background

				  for (obj of data) {
				    previousRadian = previousRadian || 0;
				    obj.percentage = parseInt((obj.value / total) * 100)
				    
				    ctx.beginPath();
				    ctx.fillStyle = obj.color;
				    obj.radian = (Math.PI * 2) * (obj.value / total);
				    ctx.moveTo(middle.x, middle.y);
				    ctx.arc(middle.x, middle.y, middle.radius - 2, previousRadian, previousRadian + obj.radian, false);
				    ctx.closePath();
				    ctx.fill();
				    ctx.save();
				    ctx.translate(middle.x, middle.y);
				    ctx.fillStyle = "white";
				    ctx.font = middle.radius / 10 + "px Arial";
				    ctx.rotate(previousRadian + obj.radian);
				  
				    ctx.restore();

				    previousRadian += obj.radian;
				  }

			}
		</script>
	</head>
	<body>
		<!-- Adjust the canvas width and height. It is a viewport. -->
		<canvas id="myCanvas" width="[CANVAS width]" height="[CANVAS HEIGHT]"></canvas>
		<script type="text/javascript">
			var canvas = document.getElementById('myCanvas');

			// Place the JSON data here which is generated in our main 'C++' project
 			// var data=JSON data


			// Load background image
			// If image is under same directory of the html file then use ./filename.png
			var background = new Image();
			background.src = "[PNG file with path]";
			
			// Make sure the image is loaded first otherwise nothing will draw.
			background.onload = function(){
				var ctx = canvas.getContext('2d');
				ctx.drawImage(background,0,0,[WIDTH],[HEIGHT]);

				for(k=0;k<data.length;k++){
				 	var position = data[k].p;
				 	drawPie(canvas, position, data[k].d, data[k].id);
				}

				// Save as png
				window.location = canvas.toDataURL("image/png");
			}
		</script>
	</body>

</html>
```