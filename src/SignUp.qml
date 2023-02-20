import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

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
            id: forenameInput
            placeholderText: "Nombre(s)"
            Layout.fillWidth: true
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pixelSize: fontSize
            EnterKey.type: Qt.EnterKeyNext
        }

        TextField {
            id: surnameInput
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
            id: emailInput
            Layout.fillWidth: true
            placeholderText: "Correo electronico"
            inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhPreferLowercase | Qt.ImhNoAutoUppercase
            validator: RegularExpressionValidator {
                regularExpression: /[a-z0-9]+@[a-z]+\.[a-z]{2,3}/
            }

            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pixelSize: fontSize
            EnterKey.type: Qt.EnterKeyNext
        }

        PasswordInput {
            id: passwordInput
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
            Layout.alignment: Qt.AlignCenter
            text: "Registrarse"
            font.pixelSize: fontSize
            onClicked: onOk()
        }

        Label {
            text: '<html><style type="text/css"></style><a href="qrc:/login/Login.qml">Iniciar sesion</a></html>'
            onLinkActivated: link => mainStackView.replace(link)
        }
    }

    BusyPopup {
        id: busyIndicator
    }

    MessagePopup {
        id: popup
    }

    Request {
        id: request
        base: "http://127.0.0.1:5000/api/users"
    }

    Connections {
        target: request

        function noInternetHandler() {
            popup.text = "Necesitas conectarte a internet.";
        }

        function onFailed(type) {
            let text = type === "singUp" ? "Usario o contrasena incorrecto." : "Datos invalidos."
            popup.text = text;
        }

        function onSucceeded() {
            mainStackView.replace("qrc:/home/UserHome.qml");
        }

        function onReceived(response) {
            busyIndicator.close();
            if (response.code !== "success") {
                popup.text = response.message;
                popup.open();
            }
        }
    }

    function onOk() {
        const newUser = {
            "forename": forenameInput.text,
            "surname": surnameInput.text,
            "email": emailInput.text,
            "password": passwordInput.text,
        };
        busyIndicator.open();
        request.post("/", newUser);
    }
}
