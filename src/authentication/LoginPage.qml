import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import User
import core
import Authentication
import "qrc:/main"
import "qrc:/singleton"

Item {
    id: root
    readonly property bool areInputsValid: usernameInput.text !== ""
                                           && passwordInput.text !== ""

    Image {
        id:loginLbl
        anchors.top:  parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/cara-logo-v1.png"
        sourceSize: "128x128"
    }

    ColumnLayout {
        id: loginInfo
        width: parent.width > root.height ? root.width/2 : root.width*.9
        height: parent.height * .3
        anchors.centerIn: parent
        spacing: 20

        TextField {
            id: usernameInput
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            placeholderText: "Usuario"
            inputMethodHints: Qt.ImhLowercaseOnly
            font.pointSize: 15
            EnterKey.type: Qt.EnterKeyNext
            focus: true
        }

        PasswordInput {
            id: passwordInput
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            font.pointSize: 15
            Keys.onReturnPressed: requestAuthentication()
        }
    }

    ColumnLayout {
        id: buttonsRowLayout
        width: parent.width
        height: 100
        anchors.top: loginInfo.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        Button {
            id: loginBtn

            onClicked: {
                if (areInputsValid) {
                    requestAuthentication();
                } else {
                    usernameInput.focus = true;
                }
            }
            text: "Iniciar Sesion"
            font.pointSize: 12
            Layout.alignment: Qt.AlignCenter
            font.capitalization: Font.MixedCase
            Material.background: Material.Blue
            Material.foreground: "white"
        }

        Label {
            text: '¿Aun no tienes una cuenta? [Registrate](SignUp.qml)'
            onLinkActivated: link => appStack.replace(link)
            textFormat: Label.MarkdownText
            Layout.alignment: Qt.AlignCenter
            font.pointSize: 12
        }

        Label {
            text: '[Continuar sin registrarte](qrc:/main/MainPage.qml)'
            onLinkActivated: link => appStack.pop()
            textFormat: Label.MarkdownText
            Layout.alignment: Qt.AlignCenter
            font.pointSize: 12
        }
    }

    MessageDialog {
        id: popup
        text: switch (login.status) {
            case Login.InvalidCredentials:
                return "Usuario o contraseña incorrecta.";
            default:
                return "Ha ocurrido un error. Favor de intentar mas tarder";
        }
        onButtonClicked: close()
    }

    BusyPopup { id: busyIndicator }

    TapHandler {
        onTapped: {
            usernameInput.focus = false
            passwordInput.focus = false
            Qt.inputMethod.hide()
        }
    }

    Login {
        id: login
        username: usernameInput.text
        password: passwordInput.text
        userStore: LocalUserStore {}
        onAuthenticated: {
            appStack.pop();
            LoginManager.authenticated();
            // TODO: Pass the created user to MainPage while showing the busyIndicator
        }
        onErrorOccurred: {
            busyIndicator.close();
            popup.open();
        }
    }

    function requestAuthentication() {
        busyIndicator.open();
        login.authenticateCredentials();
    }
}
