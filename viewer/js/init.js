var selection = null;

$(function(){
	function plotChangesHistogram(data) {
		var x = [];
		var y = [];

		for (var i = 0; i < data.changes.length; i++) {
			x[i] = "" + i;
			y[i] = data.changes[i];
		}

		var trace = [{
			x: x,
			y: y,
			type: 'bar'
		}];

		var layout = {
			title: {
				text:'Bit Changes (~)',
				font: {
					family: 'Courier New, monospace',
					size: 24
				},
				xref: 'paper',
				x: 0.05,
			},
			xaxis: {
				title: {
					text: 'Bits',
					font: {
						family: 'Courier New, monospace',
						size: 18,
						color: '#7f7f7f'
					}
				},
			},
			yaxis: {
				title: {
					text: 'Variation per operation',
					font: {
						family: 'Courier New, monospace',
						size: 18,
						color: '#7f7f7f'
					}
				}
			}
		};

		Plotly.newPlot('changesHist', trace, layout, {showSendToCloud: false});
	}

	function plotFreqHistogram(data) {
		var x = [];
		var y = [];
		var sum = 0.0;

		for (var i = 0; i < data.freq.length; i++) {
			x[i] = "" + (i - 64);
			sum += data.freq[i];
		}

		for (var i = 0; i < data.freq.length; i++)
			y[i] = data.freq[i]/sum;

		var trace = [{
			x: x,
			y: y,
			type: 'bar'
		}];

		var layout = {
			title: {
				text:'Exponent range (%)',
				font: {
					family: 'Courier New, monospace',
					size: 24
				},
				xref: 'paper',
				x: 0.05,
			},
			xaxis: {
				title: {
					text: 'Exponent (2**E)',
					tickmode: 'linear',
					tick0: -63,
					dtick: 4,
					font: {
						family: 'Courier New, monospace',
						size: 18,
						color: '#7f7f7f'
					}
				},
			},
			yaxis: {
				title: {
					text: 'Frequency',
					tickmode: 'linear',
					font: {
						family: 'Courier New, monospace',
						size: 18,
						color: '#7f7f7f'
					}
				}
			}
		};

		Plotly.newPlot('freqHist', trace, layout, {showSendToCloud: false});
	}

	function plotUtilizationHistogram(data) {
		var x = [];
		var y = [];

		for (var i = 0; i < data.bitmap.length; i++) {
			x[i] = "" +i;
			y[i] = data.bitmap[i];
		}

		var trace = [{
			x: x,
			y: y,
			type: 'bar'
		}];


		var title = y.join("");
		if (data.iwl < 0)
			title = "[." + " ".repeat(-data.iwl) + title + "]";
		else
			title = "[" + title.slice(0, data.iwl) + "." + title.slice(data.iwl) + "]";

		title = 'Bits - ' + title + "- iwl: " + data.iwl + ", wl: " + data.wl;

		var layout = {
			title: {
				text: 'Bit utilization',
				font: {
					family: 'Courier New, monospace',
					size: 24
				},
				xref: 'paper',
				x: 0.05,
			},
			xaxis: {
				title: {
					text: title,
					tickmode: 'linear',
					tick0: -63,
					dtick: 4,
					font: {
						family: 'Courier New, monospace',
						size: 18,
						color: '#7f7f7f'
					}
				},
			},
			yaxis: {
				title: {
					text: 'Required (true/false)',
					font: {
						family: 'Courier New, monospace',
						size: 18,
						color: '#7f7f7f'
					}
				}
			}
		};

		Plotly.newPlot('utilizationHist', trace, layout, {showSendToCloud: false});
	}

	function plotOperationCountDistribution(data) {
		count = data.count - data.zeros - data.neg;

		var data = [{
			values: [count, data.zeros, data.neg],
			labels: ['Positive', 'Zeros', 'Negative'],
			type: 'pie'
		}];

		var layout = {
			height: 400,
			width: 500
		};

		Plotly.newPlot('operations', data, layout);
	}

	$("#files").click(function(){
		$.post("loadfile", {file: $("#files").val()},
			function (data, status) {
				$("#code").html("<code>"+data+"</code>");
				(function() {
					var pre = document.getElementsByTagName('pre'),
						pl = pre.length;
					for (var i = 0; i < pl; i++) {
						pre[i].innerHTML = '<span class="line-number"></span>' + pre[i].innerHTML + '<span class="cl"></span>';
						var num = pre[i].innerHTML.split(/\n/).length;
						for (var j = 0; j < num; j++) {
							var line_num = pre[i].getElementsByTagName('span')[0];
							line_num.innerHTML += '<span>' + (j + 1) + '</span>';
						}
					}
				})();
			}
		);
	});

	$('body').on('click', 'b.line', function() {
		if (selection !== null)
			selection.css("background-color",selection.bg);
			
		selection = $(this).children().first();
		selection.bg = selection.css("background-color");
		selection.css("background-color","blue");

		$.post("loadinfo", {idx: $(this).attr("id")},
			function (data, status) {
				plotChangesHistogram(data);
				plotFreqHistogram(data);
				plotUtilizationHistogram(data);
				plotOperationCountDistribution(data);
			}
		);
	});
});
