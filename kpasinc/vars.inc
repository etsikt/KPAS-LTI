<?php
//DEBUGGING TRUE=ON
$accept_self_signed_certificate=TRUE;
$verbose = TRUE;
$verbose_level = 2;
$curl_verbose = FALSE;

//Dataporten app
$client_id = "";
$client_secret = "";
$redirect_uri = "";

//Canvas connection
$site = "";
$canvas_access_key = "";
$principalRoleType = "Skoleleder";
$account_id = 1;


if(getenv('APPSETTING_DATAPORTEN_AZURE'))
{
    //Dataporten app
    $client_id      = getenv('APPSETTING_DATAPORTEN_CLIENT_ID');
    $client_secret  = getenv('APPSETTING_DATAPORTEN_CLIENT_SECRET');
    $redirect_uri   = getenv('APPSETTING_DATAPORTEN_REDIRECT_URI');

    //Canvas connection
    $site               = getenv('APPSETTING_CANVAS_URI');
    $canvas_access_key         = getenv('APPSETTING_CANVAS_TOKEN');
    $principalRoleType  = getenv('APPSETTING_CANVAS_PRINCIPAL_ROLE_TYPE');
    $account_id         = getenv('APPSETTING_CANVAS_ACCOUNT_ID');

    //Logging
    $verbose_level = getenv('APPSETTING_LOG_LEVEL');
    if($verbose_level)
    {
        $verbose = TRUE;
    }
}
else
{
    //Dataporten app
    $client_id = "e8b368ca-de4e-4de3-80de-604194cc7b33";
    $client_secret = "338012f4-37e0-4547-9b16-3d6160ba6504";
    $redirect_uri = "http://localhost:8880/kpas/index.php";

    //Canvas connection
    $site = "https://bibsys.instructure.com";
    $canvas_access_key = "3727~YNUJGTLTd63CjSBAOrAPLaFbP7QOtugI4IIdMc7COvdhq5zxe9UIjr2CQiphzAOZ";

    $principalRoleType = "Skoleleder";
    $account_id =1;
}
?>
