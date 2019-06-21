<?php
error_reporting(E_ALL & ~E_NOTICE);
ini_set("display_errors", 1);

require_once 'ims-blti/blti.php';
$lti = new BLTI("7ea25a9aa639a4a9ba254e2d492bc566", false, false);

session_start();
header('Content-Type: text/html; charset=utf-8');
error_log("KPAS LTI start");
?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8" />
    <title>Building Tools With The Learning Tools Operability Specification</title>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
    <script src="js/kpasschool.js"></script>
    <script src="js/main.js"></script>
</script>  </head>

  <body>
    <h2>Role- and school selector KPAS LTI</h2>
    <div id="kpasschoolroleinfo"></div>
    Du er medlem av følgende grupper:<br/>
    Fylke: ingen<br/>
    Kommune: ingen<br/>
    Skole: ingen<br/>
    
    <div id="kpasschool"></div>
  <?php
   if ($lti->valid) {
    error_log("LTI is valid");
 ?>
    <h2>LTI informasjon</h2>
 <?php
   echo "Din Canvas brukerid: " . $_POST["custom_canvas_user_id"];
   echo "<h3>POST parametere</h3><pre>";
   foreach($_POST as $key => $value) {
       print "$key=$value\n";
   }
   echo "</pre><h3>REQUEST parametere</h3><pre>";
   foreach($_REQUEST as $key => $value) {
       print "$key=$value\n";
   }
   echo "<pre><h3>SERVER parametere</h3>";
   foreach($_SERVER as $key => $value) {
       print "$key=$value\n";
   }
 ?>
     </pre>
 <?php
    } else {
    error_log("LTI is not valid");
    var_dump($lti);
  ?>
    <h2>This was not a valid LTI launch</h2>
    <p>Error message: <?= $lti->message ?></p>
  <?php
    }
  ?>
  </body>

</html>
