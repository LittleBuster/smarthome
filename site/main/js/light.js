window.onload = function() {
	var imgs_on = ["#img_on0", "#img_on1", "#img_on2", "#img_on3", "#img_on4", "#img_on5", "#img_on6", "#img_on7"];
	var imgs_off = ["#img_off0", "#img_off1", "#img_off2", "#img_off3", "#img_off4", "#img_off5", "#img_off6", "#img_off7"];

	for (var i = 0; i < imgs_on.length; i++)
		$(imgs_on[i]).hide();

	$.ajax({
		url: "ajax/light.php",
		method: "POST",
		data: {lamp: 0, switch: "get_status", key: localStorage.apiKey},
		dataType: "json",
		success: function(data) {
			if(data.result == "ok") {
				for (var i = 0; i < data.lamps.length; i++) {
					if (data.lamps[i] == 1) {
						$(imgs_on[i]).show();
						$(imgs_off[i]).hide();
					} else {
						$(imgs_on[i]).hide();
						$(imgs_off[i]).show();
					}
				}
			} else {
				alert("Ошибка получения текущего статуса прожекторов!");
			}
		}
    });
}


function btn_on_click($lamp, $img_on, $img_off) {
	$.ajax({
		url: "ajax/light.php",
		method: "POST",
		data: {lamp: $lamp, switch: "on", key: localStorage.apiKey},
		dataType: "json",
		success: function(data) {
			if(data.result == "ok") {
				$($img_off).hide();
				$($img_on).show();
			} else {
				alert("Ошибка при включении!");
			}
		}
    });	
}

function btn_off_click($lamp, $img_on, $img_off) {
	$.ajax({
		url: "ajax/light.php",
		method: "POST",
		data: {lamp: $lamp, switch: "off", key: localStorage.apiKey},
		dataType: "json",
		success: function(data) {
			if(data.result == "ok") {
				$($img_on).hide();
				$($img_off).show();
			} else {
				alert("Ошибка при отключении!");
			}
		}
    });	
}
