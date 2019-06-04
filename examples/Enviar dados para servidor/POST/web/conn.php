<?php
 function Connection(){
  $server="localhost";
  $user="id9383262_alc";
  $pass="monitoralc";
  $db="id9383262_monitor";
     
  $connection = mysql_connect($server, $user, $pass);
           
  if (!$connection) {
      die('MySQL ERROR: ' . mysql_error());
  }
  
  mysql_select_db($db) or die( 'MySQL ERROR: '. mysql_error() );
  return $connection;
 }
?>