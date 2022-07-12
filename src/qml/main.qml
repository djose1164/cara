import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 340
    height: 680
    visible: true
    title: "RapiK"

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: "qrc:/qml/Login.qml"
    }
}
