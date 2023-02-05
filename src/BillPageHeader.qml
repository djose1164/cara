import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/search" as Search
import "qrc:/client"

ToolBar {
    id: root

    property alias clientModel: filter.clientModel
    property alias filter: filter

    RowLayout {
        anchors.fill: parent

        Label {
            text: "Todas las facturas"
            font.bold: true
            font.pointSize: 16
            Layout.alignment: Qt.AlignCenter
        }
        /*Search.SearchBar {
            placeholderText: "Ingrese nombre de cliente o ID"
            //Layout.leftMargin: 20
            Layout.rightMargin: 40
            Layout.alignment: Qt.AlignLeading
            onAccepted: txt => {
                if (txt.length >= 3) {
                    const res = JSON.parse(
                        JSON.stringify(model.getClientByName(txt))
                    );
                    print("## res: " + JSON.stringify(res, null, 2));

                    if(res) {
                        model.clearModel();
                        model.add(res);
                        print("## if(res): " + JSON.stringify(res, null, 2));
                    }
                }
            }
        }*/

        RowLayout {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            Layout.rightMargin: 10
            ToolButton {
                background: Image {
                    antialiasing: false
                    source: "qrc:/icons/search.svg"
                    sourceSize: iconSize
                }
                Layout.rightMargin: 19
            }

            ToolButton {
                background: Image {
                    source: "qrc:/icons/settings.svg"
                    sourceSize: iconSize
                }
                onClicked: filter.open()
            }
        }
    }

    Filter {
        id: filter
        clientModel: clientModel
    }
}
