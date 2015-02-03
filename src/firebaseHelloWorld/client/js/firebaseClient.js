function FirebaseClient(url) {
    var mBaseUrl = url;

    var mClientMsgPool  = null;
    var mClientState    = null;
    var mClientResponse = null;

    var mRequests = new Object();
    var mOnlineTimerHandle = null;
    var mOnlineWatchdog = null;
    var mOnlineTimeout = 10000;
    var mIsOnline = false;

    var mCallbackOnline = null;

    this.Init = function()
    {
        mClientMsgPool = new Firebase(mBaseUrl + '/msg-pool/');
        mClientState = new Firebase(mBaseUrl + '/state/');
        mClientResponse = new Firebase(mBaseUrl + '/responses/');

        mClientState.on('value', handleStateChanged);
        //mClientMsgPool.on('child_removed', handleMsgResponse);
        mClientMsgPool.on('child_added', handleMsgRequest);
        mClientResponse.on('child_added', handleMsgResponse);

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

    this.SetCallbackOnline = function(callback)
    {
        mCallbackOnline = callback;
    }

    var handleStateChanged = function(snapshot)
    {
        if (null == snapshot) return;
        var status = snapshot.val();
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
    }

    var processOnlineResponse = function(response)
    {
        if ((mIsOnline == false) && (mCallbackOnline != null))
        {
            mCallbackOnline(true);
        }
        mIsOnline = true;
    }
}
