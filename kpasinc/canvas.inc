<?php
/*
File   :utility.inc
Author :Erlend Thune
Email  :eth@udir.no
Date   :13032019
Purpose:Utility functions for communication with the instructure Canvas platform.
*/

function getUserId($arr)
{
	$user_id = 0;
	foreach($arr as $key=>$value)
	{
		if($key == "id")
		{
			$user_id = $value;
		}
		//var_dump($key);
		//var_dump($value);
	}
	return $user_id;
}



function printCanvasUser($canvasUser)
{
    echo("<h2>Canvas user</h2>");
    var_dump($canvasUser);
}

function printCanvasUserId($user_id)
{
    mydbg("<br/>Canvas user id: " . $user_id, 2);
}


function getCanvasCourse($course_id)
{
    global $site;
    global $canvas_access_key;
    $api_endpoint = $site . "/api/v1/courses/" . $course_id;
    $result = mygetcurl($api_endpoint, $canvas_access_key);
    return $result;
}

function getSections($courseId)
{
    global $site;
    global $canvas_access_key;
    $uri = $site ."/api/v1/courses/" . $courseId . "/sections";
    mydbg($uri, 2);
    $result = mygetcurl($uri, $canvas_access_key);
    myvardump($result, 2);
    return $result;
}


function getGroups($group_category_id)
{
    global $site;
    global $canvas_access_key;

    $api_endpoint = $site . "/api/v1/group_categories/" . $group_category_id . "/groups?per_page=999";
    $result = mygetcurl($api_endpoint, $canvas_access_key);
    return $result;
}
function AddUserToGroup($userId, $group, $unenrollmentIds)
{
    $groupName = $group["name"];
    $groupDescription = $group["description"];
    $membership = $group["membership"];
    $groupCategoryId = $group["group_category_id"];
    $courseId = $group["course_id"];

    $groupId = createGroup($groupCategoryId, $groupName, $groupDescription);
    $sectionName = $groupName . ":" . $groupDescription;
    $sectionId = createSection($courseId, $sectionName);
    mydbg("Section:" . $sectionId, 2);

    mydbg("Membership:".$membership, 2);

    mydbg("Fjerner tidligere medlemskap.",2);
    unenrollUserFrom($userId, $courseId, $unenrollmentIds);

    mydbg("Legger deg til som student.",2);
    $enrollment = enrollStudentInSection($userId, $courseId, $sectionId);

    mydbg("<br/>Legger bruker med id " . $userId . " til gruppekategori med id " . $groupCategoryId . " i gruppen med id " . $groupId,2);
    if(AddUserToGroupId($userId, $groupId))
    {
        mydbg("<br/>Brukeren ble lagt til.",2);
        return "Success";
    }
    return "Failure";
}

function getRoleIdFor($roleType)
{
    global $account_id;
    global $site;
    global $canvas_access_key;

    $roleId = 0;
    $uri = $site . "/api/v1/accounts/" . $account_id. "/roles";
    $roles = mygetcurl($uri, $canvas_access_key);
    myvardump($roles, 4);
    foreach ($roles as $role)
    {
        if($role["role"] == $roleType)
        {
            myvardump($role,4);
            $roleId = $role["id"];
            break;
        }
    }
    return $roleId;
}

function unenrollFromRole($enrollments, $roleType) {
    global $site;
    global $canvas_access_key;
    foreach ($enrollments as $enrollment)
    {
        if($enrollment["role"] == $roleType) {
            mydbg("<br/>Fjerner deg fra tidligere seksjon med enrollment id " . $unenrollmentId,2);
            $uri = $site . "/api/v1/courses/" . $enrollment["course_id"] . "/enrollments/"  . $enrollment["id"] . "?task=delete";
            mycurldelete($uri, $canvas_access_key);
        }
    }
}

function unenrollUserFrom($user_id, $course_id, $unenrollmentIds)
{
    if(!$unenrollmentIds) return;

    global $site;
    global $canvas_access_key;
    foreach ($unenrollmentIds as $unenrollmentId)
    {
        mydbg("<br/>Fjerner deg fra tidligere seksjon med enrollment id " . $unenrollmentId,2);
        $uri = $site . "/api/v1/courses/" . $course_id . "/enrollments/"  . $unenrollmentId . "?task=delete";
        mycurldelete($uri, $canvas_access_key);
    }
}

function enrollPrincipalInSection($user_id, $course_id, $section_id)
{
    global $principalRoleType;
    $enrollment = null;
    $role_id = getRoleIdFor($principalRoleType);
    if($role_id)
    {
        mydbg("Legger deg til i seksjon " . $section_id . " som skoleleder.",2);
        $enrollment = enrollInSection($user_id, $role_id, $course_id, $section_id);
    }
    else
    {
        mydbg("Kunne ikke finne rolle-id for skoleleder.",2);
    }
    return $enrollment;
}
function enrollPrincipalInCourse($user_id, $course_id)
{
    global $principalRoleType;
    $enrollment = null;
    $role_id = getRoleIdFor($principalRoleType);
    if($role_id)
    {
        mydbg("Legger deg til i kurs " . $course_id . " som skoleleder.",2);
        $enrollment = enrollInCourse($user_id, $role_id, $course_id);
    }
    else
    {
        mydbg("Kunne ikke finne rolle-id for skoleleder.",2);
    }
    return $enrollment;
}

function enrollInSection($user_id, $role_id, $course_id, $section_id)
{
    global $site;
    global $canvas_access_key;
    $uri = $site . "/api/v1/sections/" . $section_id . "/enrollments";
    $fields = array(
        'enrollment[user_id]' => $user_id,
        'enrollment[role_id]' => $role_id,
        'enrollment[enrollment_state]' => "active",
        'enrollment[limit_privileges_to_course_section]' => "true",
        'enrollment[self_enrolled]' => 'true'
    );
    $enrollment = mypostcurl($uri, $fields, $canvas_access_key);
	return $enrollment;
}
function enrollInCourse($user_id, $role_id, $course_id)
{
    global $site;
    global $canvas_access_key;
    $uri = $site . "/api/v1/courses/" . $course_id . "/enrollments";
    $fields = array(
        'enrollment[user_id]' => $user_id,
        'enrollment[role_id]' => $role_id,
        'enrollment[enrollment_state]' => "active",
        'enrollment[self_enrolled]' => 'true'
    );
    $enrollment = mypostcurl($uri, $fields, $canvas_access_key);
	return $enrollment;
}
function enrollStudentInCourse($user_id, $course_id)
{
    global $site;
    global $canvas_access_key;
    $uri = $site . "/api/v1/courses/" . $course_id . "/enrollments";
    $fields = array(
        'enrollment[user_id]' => $user_id,
        'enrollment[enrollment_state]' => "active",
        'enrollment[self_enrolled]' => 'true'
    );
    $enrollment = mypostcurl($uri, $fields, $canvas_access_key);
	return $enrollment;
}


function enrollStudentInSection($user_id, $course_id, $section_id)
{
    $roleType = "StudentEnrollment";
    $enrollment = null;
    $role_id = getRoleIdFor($roleType);
    if($role_id)
    {
        mydbg("Legger deg til i seksjon " . $section_id . " som student.",2);
	    $enrollment = enrollInSection($user_id, $role_id, $course_id, $section_id);
	}
	else
	{
	    mydbg("Kunne ikke finne rolle-id for student.",2);
	}
	return $enrollment;
}

function createSection($courseId, $sectionName)
{
    mydbg("Undersøker om seksjonen med navnet " . $sectionName . " eksisterer.",2);
    $sectionId = sectionExist($courseId, $sectionName);
    if($sectionId)
    {
        mydbg("Seksjonen eksisterte og har id " . $sectionId,2);
    }
    else
    {
        mydbg("Seksjonen eksisterte ikke. Oppretter den.",2);
        $createdSection = createSectionEx($courseId, $sectionName);
        myvardump($createdSection,3);
        $sectionId = $createdSection["id"];
    }
    return $sectionId;
}

function createSectionEx($courseId, $sectionName)
{
    global $site;
    global $canvas_access_key;

    $uri = $site . "/api/v1/courses/" . $courseId . "/sections";
    $fields = array('course_section[name]' => $sectionName);
    $section = mypostcurl($uri, $fields, $canvas_access_key);
    return $section;
}

function createGroupEx($groupCategoryId, $groupName, $groupDescription)
{
  global $site;
  global $canvas_access_key;

  $uri = $site . "/api/v1/group_categories/" . $groupCategoryId . "/groups";
  $fields = array(
    'name' => $groupName,
    'description' => $groupDescription
  );

  $newGroup = mypostcurl($uri, $fields, $canvas_access_key);
  return $newGroup;
}
function createGroup($groupCategoryId, $groupName, $groupDescription)
{
    mydbg("Undersøker om gruppen med navnet " . $groupName . " og beskrivelsen " . $groupDescription . " eksisterer.",2);
    $groupId = groupExist($groupName, $groupDescription, $groupCategoryId);
    if($groupId)
    {
        mydbg("Gruppen eksisterte og har id " . $groupId,2);
    }
    else
    {
        mydbg("Gruppen eksisterte ikke. Oppretter den.",2);
        $createdGroup = createGroupEx($groupCategoryId, $groupName, $groupDescription);
        myvardump($createdGroup);
        $groupId = $createdGroup["id"];
    }
    return $groupId;
}


function sectionExist($courseId, $sectionName)
{
    $sectionId = 0;
    $sections = getSections($courseId);
    foreach ($sections as $section)
    {
        mydbg($section["name"] . "=?" . $sectionName, 2);
        if(($section["name"] == $sectionName))
        {
            $sectionId = $section["id"];
            break;
        }
    }
    return $sectionId;
}

function groupExist($groupName, $groupDescription, $groupCategoryId)
{
    $groupId = 0;
    $groups = getGroups($groupCategoryId);
    foreach ($groups as $group)
    {
        if(($group["name"] == $groupName) && ($group["description"] == $groupDescription))
        {
            $groupId = $group["id"];
            break;
        }
    }
    return $groupId;
}

function getGroupCategories($course_id)
{
	global $site;
    global $canvas_access_key;
    $api_endpoint = $site . "/api/v1/courses/" . $course_id . "/group_categories?per_page=999";
    $result = mygetcurl($api_endpoint, $canvas_access_key);
    return $result;
}

function getCanvasEnrollmentsForCourse($user_login_id, $course_id)
{
    global $site;
    global $canvas_access_key;
    $enrollments = array();

    //api/v1/courses/286/users?search_term=erlend.thune@udir.no&enrollment_role_id=79&include[]=enrollments
    $api_endpoint = $site . "/api/v1/courses/" .$course_id . "/users?search_term=" . $user_login_id . "&include[]=enrollments";
    $result = mygetcurl($api_endpoint, $canvas_access_key);

    $courseEnrollments = null;
	foreach($result as $r)
	{
	    if(($r["login_id"] == $user_login_id))
	    {
    	    myvardump($r);
	        mydbg("Pushing enrollments");
	        $courseEnrollments = $r["enrollments"];
	        break;
	    } else {
	        mydbg("Skip login id:" . $r["login_id"] . " different from " . $user_login_id);
	    }
	}
	mydbg("Courseenrollments:");
	myvardump($courseEnrollments);
	return $courseEnrollments;
}
function isPrincipal($user_login_id, $course_id)
{
    global $principalRoleType;
    $isPrincipal = false;
    $enrollments = getCanvasEnrollmentsForCourse($user_login_id, $course_id);
	foreach($enrollments as $enrollment)
	{
	    mydbg("Enrollment id: ". $enrollment["id"]);
	    mydbg("Enrollment course id: ". $enrollment["course_id"]);
	    mydbg("Enrollment role: ". $enrollment["role"]);
	    if(($enrollment["course_id"] == $course_id) && ($enrollment["role"] == $principalRoleType))
	    {
        	mydbg("Du er skoleleder.");
	        return true;
	    }
	}
	mydbg("Du er ikke skoleleder.");
	return false;
}


function printCanvasEnrollments($canvasEnrollments)
{
    mydbg("<h2>Canvas enrollments</h2>");
    myvardump($canvasEnrollments);
}


function getCanvasUserFromFeideIdOrEmail($feideid, $email)
{
    global $site;
    global $canvas_access_key;
    global $account_id;
    $searchFor = $feideid;
    if($searchFor == "")
    {
        mydbg("Dataporten user has no feide id, use email address instead:" .$email);
        $searchFor = $email;
    }

    $api_endpoint = $site . "/api/v1/accounts/" . $account_id . "/users?search_term=" . $searchFor ;
    mydbg("getCanvasUserFromFeideId: " . $api_endpoint,2);
    $result = mygetcurl($api_endpoint, $canvas_access_key);
    myvardump($result,2);

    if(array_key_exists('errors', $result)) {
        myvardump($result);
        $errorMessage = "Canvas: " . $result["errors"][0]["message"];
        throw new Exception($errorMessage);
    }

    mydbg("Matching " . $searchFor . " with Canvas users found.");
	foreach($result as $canvasUser)
	{
        myvardump($canvasUser);
		if($canvasUser["login_id"] == $searchFor)
		{
            mydbg("FOUND canvasUser");
			return $canvasUser;
		}
	}

    return null;
}


function addUserToGroupId($user_id,$gid)
{
    global $site;
    global $canvas_access_key;
	$uri = $site . "/api/v1/groups/" . $gid . "/memberships";
	$fields = array(
		'user_id' => $user_id
	);
	return mypostcurl($uri, $fields, $canvas_access_key);
}
?>
