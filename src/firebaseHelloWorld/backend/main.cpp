#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FirebaseClient.h"
#include "Request.h"
#include "RequestFactory.h"
#include <CTrace.h>

int main(void)
{
    FirebaseClient client("https://heater-control.firebaseio.com/");

    try
    {
        auto document = client.GetRequestsJsonString();
        auto requests = RequestFactory::CreateRequests(document);

        for (auto requestIter : requests)
        {
            requestIter->Process();
        }
    }
    catch(std::exception& ex)
    {
        TRC_ERROR("%s", ex.what());
    }

    return 0;
}