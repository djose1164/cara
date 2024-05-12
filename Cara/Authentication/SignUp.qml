import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import QtQuick.Dialogs
import Cara.Utils
import Cara.User
import Cara.Controls

Item {
    id: signUp

    readonly property T.StackView stackView: StackView.view
    readonly property bool valid: forenameInput.text.length > 0
                                  && surnameInput.text.length > 0
                                  && usernameInput.text.length > 0
                                  && passwordInput.text.length > 0

    property int fontSize: 14

    implicitWidth: 450
    implicitHeight: 600

    LinearProgressIndicator {
        id: indicator
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: swipeView.currentIndex
        count: swipeView.count
        displayText: internal.titleList[currentIndex]
    }

    component NextButton : Button {
        text: "Siguiente"
        Material.background: Material.Blue
        onClicked: swipeView.incrementCurrentIndex()
    }

    component BackButton : Button {
        text: "Atras"
        Material.background: Material.Blue
        onClicked: swipeView.decrementCurrentIndex()
    }

    SwipeView {
        id: swipeView
        anchors {
            top: indicator.top
            topMargin: 5
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Item {
            ColumnLayout {
                anchors.fill: parent

                TextField {
                    id: forenameInput
                    objectName: "forenameField"
                    placeholderText: "Nombre(s)"
                    Layout.fillWidth: true
                    Layout.leftMargin: 15
                    Layout.rightMargin: 15
                    font.pointSize: fontSize
                    EnterKey.type: Qt.EnterKeyNext
                }

                TextField {
                    id: surnameInput
                    objectName: "surnameField"
                    placeholderText: "Apellido(s)"
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    Layout.leftMargin: 15
                    Layout.rightMargin: 15
                    font.pointSize: fontSize
                    EnterKey.type: Qt.EnterKeyNext
                }

                NextButton {
                    Layout.alignment: Qt.AlignCenter
                }

                Label {
                    text: '¿Ya tienes una cuenta? [Inicia sesion](LoginPage.qml)'
                    onLinkActivated: link => stackView.replaceCurrentItem(link)
                    textFormat: Label.MarkdownText
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignCenter
                }

                Label {
                    text: '[Continuar sin registrarme](url)'
                    onLinkActivated: stackView.popCurrentItem()
                    textFormat: Label.MarkdownText
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignCenter
                }
            }

            function validate() {
                const msgTemplate = "El campo de %1 no puede estar vacio.";
                if (forenameInput.text.length === 0)
                    throw new Error(msgTemplate.arg("nombre"), {index: swipeView.currentIndex});
                else if (surnameInput.text.length === 0)
                    throw new Error(msgTemplate.arg("apellido"), {index: swipeView.currentIndex});
            }
        }

        Item {
            width: swipeView.width
            height: swipeView.height

            ColumnLayout {
                anchors.fill: parent

                TextField {
                    id: usernameInput
                    objectName: "usernameField"
                    Layout.fillWidth: true
                    placeholderText: "Usuario"
                    inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhPreferLowercase | Qt.ImhNoAutoUppercase
                    Layout.leftMargin: 15
                    Layout.rightMargin: 15
                    Layout.columnSpan: 2
                    font.pointSize: fontSize
                    EnterKey.type: Qt.EnterKeyNext
                }

                PasswordInput {
                    id: passwordInput
                    objectName: "passwordField"
                    Layout.fillWidth: true
                    Layout.leftMargin: 15
                    Layout.rightMargin: 15
                    font.pointSize: fontSize
                    Layout.columnSpan: 2
                }

                RowLayout {
                    Layout.fillWidth: true

                    BackButton {}

                    NextButton {
                        Layout.alignment: Qt.AlignRight
                    }
                }
            }

            function validate() {
                const msgTemplate = "El campo de %1 no puede estar vacio.";
                if (usernameInput.text.length === 0)
                    throw new Error(msgTemplate.arg("usuario"), {index: swipeView.currentIndex});
                else if (passwordInput.text.length === 0)
                    throw new Error(msgTemplate.arg("contraseña"), {index: swipeView.currentIndex});
            }
        }

        Item {
            ColumnLayout {
                anchors.fill: parent
                spacing: 20

                Button {
                    objectName: "signUpBtn"
                    Layout.alignment: Qt.AlignCenter
                    text: "Registrarse"
                    font.pointSize: fontSize
                    onClicked: onOk()
                }

                RowLayout {
                    Layout.fillWidth: true

                    BackButton {}

                    Button {
                        text: "Registrarse"
                        onClicked: signUp.onOk()
                        Layout.alignment: Qt.AlignRight
                    }
                }
            }
        }
    }

    BusyPopup {
        id: busyIndicator
    }

    Message {
        id: popup
    }

    MessageDialog {
        id: successMsg
        objectName: "successMsg"
        text: "Te has registrado exitosamente"
        informativeText: "Favor de iniciar sesion"
        buttons: MessageDialog.Ok
        onAccepted: stackView.replaceCurrentItem(Qt.resolvedUrl("LoginPage.qml"))
    }

    MessageDialog {
        id: messageDialog
        objectName: "messageDialog"
        buttons: MessageDialog.Ok
    }

    CaraApi {
        id: request
        endpoint:  "/users"
        onSucceeded: successMsg.open()
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

    QtObject {
        id: internal
        readonly property list<string> titleList: ["View A", "View B", "View C"]
    }

    function onOk() {
        try {
            signUp.validate();
            const newUser = {
                contact: {
                    "forename": forenameInput.text,
                    "surname": surnameInput.text,
                },
                "username": usernameInput.text,
                "password": passwordInput.text,
            };
            print(`OnNewUser: ${JSON.stringify(newUser, null, 2)}`);
            busyIndicator.open();
        request.post("/", newUser);
        } catch (err) {
            console.error(err);
            swipeView.setCurrentIndex(err.index);
            messageDialog.title = "Campos invalidos";
            messageDialog.text = err.message;
            messageDialog.open();
        }
    }

    function validate() {
        const msgTemplate = "El campo de %1 no puede estar vacio.";
        if (forenameInput.text.length === 0)
            throw new Error(msgTemplate.arg("nombre"), {index: swipeView.currentIndex});
        else if (surnameInput.text.length === 0)
            throw new Error(msgTemplate.arg("apellido"), {index: swipeView.currentIndex});
        else if (usernameInput.text.length === 0)
            throw new Error(msgTemplate.arg("usuario"), {index: swipeView.currentIndex});
        else if (passwordInput.text.length === 0)
            throw new Error(msgTemplate.arg("contraseña"), {index: swipeView.currentIndex});
    }
}
