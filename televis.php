<?php 
$servername="localhost"; 
$dbname="id17181951_esp_data"; 
$username="id17181951_arunkumar"; 
$password="135/kAN)wCtZ6fKv";  

$conn=new mysqli($servername,$username,$password,$dbname); 
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
} 
$sql="SELECT id,value1,value2,reading_time FROM Sensor order by reading_time desc limit 40"; 
$sensor_data=array();
$result=$conn->query($sql); 
while ($row = $result->fetch_assoc()){
  $sensor_data[] = $row;
}
$readings_time=array_column($sensor_data,'reading_time'); 
$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$value2 = json_encode(array_reverse(array_column($sensor_data, 'value2')), JSON_NUMERIC_CHECK); 
$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK); 
$result->free(); 
$conn->close();
?> 

<!DOCTYPE html>
<html> 
<meta name="viewport" content="width=device-width, initial-scale=1"> 
<script src="https://code.highcharts.com/highcharts.js"></script>
<style>
body {
  /*background-image: url('https://c4.wallpaperflare.com/wallpaper/282/308/59/abstract-vector-red-purple-wallpaper-preview.jpg'); */
  min-width: 310px;
    	max-width: 1280px;
    	height: 500px;
      margin: 0 auto;
} 
h2 {
      font-family: Arial;
      font-size: 2.5rem;
      text-align: center;
    }
</style> 
<body>
<h2>Temperature and Pressure data</h2>
    <div id="chart-temperature" class="container"></div>
    <div id="chart-pressure" class="container"></div>
	<script>
var value1 = <?php echo $value1; ?>;
var value2 = <?php echo $value2; ?>;
var reading_time = <?php echo $reading_time; ?>;

var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature' },
  title: { text: 'BMP280 Temperature' },
  series: [{
    showInLegend: false,
    data: value1
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#059e8a' }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Temperature (Celsius)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});

var chartP = new Highcharts.Chart({
  chart:{ renderTo:'chart-pressure' },
  title: { text: 'BMP280 Pressure' },
  series: [{
    showInLegend: false,
    data: value2
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#18009c' }
  },
  xAxis: {
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Pressure (hPa)' }
  },
  credits: { enabled: false }
});

</script>
</body>
</html>