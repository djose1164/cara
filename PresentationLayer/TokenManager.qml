pragma Singleton
import QtQuick
import Qt.labs.settings
import "qrc:/users"

QtObject {
    id: root

    property string accessToken
    property string refreshToken

    signal accessTokenRefreshed

    readonly property Settings settings: Settings {}
    readonly property Refresh refresher: Refresh {
        onAccessTokenReceived: token => {
            root.accessToken = "Bearer " + token;
            accessTokenRefreshed();
        }
        refreshToken: root.refreshToken
    }

    function init() {
        refreshToken = settings.value("Credentials/refreshToken", "")
        refresher.refresh()
    }
}
