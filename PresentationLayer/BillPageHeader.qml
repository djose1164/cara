import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara
import "qrc:/utils"
import "qrc:/search" as Search
import "qrc:/customers"
import "qrc:/singleton"

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
            visible: CurrentUser.isAdmin
            onClicked: doSearch()
        }

        Loader {
            id: loader
            Layout.alignment: Qt.AlignRight
        }
    }
}
