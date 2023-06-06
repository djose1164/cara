import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import com.cara 1.0
import "qrc:/main"
import "qrc:/singleton"

Item {
    id: root
    readonly property bool areInputsValid: usernameInput.text !== ""
                                           && passwordInput.text !== ""
    readonly property var handlers : {
        "success": successHandler,
        "no_internet": noInternetHandler,
        "credentialsNotValid": failedHandler,
        "invalidInput": failedHandler,
        "notFound": () => popup.text = "Cuenta no registrada.",
    }

    //width: parent.width > parent.height ? parent.width/2 : parent.width
    //height: parent.height > parent.width ? parent.height*.95 : parent.height*.6

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
            text: "Iniciar Sesion"
            font.pointSize: 12
            onClicked: areInputsValid ? requestAuthentication() : usernameInput.focus = true
            Layout.alignment: Qt.AlignCenter
            font.capitalization: Font.MixedCase
            Material.background: Material.Blue
            Material.foreground: "white"
        }

        Label {
            text: '¿Aun no tienes una cuenta? [Registrate](qrc:/login/SignUp.qml)'
            onLinkActivated: link => mainStackView.replace(link)
            textFormat: Label.MarkdownText
            Layout.alignment: Qt.AlignCenter
            font.pointSize: 12
        }
    }

    MessageDialog { id: popup }

    Auth {
        id: auth
        userEntity: UserEntity {
            username: usernameInput.text
            password: passwordInput.text
        }

        onAuthenticated: r => {
                             busyIndicator.close()
                             CurrentUser.fillUserInfo(r);
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

    UserEntity {
        username: usernameInput.text
        password: passwordInput.text
    }

    function requestAuthentication() {
        busyIndicator.open();
        if (areInputsValid)
            auth.authenticate();
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
