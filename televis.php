<?php

$servername="localhost"; 
$dbname="id17181951_esp_data"; 
$username="id17181951_arunkumar"; 
$password="135/kAN)wCtZ6fKv"; 

$api_key="jdhvbgsdjvhsdbcvjyegfdmsdcvh1278ewqndb23dbu"; 

$api=$val1=$val2=""; 

if($_SERVER["REQUEST_METHOD"]=="GET")
{
	$api=clean($_GET["api_key"]); 
	if($api_key==$api)
	{
		$val2=clean($_GET["p"]); 
		$val1=clean($_GET["t"]); 
		$conn=new mysqli($servername,$username,$password,$dbname); 
		$sql="INSERT INTO Sensor (value1,value2)  
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