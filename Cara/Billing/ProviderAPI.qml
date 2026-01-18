import QtQuick
import Cara.Utils

Item {
    default property var data

    CaraApi {
        id: api
        endpoint: "/providers"

        signal providerCreated
        signal providersFetched(resp: var)
    }

    function createProvider(provider) {
        api.succeeded.connect(function () {
            api.providerCreated();
        });
        api.post("/", provider);
    }

    function fetchProviders() {
        api.succeeded.connect(function (resp) {
            api.providersFetched(resp.providers);
        });
        api.get("/");
    }
}
