<?php
	include "auth.php";
	include "creds.php";

	if (check_key($_POST["key"]) == true) {
		if ($_POST["switch"] == "on") {
			ini_set("allow_url_fopen", 1);
        		$json = file_get_contents($smart_server."stlight?switch_on=".$_POST["lamp"]);
       			echo $json;
		}
		if ($_POST["switch"] == "off") {
			ini_set("allow_url_fopen", 1);
            $json = file_get_contents($smart_server."stlight?switch_off=".$_POST["lamp"]);
            echo $json;
		}
		if ($_POST["switch"] == "get_status") {
			ini_set("allow_url_fopen", 1);
            $json = file_get_contents($smart_server."stlight?get_status=");
            echo $json;
		}
	} else {
		echo '{"result": "fail"}';
	}	
?>
