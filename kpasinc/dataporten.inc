<?php

function getExtraUserInfoFromDataporten($access_token, $oauth2)
{
    $user_info_url = "https://api.dataporten.no/userinfo/v1/userinfo";
    $user_info = $oauth2->get_identity($access_token, $user_info_url);
    mydbg("getExtraUserInfoFromDataporten");
    myvardump($user_info);
    return $user_info;
}
function printDataportenExtraUserInfo($extraUserInfo)
{
    echo("<h2>Dataporten extra user info</h2>");
    var_dump($extraUserInfo);
}
function printDataportenAccessToken()
{
    echo("<h2>Dataporten access token</h2>");
    echo $_SESSION["token"];
}

function getUserInfoFromDataporten($access_token, $oauth2)
{
    $user_url = "https://auth.dataporten.no/userinfo";
    $user = $oauth2->get_identity($access_token, $user_url);
    mydbg("getUserInfoFromDataporten");
    myvardump($user);
    return $user;
}

function printDataportenUserInfo($userInfo)
{
    echo("<h2>Dataporten user info</h2>");

    var_dump($userInfo);
}

function getFeideIdFromDataportenUserInfo($userInfo)
{
    $userid_sec = $userInfo["user"]["userid_sec"][0];
    $start = strpos($userid_sec, ":") + 1;
    $feideid = substr($userid_sec, $start);
    return $feideid;
}
function getEmailFromDataportenUserInfo($userInfo)
{
    $email = $userInfo["user"]["email"];
    return $email;
}

function printDataportenGroupsInfo($groupsInfo)
{
    echo("<h2>Dataporten groups info</h2>");

    var_dump($groupsInfo);

}
function getGroupsInfoFromDataporten($access_token, $oauth2)
{
    $groups_url = "https://groups-api.dataporten.no/groups/me/groups";
    $groups = $oauth2->get_identity($access_token, $groups_url);
    mydbg("getGroupsInfoFromDataporten");
    myvardump($groups);
    return $groups;
}


?>