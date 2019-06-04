<?php

	include("conn.php");
    
	$link=Connection();

	$temperatura = $_POST["temp"];
	$umidade = $_POST["umid"];
	$luminosidade = $_POST["lum"];

	echo "[DEBUG] Dado do sensor de Temperatura: " . $temperatura . "\n";
	echo "[DEBUG] Dado do sensor de Umidade: " . $umidade . "\n";
	echo "[DEBUG] Dado do sensor de Luminosidade: " . $luminosidade . "\n";

	$query = "INSERT INTO `id9383262_monitor`.`ESTACAO_001`(`TEMPERATURA`,`UMIDADE`,`LUMINOSIDADE`) VALUES ($temperatura,$umidade,$luminosidade)";
   
	mysql_query($query,$link);
	mysql_close($link);

	echo "[DEBUG] Novos dados dos sensores adicionados com sucesso\n";

?>