function FirebaseClient(url) {
    var mBaseUrl = url;                 // Url of firebase server

    var mClientMsgPool  = null;         // Firebase requests pool
    var mClientState    = null;         // Firebase app state
    var mClientResponse = null;         // Firebase responses pool

    var mRequests = new Object();       // List of sent requests
    var mOnlineTimerHandle = null;      // Timer for sending OnlineRequest
    var mOnlineWatchdog = null;         // Watchdog that checks whether online or not
    var mOnlineTimeout = 10000;         // How often to send online request
    var mIsOnline = false;              // Current online state

    var mCallbackOnline = null;         // Callback method. Called when online state changes
    var mCallbackSetTemp = null;        // Callback method. Called when response of setTemp comes
    var mCallbackStatusUpdate = null;   // Callback method. Called when status is updated

    this.Init = function()
    {
        mClientMsgPool  = new Firebase(mBaseUrl + '/msg-pool/');
        mClientState    = new Firebase(mBaseUrl + '/state/');
        mClientResponse = new Firebase(mBaseUrl + '/responses/');

        mClientState.on('value', handleStateChanged);
        mClientMsgPool.on('child_added', handleMsgRequest);
        mClientResponse.on('child_added', handleMsgResponse);
        //mClientMsgPool.on('child_removed', handleMsgResponse); // currently unused

        mOnlineTimerHandle = setInterval(function()
        {
            var timeStamp = parseInt(new Date().getTime());
            mClientMsgPool.push({'action': 'RequestOnline', 'timestamp': timeStamp});
        }, mOnlineTimeout);

        mOnlineWatchdog = setInterval(function()
        {
            $.each(mRequests, function(index, value)
            {
                if (value.action == "RequestOnline")
                {
                    if ((mIsOnline == true) && (mCallbackOnline != null))
                    {
                        mCallbackOnline(false);
                    }
                    mIsOnline = false;
                }
            });
        }, mOnlineTimeout * 2);
    }

    // Callbacks setters
    this.SetCallbackOnline = function(callback)
    {
        mCallbackOnline = callback;
    }
    this.SetCallbackSetTemp = function(callback)
    {
        mCallbackSetTemp = callback;
    }
    this.SetCallbackStatusUpdate = function(callback)
    {
        mCallbackStatusUpdate = callback;
    }

    // Performing requests
    this.RequestSetTemperature = function(temperature)
    {
        mClientMsgPool.push({'action': 'RequestSetTemp', 'value': temperature});
    }

    // Firebase events handling
    var handleStateChanged = function(snapshot)
    {
        if (null == snapshot) return;
        if (null != mCallbackStatusUpdate)
        {
            mCallbackStatusUpdate(snapshot.val());
        }

        // A kind of workaround. Needs to be tested
        // If we got state change, then Backend is alive
        // we could also behave as if we got online response
        processOnlineResponse();
    }
    var handleMsgResponse = function(childSnapshot, prevChildName)
    {
        $.each(mRequests, function(index, value)
        {
            if (index == childSnapshot.key())
            {
                processResponse(value, childSnapshot.val());

                // TODO: rework this workaround
                var requestRef = new Firebase(mBaseUrl + '/msg-pool/' + index);
                requestRef.remove();
                var responseRef = new Firebase(mBaseUrl + '/responses/' + index);
                responseRef.remove();
                delete mRequests[childSnapshot.key()];
            }
        });
    }
    var handleMsgRequest = function(childSnapshot, prevChildName)
    {
        mRequests[childSnapshot.key()] = childSnapshot.val();
    }
    var processResponse = function(request, response)
    {
        if (request.action == "RequestOnline")
        {
            processOnlineResponse(response);
        }
        else if (request.action == "RequestSetTemp")
        {
            processSetTempResponse(response);
        }
    }

    // Processing responses
    var processOnlineResponse = function(response)
    {
        if ((mIsOnline == false) && (mCallbackOnline != null))
        {
            mCallbackOnline(true);
        }
        mIsOnline = true;
    }

    var processSetTempResponse = function(response)
    {
        if (mCallbackSetTemp != null)
        {
            mCallbackSetTemp(response);
        }
    }
}
