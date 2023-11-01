import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Page {
    id: root

    required property int salespersonId
    required property int creditLimit
    required property int creditAvailable
    required property int creditConsumed
    required property var personInfo

    readonly property StackView stackView: StackView.view

    title: "Detalles de vendedor"
    header: TitleHeader {
        title: root.title
        backBtn.onClicked: stackView.pop()
        btnLoader.sourceComponent: MoreVertButton {
            onClicked: menu.open()

            Menu {
                id: menu

                Repeater {
                    model: listModel
                    MenuItem {
                        required text
                    }
                }

                ListModel {
                    id: listModel

                    ListElement { text: "Ver clientes" }
                    ListElement { text: "Ver estadisticas" }
                    ListElement { text: "Ver inventario" }
                }
            }
        }
    }

    GridLayout {
        anchors.fill: parent
        columns:2

        Label {
            text: personInfo.name
            font.pointSize: 12
            Layout.columnSpan: 2
        }

        Label {
            text: "Limite de credito actual: " + creditLimit
        }

        Label {
            text: "Credito disponible: " + creditAvailable
        }

        Label {
            text: "Credito consumido: " + creditConsumed
        }
    }
}
