import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/js/test.mjs" as Test
import "qrc:/main"

Item {
    id: root

    property alias isLoggedin: settings.isLoggedin
    readonly property bool areInputsValid: usernameInput.text !== "" && passwordInput.text !== ""
    readonly property variant handlers : {
        "success": successHandler,
        "no_internet": noInternetHandler,
        "credentialsNotValid": failedHandler,
        "invalidInput": failedHandler,
        "notFound": () => popup.text = "Cuenta no registrada.",
    }

    Label {
        id:loginLbl
        anchors.top:  parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Cara"
        font.pointSize: parent.height * .08
    }

    ColumnLayout {
        id: loginInfo
        width: parent.width
        anchors.top: loginLbl.bottom
        anchors.topMargin: 100
        spacing: 40

        component MyTextField: TextField {
            onEditingFinished: {
                focus = false
                Qt.inputMethod.hide()
            }
        }

        MyTextField {
            id: usernameInput
            Layout.preferredWidth: parent.width/1.5
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            placeholderText: "Usuario"
            inputMethodHints: Qt.ImhLowercaseOnly
            font.pointSize: 15
            onEditingFinished: Qt.inputMethod.hide()
        }

        PasswordInput {
            id: passwordInput
            Layout.preferredWidth: parent.width/1.5
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            font.pointSize: 15
        }
    }

    RowLayout {
        id: buttonsRowLayout
        width: parent.width * .9
        anchors.top: loginInfo.bottom
        anchors.topMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            id: loginBtn
            text: "Iniciar Sesion"
            font.pointSize: 12
            onClicked: requestAuthentication()
            Layout.alignment: Qt.AlignCenter
        }

        Button {
            id: signupBtn
            text: "Registrarse"
            font.pointSize: 12
            onClicked: mainStackView.replace("qrc:/main/SignUp.qml")
            Layout.alignment: Qt.AlignCenter
        }
    }

    MessagePopup { id: popup }

    Auth {
        id: auth
        request.onSent: busyIndicator.open()
        request.onReceived: busyIndicator.close()
        request.onFailed: (response) => handlers[response.code]()
        onAuthenticated: successHandler()
        onInvalid: popup.open()
    }

    Settings {
        id: settings
        property bool isLoggedin
        category: "Credentials"
    }

    BusyPopup { id: busyIndicator }

    TapHandler {
        onTapped: {
            usernameInput.focus = false
            passwordInput.focus = false
            Qt.inputMethod.hide()
        }
    }

    function requestAuthentication() {
        if (areInputsValid)
            auth.authenticate({"username": usernameInput.text, "password": passwordInput.text})
        print("Validating login...");
    }

    function noInternetHandler() {
        popup.text = "Necesitas conectarte a internet.";
    }

    function failedHandler() {
        popup.text = "Usario o contrasena incorrecto."
    }

    function successHandler() {
        print("Valid credentials.");
        settings.isLoggedin = true;
        mainStackView.replace("qrc:/main/MainPage.qml")
    }
}
