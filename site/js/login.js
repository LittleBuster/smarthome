window.onload = function() {
	if (localStorage.apiKey != "undefined")
		document.getElementById('key').value = localStorage.apiKey;
}

function load() {
	var $pass = document.getElementById('key').value;
	localStorage.apiKey = $pass;	

	$.ajax({
		url: "../main/ajax/login.php",
		method: "POST",
		data: {key: $pass},
		dataType: "json",
		success: function(data){
			if(data.result == "ok") {
				document.location.href = "main/index.html";
			} else {
				alert("Неверный ключ!");
			}
		}
    });	
}