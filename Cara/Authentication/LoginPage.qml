import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.User
import Cara.Authentication
import Cara.Salesperson
import Cara.Controls

Item {
    id: root

    readonly property T.StackView stackView: StackView.view
    readonly property alias usernameField: usernameInput
    readonly property alias passwordField: passwordInput
    readonly property alias loginBtn: loginBtn

    signal loggedIn
    signal errorOccurred(errorString: string)

    ColumnLayout {
        id: loginInfo
        anchors.fill: parent
        anchors.margins: spacing
        spacing: 20

        Image {
            id:loginLbl
            source: "qrc:/cara-logo-v1.png"
            sourceSize: Qt.size(128, 128)
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
        }

        TextField {
            id: usernameInput
            placeholderText: "Usuario"
            inputMethodHints: Qt.ImhLowercaseOnly
            focus: true
            font.pointSize: 15
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            EnterKey.type: Qt.EnterKeyNext

            ToolTip {
                id: usernameToolTip
                objectName: "usernameToolTip"
                y: usernameInput.height
            }
        }

        PasswordInput {
            id: passwordInput
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            font.pointSize: 15
            Keys.onReturnPressed: if (internal.areInputsValid) root.requestAuthentication()

            ToolTip {
                id: passwordToolTip
                objectName: "passwordToolTip"
                y: passwordInput.height
            }
        }

        Item {
            implicitWidth: buttonsRowLayout.implicitWidth
            implicitHeight: buttonsRowLayout.implicitHeight
            Layout.alignment: Qt.AlignHCenter

            ColumnLayout {
                id: buttonsRowLayout
                anchors.centerIn: parent
                spacing: 20

                Button {
                    id: loginBtn
                    text: "Iniciar Sesion"
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignCenter
                    font.capitalization: Font.MixedCase
                    Material.background: Material.Blue
                    Material.foreground: "white"
                    onClicked: {
                        if (internal.areInputsValid) {
                            login.authenticate();
                            return;
                        }
                        if (internal.isUsernameEmpty)
                            usernameToolTip.show("* El usuario es requerido", 1000);
                        if (internal.isPasswordEmpty)
                            passwordToolTip.show("* La contraseña es requerida", 1000);
                    }
                }

                Label {
                    text: '¿Aun no tienes una cuenta? [Registrate](SignUp.qml)'
                    onLinkActivated: link => root.stackView.replaceCurrentItem(Qt.resolvedUrl(link));
                    textFormat: Label.MarkdownText
                    Layout.alignment: Qt.AlignCenter
                    font.pointSize: 12
                }

                Label {
                    text: '[Continuar sin registrarte](qrc:/qt/qml/Cara/MainPage.qml)'
                    onLinkActivated: link => root.stackView.popCurrentItem();
                    textFormat: Label.MarkdownText
                    Layout.alignment: Qt.AlignCenter
                    font.pointSize: 12
                }
            }
        }
    }

    MessageDialog {
        id: popup
        objectName: "popup"
        text: switch (login.status) {
            case Login.InvalidCredentials:
                return "Usuario o contraseña incorrecta.";
            default:
                return "Ha ocurrido un error. Favor de intentar más tarde.";
        }
        onButtonClicked: close()
    }

    BusyPopup {
        id: busyPopup
        objectName: "busyIndicator"
        width: root.width * .7
        height: root.height/2
        visible: login.status === Login.Busy
    }

    TapHandler {
        onTapped: {
            usernameInput.focus = false
            passwordInput.focus = false
            Qt.inputMethod.hide()
        }
    }

    Login {
        id: login

        authenticator: RestUserAuthentication {
            id: auth
            credentials {
                username: usernameInput.text
                password: passwordInput.text
            }
        }

        onAuthenticated: session => {
            // Prevent crashing when deleting this view.
            busyPopup.modal = false;

            // MainPage.qml is already in stack. No need to replace.
            if (root.stackView)
                root.stackView.popCurrentItem();
            SessionManager.startSession(session);
            //SalespersonSettings.setEntity(session.seller);
            root.loggedIn();
        }

        onErrorOccurred: errorString => {
            popup.open();
            root.errorOccurred(errorString);
        }
    }

    QtObject {
        id: internal
        readonly property url mainPage: "qrc:/qt/qml/Cara/MainPage.qml"
        readonly property bool areInputsValid: !isUsernameEmpty && !isPasswordEmpty
        readonly property bool isUsernameEmpty: usernameInput.text === ""
        readonly property bool isPasswordEmpty: passwordInput.text === ""
    }

    function requestAuthentication() {
        login.authenticate();
    }
}
