/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    required property string forename
    required property string surname
    required property int id
    readonly property int clientID: id
    readonly property string name: forename + " " + surname

    width: orderView.width
    height: orderView.height * .15

    onClicked: optionPopup.open()

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

    OptionPopup {
        id: optionPopup
        clientId: id
    }
}
