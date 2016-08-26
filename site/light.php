<html>
	<body>
		<h1><center>Умная дача</center></h1>
		<form method=post>
			<?php
				function switch_off($lamp) {
					ini_set("allow_url_fopen", 1);
					$json = file_get_contents('http://10.8.0.6:8080/stlight?switch_off=$lamp');
					$obj = json_decode($json);
	
					if ($obj->result == 'ok')
						echo("<img width=32 height=32 src='http://www.iconsearch.ru/ajax/download.php?icon_id=34878&size=2&format=png' />");
					else 
						echo("<script>alert('ERROR: while switch OFF light');</script>");
				}
				function switch_on($lamp) {
					ini_set("allow_url_fopen", 1);
					$json = file_get_contents('http://10.8.0.6:8080/stlight?switch_on=$lamp');
					$obj = json_decode($json);
	
					if ($obj->result == 'ok')
						echo("<img width=32 height=32 src='http://www.iconsearch.ru/ajax/download.php?icon_id=16674&size=6&format=png' />");
					else
						echo("<script>alert('ERROR: while switch OFF light');</script>");
				}
?>
			
			<a>Левый прожектор: </a>
			<input type=submit value="Включить" name="left_on" />
			<input type=submit value="Отключить" name="left_off" />
			<?php
				if (isset($_POST['left_off']))
					switch_off(1);

				if (isset($_POST['left_on']))
					switch_on(1);
			?>
			<br>
			<br>
			<a>Центральный прожектор: </a>
			<input type=submit value="Включить" name="center_on" />
			<input type=submit value="Отключить" name="center_off" />
			<?php
				if (isset($_POST['center_off']))
					switch_off(2);

				if (isset($_POST['center_on']))
					switch_on(2);
			?>
			<br>
			<br>
			<a>Основной прожектор: </a>
			<input type=submit value="Включить" name="main_on" />
			<input type=submit value="Отключить" name="main_off" />
			<?php
				if (isset($_POST['main_off']))
					switch_off(3);

				if (isset($_POST['main_on']))
					switch_on(3);
			?>
			<br>
			<br>
			<a>Крыльцо: </a>
			<input type=submit value="Включить" name="exit_on" />
			<input type=submit value="Отключить" name="exit_off" />
			<?php
				if (isset($_POST['exit_off']))
					switch_off(4);

				if (isset($_POST['exit_on']))
					switch_on(4);
			?>
			<br>
			<br>
			<a>Калитка: </a>
			<input type=submit value="Включить" name="kal_on" />
			<input type=submit value="Отключить" name="kal_off" />
			<?php
				if (isset($_POST['kal_off']))
					switch_off(5);

				if (isset($_POST['kal_on']))
					switch_on(5);
			?>
			<br>
			<br>
			<a>Машина: </a>
			<input type=submit value="Включить" name="car_on" />
			<input type=submit value="Отключить" name="car_off" />
			<?php
				if (isset($_POST['car_off']))
					switch_off(6);

				if (isset($_POST['car_on']))
					switch_on(6);
			?>
			<br>
			<br>
			<a>Баня: </a>
			<input type=submit value="Включить" name="bath_on" />
			<input type=submit value="Отключить" name="bath_off" />
			<?php
				if (isset($_POST['bath_off']))
					switch_off(7);

				if (isset($_POST['bath_on']))
					switch_on(7);
			?>
			<br>
			<br>
			<a>Туалет: </a>
			<input type=submit value="Включить" name="toilet_on" />
			<input type=submit value="Отключить" name="toilet_off" />
			<?php
				if (isset($_POST['toilet_off']))
					switch_off(8);

				if (isset($_POST['toilet_on']))
					switch_on(8);
			?>
		</form>
	</body>
</html>
