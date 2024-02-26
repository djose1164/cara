import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Templates as T
import Qt.labs.folderlistmodel
import Utils
import "qrc:/utils"

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
        Item {
            required property string fileName
            width: root.width
            height: 50

            Frame {
                width: root.width * .9
                anchors.centerIn: parent

                Label {
                    text: fileName
                }

                TapHandler {
                    onTapped: print(`${internal.documentsPath}/${fileName}`)
                }
            }
        }
    }

    QtObject {
        id: internal
        readonly property url documentsPath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    }
}
