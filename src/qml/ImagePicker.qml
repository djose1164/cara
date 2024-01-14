import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtCore

Rectangle {
    id: root

    property alias imagePath: img.source

    color: "gray"

    states: [
        State {
            name: "image-exists"
            when: img.status === Image.Ready
            PropertyChanges {
                plusBtn { visible: false }
            }
            PropertyChanges {
                deleteBtn { visible: true }
            }
        }
    ]

    RoundButton {
        id: plusBtn
        anchors.centerIn: parent
        visible: img.source
        flat: true
        text: "+"
        font.pointSize: 14
        onClicked: fileDialog.open()
    }

    Image {
        id: img
        anchors.fill: parent
    }

    RoundButton {
        id: deleteBtn
        anchors {
            left: img.right
            leftMargin: 20
            top: img.top
        }
        visible: false
        flat: true
        icon.source: "qrc:/icons/trash-can-outline.svg"
        onClicked: img.source = ""
    }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: img.source = selectedFile
    }
}
