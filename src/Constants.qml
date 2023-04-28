pragma Singleton
import QtQuick

QtObject {
    id: root

    readonly property string base: "https://cara-rd.live/api"

    readonly property int appWidth: 360
    readonly property int appHeight: 640
}
