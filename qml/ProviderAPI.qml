import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

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
