<?php
	$con=mysqli_connect("192.168.0.201","root","root","noise");
	if(mysqli_connect_errno($con))
	{
		echo "Failed to connect to MySQL:".mysqli_connect_error();
	}
	
	mysqli_set_charset($con,"utf8");
	
	$res = mysqli_query($con,"select*from data where id=(select max(id) from data)");
	$result = array();
	
	while($row = mysqli_fetch_array($res)){
		
		array_push($result, array('time'=>$row[0], 'noise'=>$row[1], 'no'=>$row[2]));
	
	}	
	echo json_encode(array("result"=>$result));
	
	mysqli_close($con);
?>


