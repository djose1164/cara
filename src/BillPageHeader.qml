import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/search" as Search
import "qrc:/customers"

ToolBar {
    id: root
    signal openFilter
    required property StackView stackView
    required property CustomerModel customerModel

    RowLayout {
        anchors.fill: parent

        Label {
            text: "Todas las facturas"
            font.bold: true
            font.pointSize: 16
            Layout.alignment: Qt.AlignCenter
        }

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
                onClicked: stackView.push("qrc:/bills/BillSearch.qml", {
                                              "customerModel": customerModel
                                          })
            }

            ToolButton {
                background: Image {
                    source: "qrc:/icons/settings.svg"
                    sourceSize: iconSize
                }
                onClicked: openFilter()
            }
        }
    }
}
