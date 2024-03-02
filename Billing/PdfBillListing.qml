import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Templates as T
import Qt.labs.folderlistmodel
import Utils

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    title: "Facturas en PDF"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
        btnLoader.sourceComponent: MoreVertButton {
            id: menuBtn
            onClicked: menu.open()

            Menu {
                id: menu
                MenuItem {
                    text: "Eliminar todo"
                    onTriggered: {
                        fileUtils.removeMatchingFiles(internal.documentsPath , "factura-*.pdf")
                    }
                }
            }
        }
    }

    FileUtils {
        id: fileUtils
    }

    ListView {
        anchors.fill: parent
        anchors.topMargin: 10
        model: folderModel
        delegate: fileDelegate
        spacing: 10
    }

    FolderListModel {
        id: folderModel
        folder: internal.documentsPath
        nameFilters: ["factura-*.pdf"]
        showDirs: false
    }

    Component {
        id: fileDelegate
        SwipeDelegate {
            id: control
            required property string fileName
            readonly property string path: `${internal.documentsPath}/${fileName}`

            width: root.width
            height: 50
            text: fileName
            contentItem: Item {
                Row {
                    spacing: 10
                    Image {
                        id: img
                        source: "pdf-icon.svg"
                        sourceSize: "32x32"
                    }

                    Label {
                        height: parent.height
                        text: control.text
                        verticalAlignment: Label.AlignVCenter
                        font.pointSize: 12
                    }
                }
            }

            swipe.right: Label {
                id: deleteLabel
                height: parent.height
                anchors.right: parent.right
                verticalAlignment: Label.AlignVCenter
                text: "Eliminar"
                color: "white"
                padding: 12
                background: Rectangle {
                    color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
                }

                SwipeDelegate.onClicked: {
                    print(path);
                    if (!fileUtils.removeFile(path))
                        print("Couldn't remove file.");
                }
            }

            onClicked: shareUtils.share(path)
        }
    }

    ShareUtils {
        id: shareUtils
    }

    QtObject {
        id: internal
        readonly property url documentsPath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    }
}
