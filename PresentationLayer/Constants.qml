pragma Singleton
import QtQuick

QtObject {
    id: root

    readonly property string remoteBase: "https://cara-rd.live/api"
    readonly property string localBase: "http://127.0.0.1:5000/api"
    readonly property string base: localBase

    readonly property int appWidth: 360
    readonly property int appHeight: 640
}
