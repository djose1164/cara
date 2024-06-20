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
            anchors.topMargin: 20
            source: "qrc:/cara-logo-v1.png"
            sourceSize: "128x128"
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
                return "Ha ocurrido un error. Favor de intentar mas tarde.";
        }
        onButtonClicked: close()
    }

    BusyPopup {
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
        store: LocalUserStore {
            id: localUserStore
        }

        authenticator: RestUserAuthentication {
            id: auth
            credentials {
                username: internal.username
                password: internal.password
            }
        }

        onAuthenticated: data => {
            const jsonData = JSON.parse(data);
            LoginManager.loggingIn();
            internal.saveSalespersonSettings(data);
            UserSettings.setEntity(userController.create(data));
            LoginManager.loggedIn();
            root.loggedIn();

            // MainPage.qml is already in stack. No need to replace.
            if (root.stackView)
                root.stackView.popCurrentItem();
        }

        onErrorOccurred: errorString => {
            popup.open();
            root.errorOccurred(errorString);
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
        readonly property bool areInputsValid: !isUsernameEmpty && !isPasswordEmpty
        readonly property bool isUsernameEmpty: usernameInput.text === ""
        readonly property bool isPasswordEmpty: passwordInput.text === ""

        function saveSalespersonSettings(data) {
            const salespersonJson = JSON.parse(data).user.salesperson;
            const salesperson = spController.fromJsonObject(salespersonJson);
            SalespersonSettings.setEntity(salesperson);
        }
    }

    function requestAuthentication() {

    }
}
