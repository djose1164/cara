import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: signUp

    property int fontSize: 22

    Label {
        id: cara
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Cara"
        font.pixelSize: fontSize*1.6
        font.bold: true
    }

    Label {
        id: signUpLbl
        anchors.top: cara.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Registrarse"
        font.pointSize: 20
        font.bold: true
    }

    RowLayout {
        id: completeNameRL
        width: signUp.width
        anchors.top: signUpLbl.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        TextField {
            placeholderText: "Nombre(s)"
            Layout.fillWidth: true
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pixelSize: fontSize
            EnterKey.type: Qt.EnterKeyNext
        }

        TextField {
            placeholderText: "Apellido(s)"
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pixelSize: fontSize
            EnterKey.type: Qt.EnterKeyNext
        }
    }

    ColumnLayout {
        anchors.top: completeNameRL.bottom
        anchors.topMargin: this.height / 3
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        spacing: 20


        TextField {
            Layout.fillWidth: true
            placeholderText: "Correo electronico"
            inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhPreferLowercase | Qt.ImhNoAutoUppercase
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pixelSize: fontSize
            EnterKey.type: Qt.EnterKeyNext
        }

        PasswordInput {
            Layout.fillWidth: true
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pixelSize: fontSize
        }
    }

    RowLayout {
       id: footerRL
       width: parent.width
       anchors.bottom: parent.bottom
       anchors.bottomMargin: 50
       spacing: 20

        Button {

            text: "Registrarse"
            font.pixelSize: fontSize
            onClicked:
                mainStackView.replace("qrc:/qml/Home.qml")
        }

        Label {
            id: link_Text
            text: '<html><style type="text/css"></style><a href="http://google.com">google</a></html>'
            onLinkActivated: link => Qt.openUrlExternally(link)

        }

    }
}
