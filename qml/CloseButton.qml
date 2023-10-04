import QtQuick.Controls

RoundButton {
    onClicked: root.close()

    flat: true

    IconImage {
        anchors.centerIn: parent
        source: "qrc:/crossIcon"
        sourceSize: "20x20"
    }
}
