import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import QtQuick.Dialogs
import Cara.Controls
import Cara.User
import Cara.Controls
import Cara.Customer

Item {
    id: signUp

    readonly property T.StackView stackView: StackView.view

    property int fontSize: 14

    implicitWidth: 450
    implicitHeight: 600

    LinearProgressIndicator {
        id: indicator
        width: signUp.width * .6
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: swipeView.currentIndex
        count: swipeView.count
        displayText: swipeView.currentItem?.title ?? ""
    }

    component NextButton : Button {
        text: "Siguiente"
        Layout.fillWidth: true
        Material.background: Material.Blue
        onClicked: {
            try {
                swipeView.currentItem.validate();
                swipeView.incrementCurrentIndex();
            } catch (err) {
                messageDialog.showErrorString(err.message);
            }
        }
    }

    component BackButton : Button {
        text: "Atras"
        onClicked: swipeView.decrementCurrentIndex()
    }

    SwipeView {
        id: swipeView
        anchors {
            top: indicator.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        //interactive: false

        PersonalInfoItem {
            id: personalInfoForm

            readonly property string title: "Informacion Personal"

            dateRequired: true

            NextButton {
                Layout.alignment: Qt.AlignCenter
            }

            Label {
                text: '¿Ya tienes una cuenta? [Inicia sesion](LoginPage.qml)'
                onLinkActivated: link => stackView.replaceCurrentItem(Qt.resolvedUrl(link))
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

        ContactInfoItem {
            id: contactInfo
            readonly property string title: "Informacion de Contacto"
            RowLayout {
                width: contactInfo.width
                spacing: 30

                BackButton {}

                NextButton {
                    Layout.alignment: Qt.AlignRight
                }
            }
        }

        ScrollView {
            id: scrollView

            readonly property string title: "Dirección"
            readonly property ScrollBar verticalBar: scrollView.ScrollBar.vertical

            contentWidth: availableWidth

            Binding {
                when: Qt.inputMethod.visible
                scrollView.ScrollBar.vertical.position: .33
            }

            AddressItem {
                id: addressItem
                width: scrollView.availableWidth - scrollView.effectiveScrollBarWidth*2
                height: scrollView.availableHeight
                anchors.horizontalCenter: parent.horizontalCenter
                onDonePressed: {
                    validate();
                    swipeView.incrementCurrentIndex();
                }

                RowLayout {
                    width: addressItem.width
                    spacing: 30

                    BackButton {}

                    NextButton {
                        Layout.alignment: Qt.AlignRight
                    }
                }
            }

            function validate() {
                addressItem.validate();
                console.log(addressItem.toJson())
            }

            function toJson() {
                addressItem.toJson();
            }
        }

        Item {
            id: credentialForm
            readonly property string title: "Credenciales"

            ColumnLayout {
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                anchors.topMargin: 30
                anchors.bottomMargin: 30
                spacing: 30

                TextField {
                    id: usernameInput
                    objectName: "usernameField"
                    Layout.fillWidth: true
                    placeholderText: "Usuario"
                    inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhPreferLowercase | Qt.ImhNoAutoUppercase

                    Layout.columnSpan: 2
                    font.pointSize: fontSize
                    EnterKey.type: Qt.EnterKeyNext
                }

                PasswordItem {
                    id: passwordItem
                    objectName: "passwordItem"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 30

                    BackButton {}

                    Button {
                        text: "Registrarse"
                        Layout.fillWidth: true
                        Material.background: Material.Blue
                        onClicked: signUp.onOk()
                    }
                }
            }

            function validate() {
                const msgTemplate = "El campo de %1 no puede estar vacio.";
                if (usernameInput.text.length === 0)
                    throw new Error(msgTemplate.arg("usuario"));
                else if (passwordInput.text.length === 0)
                    throw new Error(msgTemplate.arg("contraseña"));
            }

            function toJson() {
                return {
                    username: usernameInput.text,
                    password: passwordItem.text
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

        function showErrorString(err: string) {
            console.error(err);
            messageDialog.title = "Campos invalidos";
            messageDialog.text = err;
            messageDialog.open();
        }
    }

    UserController {
        id: userController
        onUserRegistered: {
            busyIndicator.close();
            successMsg.open();
        }

        onErrorOccurred: {
            busyIndicator.close();
            messageDialog.showErrorString(errorString);
        }
    }

    Connections {
        target: null

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
        try {
            let signupObj = credentialForm.toJson();
            signupObj.person = personalInfoForm.toJson();
            const person = signupObj.person;
            person.contact = contactInfo.toJson();
            person.address = addressItem.toJson();
            console.log(JSON.stringify(signupObj, null, 2));

            busyIndicator.open();

            userController.signUp(signupObj);

        } catch (err) {
            console.error(err);
            messageDialog.title = "Campos invalidos";
            messageDialog.text = err.message;
            messageDialog.open();
        }
    }

    function validate() {
        const msgTemplate = "El campo de %1 no puede estar vacio.";
        if (forenameInput.text.length === 0)
            throw new Error(msgTemplate.arg("nombre"));
        else if (surnameInput.text.length === 0)
            throw new Error(msgTemplate.arg("apellido"));
        else if (usernameInput.text.length === 0)
            throw new Error(msgTemplate.arg("usuario"));
        else if (passwordInput.text.length === 0)
            throw new Error(msgTemplate.arg("contraseña"));
    }

    function getUser({
        username,
        forename,
        surname,
        userType,
        email,
        birthday,
        address
    }): user {
        return {
            username,
            forename,
            surname,
            userType,
            email,
            birthday,
            address
        };
    }
}
