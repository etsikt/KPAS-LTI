<?php
function mydbg($s, $level=1)
{
    global $verbose;
	global $verbose_level;

	if($verbose && ($verbose_level >= $level))
	{
		error_log($s);
	}
}
function myerror($s)
{
	$stuff = <<<EOT
  <div class="card">
    <div class="card-body bg-danger text-white">$s</div>
  </div>
EOT;
    echo $stuff;
}

function myjson($s)
{
    $json = json_encode($s, JSON_PRETTY_PRINT); 
    error_log($json);
}

function myvardump($v, $level=1)
{
	global $verbose;
	global $verbose_level;

	if($verbose && ($verbose_level >= $level))
	{
        ob_start();
        var_dump($v);
        $debug_dump = ob_get_clean();
        error_log("Variable dump, level=" . $level . " verbose_level=". $verbose_level);
        error_log($debug_dump);
	}
}

function printGroupMemberships($course_id, $groupCategories, $dataportenGroups)
{
    echo "<table class='table'><tr><th>Gruppekategori i Canvas</th><th>Gruppemuligheter i dataporten</th></tr>";
    foreach ($groupCategories as $groupCategory)
    {
        echo "<tr><td>" . $groupCategory["name"];
        echo "</td>";
        $dp = "<ul>";
        foreach ($dataportenGroups as $dataportenGroup)
        {
            $membership = $dataportenGroup["membership"]["basic"];
            $dp .= "<li>" . $dataportenGroup["displayName"] . "(" . $membership . ")</li>";
        }
        echo "</lu><td>" . $dp . "</td></tr>";
    }
    echo "</table>";
}
function printCourseId($courseId)
{
    echo "<h2>Canvas course id</h2>" . $courseId . "<br/>";
}

function printGroupsForCourse($course_id, $dataportenGroups)
{
    mydbg("Henter Canvaskurs " . $course_id);
    $course = getCanvasCourse($course_id);
    mydbg("Course name" . $course["name"]);
    $groupCategories = getGroupCategories($course_id);
    echo "<h2>Canvas <-> Dataporten</h2>";
    printGroupMemberships($course_id, $groupCategories, $dataportenGroups);
}

function printCanvasEnrollmentGroups($user_id, $canvasEnrollments, $dataportenGroups)
{
    global $verbose;
    if($verbose)
    {
        echo "<h2>Canvas enrollment groups</h2>";
        if(!$canvasEnrollments)
        {
            echo "No enrollments.";
            return;
        }
        myvardump($canvasEnrollments);
    }
    
    foreach ($canvasEnrollments as $course)
    {    
        printGroupsForCourse($user_id, $course_id, $dataportenGroups);
    }
}

function printCommonGroups($dataportenGroups, $canvasEnrollmentGroups)
{
    echo "<h1>Common dataporten and Canvas groups</h1>";
    foreach ($canvasEnrollmentGroups as $canvasEnrollmentGroup)
    {
        //For each enrollment
        foreach ($canvasEnrollmentGroup as $canvasGroup)
        {
            foreach ($dataportenGroups as $dataportenGroup)
            {
                if($dataportenGroup["displayName"] == $canvasGroup["name"])
                {
                    $groupName = $dataportenGroup["displayName"];
                    echo "<p>$groupName</p>";
                }
            }
        }
    }
}
function printDataportenHeading()
{
    global $verbose;
    if(!$verbose) return;
    echo "<h1>Dataporten</h1>";
}

function printCanvasHeading()
{
    global $verbose;
    if(!$verbose) return;
    echo "<h1>Canvas</h1>";
}

function printHtmlHeader()
{
	$stuff = <<<EOT
<!DOCTYPE html>
<html lang="en">
<head>
  <title>KPAS (Kompetanseplattform Administrativt System</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js"></script>
</head>
<body>

<div class="container">
EOT;
	echo $stuff;
}

function printLoginButton() 
{
	$stuff = <<<EOT
<div id="button">
	<a id="login" href="index.php?login">Login</a>
</div>
EOT;
    print $stuff;
}

function printLogoutButton()
{
    $stuff = <<<EOT
    <a id="logout" href="index.php?logout">Logout</a>
EOT;
    print $stuff;
}

function printHtmlFooter()
{
	$stuff = <<<EOT
    </div>
    </body>
    </html>
EOT;
	echo $stuff;
}
?>