<?php
error_reporting(E_ALL & ~E_NOTICE);
ini_set("display_errors", 1);

require_once 'ims-blti/blti.php';
require_once 'kpasinc/vars.inc';
require_once 'kpasinc/canvas.inc';
require_once 'kpasinc/utility.inc';
require_once 'kpasinc/curlutility.inc';


//Canvas configuration
$lti = new BLTI("fc83dea2b575f839f450a7e4bb972c6a", false, false);
//$lti = new BLTI("fc83dea2b575f839f450a7e4bb972c6a", false, false);

session_start();
header('Content-Type: text/html; charset=utf-8');
error_log("KPAS LTI start");

if ($lti->valid) {
    error_log("LTI is valid");
    $user_id = $_POST["custom_canvas_user_id"];
    $course_id = $_POST["custom_canvas_course_id"];
    $user_login_id= $_POST["custom_canvas_user_login_id"];

    mydbg("Checking if principal");
    $isPrincipal = isPrincipal($user_login_id, $course_id);
    $_SESSION["canvas_user_id"] = $user_id;
    $_SESSION["canvas_course_id"] = $course_id;
    $_SESSION["canvas_user_login_id"] = $user_login_id;
}
?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8" />
    <title>Rollevalg for fagfornyelsen</title>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
    <script src="js/kpasschool.js"></script>
    <script src="js/main.js"></script>
<?php
    echo <<<EOT
    <script type='text/javascript'>
        function getCanvasUserId() {
            return $user_id;
        }
        function getCanvasUserLoginId() {
            return "$user_login_id";
        }
        function getCanvasCourseId() {
            return $course_id;
        }
        function isPrincipal() {
            return $isPrincipal;
        }
    </script>
EOT;
?>
 </head>

  <body>
    <div id="kpasschoolroleinfo"></div>
<!--    Du er medlem av følgende grupper:<br/>
    Fylke: ingen<br/>
    Kommune: ingen<br/>
    Skole: ingen<br/>
-->
    <div id="kpasschool"></div>
  <?php
   if ($lti->valid) {
/*    echo "Din Canvas brukerid: " . $user_id;
    echo "Canvas course id: " . $course_id;
    echo "<h2>LTI informasjon</h2>";

   echo "<h3>POST parametere</h3><pre>";
   foreach($_POST as $key => $value) {
       print "$key=$value\n";
   }
    print "<h2>Rolle</h2>";

    if($isPrincipal) {
      echo "Du er skoleleder";
    } else {
      echo "Du er lærer";
    }

   echo "</pre><h3>REQUEST parametere</h3><pre>";
   foreach($_REQUEST as $key => $value) {
       print "$key=$value\n";
   }
    echo "</pre>";
*/
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
