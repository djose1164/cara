import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils

CaraApi {
    id: api
    endpoint: "/providers"

    signal providerCreated
    signal providersFetched(resp: var)

    function createProvider(provider) {
        succeeded.connect(function() {
            providerCreated()
        });
        post("/", provider);
    }

    function fetchProviders() {
        succeeded.connect(function(resp) {
            providersFetched(resp.providers)
        });
        get("/");
    }
}
