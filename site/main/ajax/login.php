<?php
	include "auth.php";

	if (check_key($_POST["key"]) == true)
	    echo '{"result": "ok"}';
	else
		echo '{"result": "fail"}';
?>