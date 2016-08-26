/* Run */
	$("#change1_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 0},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status1_off").hide();
					$("#status1_on").show();
				}
				else
				{
					$("#status1_off").show();
					$("#status1_on").hide();
				}
			}
		});
	});
	
	$("#change1_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 0},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status1_off").show();
					$("#status1_on").hide();
				}
				else
				{
					$("#status1_off").hide();
					$("#status1_on").show();
				}
			}
		});
	});
	
	$("#change2_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 1},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status2_off").hide();
					$("#status2_on").show();
				}
				else
				{
					$("#status2_off").show();
					$("#status2_on").hide();
				}
			}
		});
	});
	
	$("#change2_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 1},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status2_off").show();
					$("#status2_on").hide();
				}
				else
				{
					$("#status2_off").hide();
					$("#status2_on").show();
				}
			}
		});
	});
	
	$("#change3_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 2},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status3_off").hide();
					$("#status3_on").show();
				}
				else
				{
					$("#status3_off").show();
					$("#status3_on").hide();
				}
			}
		});
	});
	
	$("#change3_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 2},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status3_off").show();
					$("#status3_on").hide();
				}
				else
				{
					$("#status3_off").hide();
					$("#status3_on").show();
				}
			}
		});
	});
	
	$("#change4_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 3},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status4_off").hide();
					$("#status4_on").show();
				}
				else
				{
					$("#status4_off").show();
					$("#status4_on").hide();
				}
			}
		});
	});
	
	$("#change4_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 3},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status4_off").show();
					$("#status4_on").hide();
				}
				else
				{
					$("#status4_off").hide();
					$("#status4_on").show();
				}
			}
		});
	});
	
	$("#change5_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 4},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status5_off").hide();
					$("#status5_on").show();
				}
				else
				{
					$("#status5_off").show();
					$("#status5_on").hide();
				}
			}
		});
	});
	
	$("#change5_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 4},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status5_off").show();
					$("#status5_on").hide();
				}
				else
				{
					$("#status5_off").hide();
					$("#status5_on").show();
				}
			}
		});
	});
	
	$("#change6_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 5},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status6_off").hide();
					$("#status6_on").show();
				}
				else
				{
					$("#status6_off").show();
					$("#status6_on").hide();
				}
			}
		});
	});
	
	$("#change6_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 5},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status6_off").show();
					$("#status6_on").hide();
				}
				else
				{
					$("#status6_off").hide();
					$("#status6_on").show();
				}
			}
		});
	});
	
	$("#change7_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 6},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status7_off").hide();
					$("#status7_on").show();
				}
				else
				{
					$("#status7_off").show();
					$("#status7_on").hide();
				}
			}
		});
	});
	
	$("#change7_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 6},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status7_off").show();
					$("#status7_on").hide();
				}
				else
				{
					$("#status7_off").hide();
					$("#status7_on").show();
				}
			}
		});
	});
	
	$("#change8_on").on("click", function() {
		$.ajax({
			url: "../ajax/on.php",
			method: "POST",
			data: {lamp: 7},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status8_off").hide();
					$("#status8_on").show();
				}
				else
				{
					$("#status8_off").show();
					$("#status8_on").hide();
				}
			}
		});
	});
	
	$("#change8_off").on("click", function() {
		$.ajax({
			url: "../ajax/off.php",
			method: "POST",
			data: {lamp: 7},
			dataType: "json",
			success: function(data){
				if(data.result == "ok")
				{
					$("#status8_off").show();
					$("#status8_on").hide();
				}
				else
				{
					$("#status8_off").hide();
					$("#status8_on").show();
				}
			}
		});
	});
	
/* /Run */
