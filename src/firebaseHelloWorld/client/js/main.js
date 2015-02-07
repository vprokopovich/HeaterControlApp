var targetTemperatureTimer = null;
var halfCircularSlider = null;
var firebaseClient = new FirebaseClient("https://heater-control.firebaseio.com/");

$(document).ready(function()
{
    firebaseClient.Init();
    firebaseClient.SetCallbackOnline(onOnlineChange);
    firebaseClient.SetCallbackStatusUpdate(function(status)
    {
        updateView(status.status,
                   status.temp,
                   status.targetTemp,
                   status.targetTime, 
                   "status.timerActivated");
    });

    halfCircularSlider = $('#slider').CircularSlider({ 
        min : 16, 
        max: 29, 
        radius: 93,
        labelSuffix: "&deg;",
        shape: "Half Circle",
        slide: function(el, value)
        {
            if (null == targetTemperatureTimer)
            {
                targetTemperatureTimer = setTimeout(function(){}, 10);
                return;
            }
            clearTimeout(targetTemperatureTimer);
            targetTemperatureTimer = setTimeout(function(){onTargetTemperatureChange(value);}, 500);
        }
    });
});

function updateView(status, temperature, targetTemperature, targetTime, timerActivated)
{
    $("div#lblStatus").html(status);
    $("div#lblTemp").html(temperature + "&deg;");
    $("input#txtTargetTemp").val(targetTemperature);
    $("input#txtTargetTime").val(targetTime);

    var minTemp = 17;
    var maxTemp = 40;
    var tempToSet = Math.round(targetTemperature);
    if (tempToSet < minTemp) tempToSet = minTemp;
    if (tempToSet > maxTemp) tempToSet = maxTemp;

    if (null != halfCircularSlider)
    {
        halfCircularSlider.setValue(tempToSet);
    }
}

function updateViewProcessingRequests()
{
    if (false == isEmpty(requests))
    {
        $("div#lblRequestStatus").html("Processing request");
        $("div#lblRequests").html(JSON.stringify(requests));
    }
    else
    {
        $("div#lblRequestStatus").html("All requests processed");
        $("div#lblRequests").html("");
    }    
}

function onTargetTemperatureChange(value)
{
    firebaseClient.RequestSetTemperature(value);
}
function onOnlineChange(isOnline)
{
    if (isOnline == true)
    {
        $("div#pageConnecting").css("display", "none");
        $("div#pageMain").css("display", "block");
    }
    else
    {
        $("div#pageConnecting").css("display", "block");
        $("div#pageMain").css("display", "none");
    }
}

function isEmpty(obj)
{
    return Object.keys(obj).length === 0;
}