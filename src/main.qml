import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: app
    width: 360
    height: 640
    visible: true
    title: "Cara"

    property string source
    property string iconSize: "24x24"

    StackView {
        id: mainStackView
        anchors.fill: parent
    }

    MainSettings { id: settings }
}
