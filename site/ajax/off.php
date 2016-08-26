<?php
	ini_set("allow_url_fopen", 1);
	$json = file_get_contents("http://10.8.0.6:8080/stlight?switch_off=".$_POST["lamp"]);
	echo $json;
?>
