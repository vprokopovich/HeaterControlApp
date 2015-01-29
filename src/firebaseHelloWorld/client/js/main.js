var requests = new Object();
var msgPool = null;
var state = null;
var targetTemperatureTimer = null;
var halfCircularSlider = null;

$(document).ready(function()
{
    msgPool = new Firebase('https://heater-control.firebaseio.com/msg-pool/');
    state   = new Firebase('https://heater-control.firebaseio.com/state/');

    state.on('value', handleStateChanged);
    msgPool.on('child_removed', handleMsgResponse);
    msgPool.on('child_added', handleMsgRequest);

    $("input#txtTargetTemp").change(onTargetTemperatureChange);

/*
    halfCircularSlider = $('#slider').CircularSlider({ 
        min : 17, 
        max: 40, 
        value : 24,
        radius: 95,
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
*/

});

function handleStateChanged(snapshot)
{
    if (null == snapshot) return;

    var status = snapshot.val();
    updateView(status.status, status.temp, status.targetTemp, status.targetTime, status.timerActivated);
}

function handleMsgResponse(oldChildSnapshot)
{
    delete requests[oldChildSnapshot.key()];
    updateViewProcessingRequests();
}

function handleMsgRequest(childSnapshot, prevChildName)
{
    requests[childSnapshot.key()] = childSnapshot.val();
    updateViewProcessingRequests();
}

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

    halfCircularSlider = $('#slider').CircularSlider({ 
        min : 17, 
        max: 40, 
        value : tempToSet,
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
    //var newTemperature = $("input#txtTargetTemp").val();
    //$("body").append(value);
    //alert(value);
    msgPool.push({'action': 'RequestSetTemp', 'value': value});
    //var a = halfCircularSlider.value;
}

function isEmpty(obj)
{
    return Object.keys(obj).length === 0;
}