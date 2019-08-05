<?php

function urlify($fields)
{
	//url-ify the data for the POST
	$fields_string = "";
	foreach($fields as $key=>$value) 
	{ 
		$fields_string .= $key.'='.$value.'&'; 
	}
	rtrim($fields_string, '&');
	return $fields_string;
}

function mycurldelete($api_endpoint, $access_key)
{
	$url = $api_endpoint;
    $post = false;
    $delete = true;
	$fields = array();
    return mycurlex($url, $fields, $post, $delete, $access_key);
}

function mygetcurl($api_endpoint, $access_key)
{
	$url = $api_endpoint;
	$post = 0;
	$delete = 0;
	$fields = array();
	$result = mycurlex($url, $fields, $post, $delete, $access_key);
	if (!($result === FALSE)) 
	{
        $content = json_decode($result,true);
        return $content;
    }
    return null;
}

function mypostcurl($api_endpoint, $fields, $access_key)
{
	$url = $api_endpoint;
	$post = 1;
    $delete = false;

	$result = mycurlex($url, $fields, $post, $delete, $access_key);
    return json_decode($result,true);
}

function mycurlex($url, $fields, $post, $delete, $access_key)
{
	global $curl_verbose;
    $curl_verbose_file = null;
    
    mydbg("mycurlex url:" .$url);
	
	$fields_string = urlify($fields); 
	$curl = curl_init();

	// CURLOPT_VERBOSE: TRUE to output verbose information. Writes output to STDERR, 
	// or the file specified using CURLOPT_STDERR.
	curl_setopt($curl, CURLOPT_VERBOSE, true);

	if($curl_verbose)
	{
		$curl_verbose_file = fopen('php://temp', 'w+');
		curl_setopt($curl, CURLOPT_STDERR, $curl_verbose_file);	    
	}

	curl_setopt($curl, CURLOPT_URL, $url); 
	if($post)
	{
		curl_setopt($curl, CURLOPT_POST, count($fields));
		curl_setopt($curl, CURLOPT_POSTFIELDS, $fields_string);
	}
	else if($delete)
	{
	    mydbg("mycurlex: DELETE",2);
    	curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	}
	curl_setopt($curl, CURLOPT_HTTPHEADER, array('Authorization: Bearer ' . $access_key));
	curl_setopt($curl, CURLOPT_RETURNTRANSFER,TRUE);
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
	mydbg("Executing curl start</p>");

	$result = curl_exec($curl);
	
	if ($result === FALSE) 
	{
		mydbg(sprintf("cUrl error (#%d): %s<br>\n", curl_errno($curl),
			   htmlspecialchars(curl_error($curl))));
	}
	mydbg("<p>Curl executed</p>");

	if($curl_verbose_file)
	{
		rewind($curl_verbose_file);
		$verboseLog = stream_get_contents($curl_verbose_file);

		error_log($verboseLog);        

		$version = curl_version();
		extract(curl_getinfo($curl));
		$metrics = <<<EOD
URL....: $url
Code...: $http_code ($redirect_count redirect(s) in $redirect_time secs)
Content: $content_type Size: $download_content_length (Own: $size_download) Filetime: $filetime
Time...: $total_time Start @ $starttransfer_time (DNS: $namelookup_time Connect: $connect_time Request: $pretransfer_time)
Speed..: Down: $speed_download (avg.) Up: $speed_upload (avg.)
Curl...: v{$version['version']}
EOD;
	}

	curl_close($curl);
	return $result;
}
?>