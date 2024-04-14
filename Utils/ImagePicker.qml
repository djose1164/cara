import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtCore

Rectangle {
    id: root

    signal fileAccepted(filename: url)

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

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: fileAccepted(selectedFile)
    }

    function open() {
        fileDialog.open();
    }
}
