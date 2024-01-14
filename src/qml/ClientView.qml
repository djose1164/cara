import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    width: orderView.width
    height: orderView.height * .15
    readonly property int clientID: id
    readonly property string name: forename + " " + surname

    ColumnLayout {
        anchors.fill: parent

        Label {
            Layout.topMargin: 15
            text: forename + " " + surname
            font.pointSize: 18
        }

        Label {
            text: "Cliente ID #" + id
            font.pointSize: 16
        }
    }
}
