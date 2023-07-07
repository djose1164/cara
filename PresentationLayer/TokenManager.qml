pragma Singleton
import QtQuick
import Qt.labs.settings
import "qrc:/users"

QtObject {
    id: root

    property string accessToken
    property string refreshToken

    signal accessTokenRefreshed
    signal accessTokenAvailable

    readonly property Settings settings: Settings {}
    readonly property Refresh refresher: Refresh {
        onAccessTokenReceived: token => {
            root.accessToken = "Bearer " + token;
            accessTokenRefreshed();
            accessTokenAvailable();
        }
        refreshToken: root.refreshToken
    }

    function init() {
        refreshToken = settings.value("Credentials/refreshToken", "")
        refresher.refresh()
    }
}
