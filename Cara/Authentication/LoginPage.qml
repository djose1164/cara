import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.User
import Cara.Authentication
import Cara.Salesperson
import Cara.Utils

Item {
    id: root

    readonly property T.StackView stackView: StackView.view

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
            Keys.onReturnPressed: internal.requestAuthentication()
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
                if (internal.areInputsValid) {
                    internal.requestAuthentication();
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
            text: '[Continuar sin registrarte](qrc:/qt/qml/Cara/MainPage.qml)'
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
                return "Ha ocurrido un error. Favor de intentar mas tarde.";
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
        stores: [
            LocalUserStore {
                id: localUserStore
            }
        ]
        authenticator: auth
        onAuthenticated: {
            // MainPage.qml is already in stack. No need to replace.
            stackView.popCurrentItem();
        }
        onErrorOccurred: {
            busyIndicator.close();
            popup.open();
        }
    }

    RestUserAuthentication {
        id: auth
        credentials {
            username: internal.username
            password: internal.password
        }
        onSucceeded: data => {
            LoginManager.loggingIn();
            internal.saveSalespersonSettings(data);
            const user = userController.create(data);
            busyIndicator.close();
            UserSettings.setEntity(user);
            LoginManager.loggedIn();
        }
    }

    UserController {
        id: userController
    }

    SalespersonController {
        id: spController
    }

    QtObject {
        id: internal
        readonly property url mainPage: "qrc:/qt/qml/Cara/MainPage.qml"
        readonly property string username: usernameInput.text
        readonly property string password: passwordInput.text
        readonly property bool areInputsValid: usernameInput.text !== ""
                                               && passwordInput.text !== ""

        function requestAuthentication() {
            busyIndicator.open();
            login.authenticate();
        }

        function saveSalespersonSettings(data) {
            const salespersonJson = JSON.parse(data).user.salesperson;
            const salesperson = spController.fromJsonObject(salespersonJson);
            SalespersonSettings.setEntity(salesperson);
        }
    }
}
