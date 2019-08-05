<?php
function KPASAPI_GetUserInfo() {
    global $token;
    $user_info_url = "https://auth.dataporten.no/userinfo";
    $result = mygetcurl($user_info_url, $token);
    myvardump($result);
    return $result;
}


function KPASAPI_Response($status,$status_message,$data)
{
	header("HTTP/1.1 ".$status);
    header('Content-Type: application/json');	
	
	$response['status']=$status;
	$response['status_message']=$status_message;
	$response['data']=$data;
	
	$json_response = json_encode($response);
	echo $json_response;
}
?>