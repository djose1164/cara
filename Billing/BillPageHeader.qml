import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import User
import Utils
import Customer

ToolBar {
    id: root

    property alias buttonLoader: loader

    required property StackView stackView
    property CustomerModel customerModel

    signal doSearch

    RowLayout {
        anchors.fill: parent

        Label {
            text: "Todas las facturas"
            font.bold: true
            font.pointSize: 16
            Layout.alignment: Qt.AlignCenter
        }

        SearchButton {
            visible: UserSettings.userTypeId === UserEntity.Admin || UserSettings.userTypeId === UserEntity.Salesperson
            onClicked: doSearch()
        }

        Loader {
            id: loader
            Layout.alignment: Qt.AlignRight
        }
    }
}
