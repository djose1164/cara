pragma Singleton
import QtQuick

QtObject {
    id: root

    readonly property string base: "http://127.0.0.1:5000/api"

    readonly property int appWidth: 360
    readonly property int appHeight: 640
}
