<?php
	function check_key($key) {
		//TODO: Add db auth
		if ($key == "******")
			return true;
		else
			return false;
	}	
?>