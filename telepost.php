<?php

$servername="localhost"; 
$dbname="id17073338_esp_data"; 
$username="id17073338_esp_board"; 
$password="esp32@RMK2019-2023"; 

$api_key="jdhvbgsdjvhsdbcvjyegfdmsdcvh1278ewqndb23dbu"; 

$api=$val1=$val2=""; 

if($_SERVER["REQUEST_METHOD"]=="POST")
{
	$api=clean($_POST["api_key"]); 
	if($api_key==$api)
	{
		$val1=clean($_POST["p"]); 
		$val2=clean($_POST["t"]); 
		$conn=new mysqli($servername,$username,$password,$dbname); 
		$sql="INSERT INTO SensorData (value1,value2)  
				VALUES ('" . $val1 . "','" . $val2 . "')";  
		if($conn->query($sql) === TRUE)
		{
			echo "Success";
		} 
		else
		{
			echo "Failed" . $sql . "<br>" . $conn->error;
		} 
		$conn->close();
	} 
	else
	{
		echo "API key mismatch";
	} 
} 
else
{
	echo "No POST data";
} 

function clean($data)
{
	$data=trim($data); 
	$data=stripslashes($data); 
	$data=htmlspecialchars($data); 
	return $data;
}