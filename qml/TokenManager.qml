pragma Singleton
import QtQuick
import QtCore
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
            setAccessToken(token);
            accessTokenRefreshed();
        }
        refreshToken: root.refreshToken
    }

    function init() {
        refreshToken = settings.value("Credentials/refreshToken", "");
        refresher.refresh();
    }

    function setAccessToken(token: string) {
        root.accessToken = "Bearer " + token;
        accessTokenAvailable();
    }

    function clear() {
        accessToken = refreshToken = "";
    }
}
