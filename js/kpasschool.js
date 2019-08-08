this.kpas=this.kpas||{};

this.kpas.school = function() {
  function enrollUser(role) {
      /*Enroll user in course with correct role: https://canvas.instructure.com/doc/api/enrollments.html#method.enrollments_api.create
      Scope: url:POST|/api/v1/courses/:course_id/enrollments
      You can base it on the enroll function in CanvasService.php in the Laravel implementation,
      but we want the user to be enrolled in the course and not in a course section because
      Canvas does not handle a large number of sections very well.
      */
//      alert("Enrolling you as " + role);
  }
  function joinGroup(school) {
      /*Enroll user in group
        You can base it on addUserToGroup in CanvasRepository.php
        Note that the user should not be added to a section.
      */
//    alert("Joining group " + school);
  }
  /**
   * Helper function for creating county
   **/
  function createCounty(name, id) {
    return {
      name: name,
      id: id,
    };
  }

  /**
   * Helper function for creating community
   **/
  function createCommunity(name, id, county) {
    return {
      name: name,
      id: id,
      county: county,
    };
  }

  /**
   * Helper function for creating school
   **/
  function createSchool(name, id, community) {
    return {
      name: name,
      id: id,
      community: community,
    };
  }

  /**
   * Removes all options but the first value in a given select
   * and than selects the only remaining option
   **/
  function removeOptions(select) {
    while (select.options.length > 1) {
      select.remove(1);
    }

    select.value = "";
  }

  /**
   * Adds given options to a given select
   **/
  function addOptions(select, options) {
    console.log(select, options)
    options.forEach(function(option) {
      select.options.add(new Option(option.name, option.id));
    });
  }

  /**
   * Select elements references
   **/
  var countySelect = null;
  var communitySelect = null;
  var schoolSelect = null;

  /**
   * Available counties: Name, County number.
   **/
//  alert("Get counties from https://data-nsr.udir.no/fylker");
  var county = [
    createCounty('Troms', '19'),
    createCounty('Oslo', '3'),
    createCounty('Nordland', '18'),
  ];

  /**
   * Available communities: Name, Community number, County number
   **/
//  alert("Get communities from https://data-nsr.udir.no/kommuner/...");
  var communities = [
    createCommunity('Tromsø', '1902', '19'),
    createCommunity('Balsfjord', '1933', '19'),
    createCommunity('Alstahaug', '1820', '18'),
    createCommunity('Andøy', '1871', '18'),
    createCommunity('Oslo', '0301', '03'),
  ];

  /**
   * Available schools: Name, NSRid, Community number
   **/
//  alert("Get schools from https://data-nsr.udir.no/enheter/kommune/...");
  var schools = [
    createSchool('Solneset skole', '1008716', '1902'),
    createSchool('Gyllenborg skole', '1005091', '1902'),
    createSchool('Malangseidet skole', '1004632', '1933'),
    createSchool('Nordkjosbotn skole', '1004634', '1933'),
    createSchool('Abildsø skole', '1005418', '0301'),
    createSchool('Morellbakken skole', '1011291', '0301'),
  ];

  function displaySchoolSelector() {
    $("#kpasschool").prepend("<div id='kpasschoolStatus'/><div id='kpasschoolSelector'/><div id='kpasschoolContent'/>");
    var infoHtml = 'Welcome';
    var schoolSelectorHtml = 'Select school:<select id="county-select">\
<option value="" disabled selected>--- Fylke ---</option>\
</select> \
\
<select id="community-select">\
<option value="" disabled selected>--- Kommune ---</option>\
</select>\
\
<select id="school-select">\
<option value="" disabled selected>--- Skole ---</option>\
</select> \
<button id="schoolJoin" disabled>Bli med</button>';
    $("#kpasschoolSelector").html(schoolSelectorHtml);

    $("#schoolJoin").click(function() {
      let schoolSelect = document.getElementById('school-select');
      joinGroup(schoolSelect.value);
    });

    /**
    * Adds options to county select
    **/
    countySelect = document.getElementById('county-select');
    $("#county-select").change(function() {
      kpas.school.updateCommunities();
    });
    communitySelect = document.getElementById('community-select');
    $("#community-select").change(function() {
      kpas.school.updateSchools();
    });
    schoolSelect = document.getElementById('school-select');
    $("#school-select").change(function() {
      $("#schoolJoin").prop('disabled', false);
    });
    addOptions(countySelect, county);
  }



  function displaySchoolRoleSelector() {
      var canvasUserId = getCanvasUserId();
      var canvasCourseId = getCanvasCourseId();
      var principal = isPrincipal();

      var html = "Du er registrert som ";
      var buttonText = "Bli deltager";
      var newRole = "Skoleleder";
      if(principal) {
        html += "skoleleder. <p/>";
        html += "Som skoleleder får du se \
         innhold som hjelper deg &aring; lede arbeidet med fagfornyelsen \
          i din organisasjon. Dette innholdet er markert med et eget ikon: <img src='img/usertie.png'/><p/> \
          Dersom du ikke er skoleleder kan du velge å bli deltager i stedet.<p/>";

        newRole = "Student";
      } else {
        html += "deltager. <p/>";
        html += "Dersom du er skoleeier- eller leder skal du  \
         ogs&aring; sette deg inn i innhold som hjelper deg &aring; lede arbeidet med fagfornyelsen \
          i din organisasjon. Dette innholdet vil markeres med et eget ikon: <img src='img/usertie.png'/><p/>";

        buttonText = "Bli skoleleder";
      }

      $("#kpasschoolroleinfo").html(html);

      $("#kpasschoolroleinfo").append(" <a href='enrolluser.php?newRole=" + newRole + "' class='btn btn-primary' id='kpasChangeRoleButton'>" + buttonText + "</a");
    }

    return {
        updateStatus : function(s, waitIcon) {
            $("#kpasschoolStatus").html(s);
            if(waitIcon) {
                $("#kpasschoolStatus").append("<span class='loading-gif'></span>");
            }
        },
        clearStatus : function() {
            $("#kpasschoolStatus").html("");
        },
        updateContent : function(s) {
            $("#kpasschoolContent").html(s);
        },
        appendContent : function(s){
            $("#kpasschoolContent").append(s);
        },
        clearContent : function() {
            $("#kpasschoolContent").html("");
            $("#kpasschoolUserInfo").html("");
        },
        displayEnrollResult : function() {
          var canvasUserId = getCanvasUserId();
          var canvasCourseId = getCanvasCourseId();
          var principal = isPrincipal();

          var html = "Du er nå registrert som ";
          var buttonText = "Bli deltager";
          var newRole = "Skoleleder";
          if(principal) {
            html += "skoleleder. <p/>";
            newRole = "Student";
          } else {
            html += "deltager. <p/>";
            buttonText = "Bli skoleleder";
          }
          html += "Klikk på fanen <i>Kompetansepakkeforside</i> for å se \
          hvordan forsiden har endret seg.<p/>";

          $("#kpasschoolroleinfo").html(html);

          $("#kpasschoolroleinfo").append(" <a href='enrolluser.php?newRole=" + newRole + "' class='btn btn-primary' id='kpasChangeRoleButton'>" + buttonText + "</a");
        },

        display: function() {
//          displaySchoolSelector();
          displaySchoolRoleSelector();
        },


        /**
         * Updates communities
         **/
        updateCommunities : function() {
          var selectedCounty = countySelect.value;
          var options = communities.filter(function(community) {
            return community.county === selectedCounty;
          });

          removeOptions(communitySelect);
          removeOptions(schoolSelect);
          addOptions(communitySelect, options);
        },

        /**
         * Updates schools
         */
        updateSchools : function() {
          var selectedCommunity = communitySelect.value;
          var options = schools.filter(function(school) {
            return school.community === selectedCommunity;
          });

          removeOptions(schoolSelect);
          addOptions(schoolSelect, options);
        }
    }
}();
