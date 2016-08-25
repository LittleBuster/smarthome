<html>
	<body>
		<h1><center>Умная дача</center></h1>
		<form method=post>
			<a>Левый прожектор: </a>
			<input type=submit value="Включить" name="left_on" />
			<input type=submit value="Отключить" name="left_off" />
			<?php
				if (isset($_POST['left_off'])) {
					$json = file_get_contents('http://10.8.0.6:8080/stlight?switch_off=1');
					$obj = json_decode($json);
	
					if ($obj->left == "0")
						echo("<img width=32 height=32 src='http://www.iconsearch.ru/ajax/download.php?icon_id=34878&size=2&format=png' />");
					else
						echo("ERROR");
				}
				if (isset($_POST['left_on'])) {
					echo("<img width=32 height=32 src='http://www.iconsearch.ru/ajax/download.php?icon_id=16674&size=6&format=png' />");
					$json = file_get_contents('http://10.8.0.6:8080/stlight?switch_on=1');
					$obj = json_decode($json);
	
					if ($obj->left == "1")
						echo("<img width=32 height=32 src='http://www.iconsearch.ru/ajax/download.php?icon_id=16674&size=6&format=png' />");
					else
						echo("ERROR");
				}
			?>
			<br>
			<br>
		</form>
	</body>
</html>
