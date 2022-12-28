import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/search" as Search

ToolBar {
    required property var model

    id: root
    RowLayout {
        anchors.fill: parent

        Search.SearchBar {
            placeholderText: "Ingrese nombre de cliente o ID"
            //Layout.leftMargin: 20
            Layout.rightMargin: 40
            Layout.alignment: Qt.AlignHCenter
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
        }

        ToolButton {
            background: Image {
                source: "qrc:/icons/settings.svg"
                sourceSize: iconSize
            }
            onClicked: filter.open()
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 5
            Layout.rightMargin: 5
        }
    }

    Filter {
        id: filter
        listModel: root.model.model
    }
}
