import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/main"
import "qrc:/users"

Page {
    id: root
    readonly property bool areInputsValid: usernameInput.text !== ""
                                           && passwordInput.text !== ""
    readonly property variant handlers : {
        "success": successHandler,
        "no_internet": noInternetHandler,
        "credentialsNotValid": failedHandler,
        "invalidInput": failedHandler,
        "notFound": () => popup.text = "Cuenta no registrada.",
    }

    Item {

        width: parent.width > parent.height ? parent.width/2 : parent.width
        height: parent.height > parent.width ? parent.height*.95 : parent.height*.6
        anchors.centerIn: parent

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
                focus: true
            }

            PasswordInput {
                id: passwordInput
                Layout.preferredWidth: parent.width/1.5
                Layout.alignment: Qt.AlignCenter
                Layout.topMargin: 5
                font.pointSize: 15
                Keys.onReturnPressed: requestAuthentication()
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
            request.onReceived: busyIndicator.close()
            request.onFailed: (response) => handlers[response.code]()
            onAuthenticated: r => {
                TheCurrentUser.init(r);
                TheCurrentUser.isLoggedIn = true;
                successHandler();
            }
            onInvalid: popup.open()
        }

        BusyPopup { id: busyIndicator }

        TapHandler {
            onTapped: {
                usernameInput.focus = false
                passwordInput.focus = false
                Qt.inputMethod.hide()
            }
        }
    }
    function requestAuthentication() {
        busyIndicator.open();
        if (areInputsValid)
            auth.authenticate({"username": usernameInput.text, "password": passwordInput.text});
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
        mainStackView.replace("qrc:/main/MainPage.qml")
    }
}
