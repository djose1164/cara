/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    required property int customer_id
    required property string forename
    required property string surname
    readonly property string name: forename + " " + surname

    width: orderView.width
    height: orderView.height * .15

    onClicked: optionPopup.open()

    ColumnLayout {
        anchors.fill: parent

        Label {
            Layout.topMargin: 15
            text: name
            font.pointSize: 18
        }

        Label {
            text: "Cliente ID #" + customer_id
            font.pointSize: 16
        }
    }

    OptionPopup {
        id: optionPopup
        clientId: customer_id
    }
}
