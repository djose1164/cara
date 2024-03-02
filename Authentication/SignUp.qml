import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Utils

Item {
    id: signUp

    readonly property T.StackView stackView: StackView.view
    property int fontSize: 14

    Image {
        id: cara
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        source: "https://iili.io/HXfzSQj.png"
        sourceSize: "128x128"
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

    GridLayout {
        id: completeNameRL
        width: signUp.width*.8 > signUp.height ? signUp.width/2 : signUp.width
        height: signUp.height/2
        anchors.centerIn: parent
        columns: 2

        TextField {
            id: forenameInput
            placeholderText: "Nombre(s)"
            Layout.fillWidth: true
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pointSize: fontSize
            EnterKey.type: Qt.EnterKeyNext
        }

        TextField {
            id: surnameInput
            placeholderText: "Apellido(s)"
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            font.pointSize: fontSize
            EnterKey.type: Qt.EnterKeyNext
        }


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
        Layout.columnSpan: 2
        font.pointSize: fontSize
        EnterKey.type: Qt.EnterKeyNext
    }

    PasswordInput {
        id: passwordInput
        Layout.fillWidth: true
        Layout.leftMargin: 15
        Layout.rightMargin: 15
        font.pointSize: fontSize
        Layout.columnSpan: 2
    }
    }
    ColumnLayout {
        id: footerRL
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        spacing: 20

        Button {
            Layout.alignment: Qt.AlignCenter
            text: "Registrarse"
            font.pointSize: fontSize
            onClicked: onOk()
        }

        // TODO: Delete weak reference of `appStack`.
        Label {
            text: '¿Ya tienes una cuenta? [Inicia sesion](LoginPage.qml)'
            onLinkActivated: link => appStack.replace(link)
            textFormat: Label.MarkdownText
            Layout.alignment: Qt.AlignCenter
            font.pointSize: 12
        }
    }

    BusyPopup {
        id: busyIndicator
    }

    Message {
        id: popup
    }

    CaraApi {
        id: request
        endpoint:  "/users"
        onSucceeded: stackView.pop();
    }

    Connections {
        target: request

        function noInternetHandler() {
            popup.text = "Necesitas conectarte a internet.";
        }

        function onFailed(type) {
            let text = type === "singUp" ? "Usario o contraseña incorrecto." : "Datos invalidos."
            popup.text = text;
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
        print(`OnNewUser: ${JSON.stringify(newUser, null, 2)}`);
        busyIndicator.open();
        request.post("/", newUser);
    }
}
