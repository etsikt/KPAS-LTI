<?php
header('P3P: CP="CAO PSA OUR"');
session_start();
error_reporting(E_ALL & ~E_NOTICE);
ini_set("display_errors", 1);

require_once 'kpasinc/vars.inc';
require_once 'kpasinc/canvas.inc';
require_once 'kpasinc/utility.inc';
require_once 'kpasinc/curlutility.inc';

header('Content-Type: text/html; charset=utf-8');
error_log("KPAS ENROLL USER start");

$user_id=0;
$course_id=0;
$isPrincipal=false;
$isValidSession = false;

if(isset($_SESSION["canvas_user_id"])){
    $isValidSession = true;
    $user_id = $_SESSION["canvas_user_id"];
    $user_login_id = $_SESSION["canvas_user_login_id"];
    $course_id = $_SESSION["canvas_course_id"];
}

?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8" />
    <title>Rollevalg for fagfornyelsen</title>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
    <link rel="stylesheet" href="https://kompetanseplattform.azurewebsites.net/mmooc-min.css">

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
    <script src="js/kpasschool.js"></script>
    <script src="js/mainenroll.js"></script>
 </head>

  <body>

  <?php
    global $principalRoleType;
    if($isValidSession) {
        global $principalRoleType;

        $newRole = $_GET["newRole"];
        $enrollmentResult = "Kunne ikke endre rolle.";

        $enrollments = getCanvasEnrollmentsForCourse($user_login_id, $course_id);

        if($newRole == $principalRoleType) {
        //Do not remove studentEnrollment. A principal should both be student and leader.
//            unenrollFromRole($enrollments, "StudentEnrollment");
            $enrollmentResult = enrollPrincipalInCourse($user_id, $course_id);
        } else {
            unenrollFromRole($enrollments, $principalRoleType);
            $enrollmentResult = enrollStudentInCourse($user_id, $course_id);
        }
        myvardump($enrollmentResult);
    } else
    {
        echo "<h2>Noe gikk galt</h2>";
        echo "<p>Det ser ut til at din nettleser blokkerer inforomasjonskapsler(cookies).</p>";
        echo "<p>Vi bruker informasjonskapsler til å identifisere deg når du endrer rollen din.</p>";
        echo "<a target='_blank' href='https://nettvett.no/slik-administrer-du-informasjonskapsler/'>Slik administrer du informasjonskapsler</a>.</p>";
        echo "<p>Dersom du ikke får til å slå på administrasjonskapsler kan du ta kontakt med din IT-avdeling for å få hjelp.</p>";
        echo "<p>Dersom du ikke ønsker å slå på informasjonskapsler kan du be oss om å endre rollen din ved å kontakte oss på: kompetansesupport@udir.no</h2></p>";
    }
    if($user_login_id && $course_id) {
        $isPrincipal = isPrincipal($user_login_id, $course_id);
    }
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
  <div id="kpasschoolroleinfo"></div>
  </p>
  </body>
</html>
