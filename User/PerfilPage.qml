import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import QtQuick.Dialogs
import Utils
import User
import Salesperson
import Customer

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    title: "Perfil"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
        btnLoader.sourceComponent: saveBtn
    }

    states: [
        State {
            name: "busy"
            PropertyChanges {
                target: busyIndicator
                visible: true
            }
        }
    ]
    Component.onCompleted: {
        userController.getUserById(UserSettings.userId);
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: root.width - (scrollView.effectiveScrollBarWidth * 2)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            PersonalInformationForm {
                id: personalInfoForm
                forename: UserSettings.forename
                surname: UserSettings.surname
                Layout.topMargin: 10
            }

            ContactForm {
                id: contactForm
                email: UserSettings.email
                phoneNumber: UserSettings.phoneNumber
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            AddressForm {
                id: addressForm
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            // TODO: Show salesperson details of this user.
            CreditOverview {
                visible: UserSettings.userTypeId === UserEntity.Salesperson
            }

            Item { height: 1 }
        }
    }

    ProcessingIndicator {
        id: busyIndicator
    }

    Component {
        id: saveBtn
        ToolButton {
            enabled: !personalInfoForm.same || !contactForm.same || !addressForm.same
            padding: 12
            text: "Guardar"
            font.pointSize: 12
            font.bold: true
            onClicked: {
                const tmp = {
                    forename: personalInfoForm.forename,
                    surname: personalInfoForm.surname,
                    user_id: UserSettings.userId
                };

                const data = Object.assign(tmp, contactForm.contact);
                data.address = addressForm.address;
                console.log(JSON.stringify(data, null, 2));

                root.state = "busy";
                userController.updateContact(data);
            }
        }
    }

    MessageDialog {
        id: message
    }

    UserController {
        id: userController
        onFetched: resp =>{
            const user = JSON.parse(resp).user;
            const address = user.contact.address;
            if (address)
                addressForm.fillFields(address);
        }
        onContactUpdated: {
            UserSettings.phoneNumber = contactForm.phoneNumber;
            UserSettings.email = contactForm.email;
            root.state = "";
            message.title = "Datos actualizados";
            message.open();

            root.updateOldValuesFromForms();
        }
        onErrorOccurred: {
            root.state = "";
            message.title = "Error al actualizar";
            message.text = errorString;
            message.open();
        }
    }

    function updateOldValuesFromForms() {
        addressForm.resetSame();
        personalInfoForm.resetSame();
        contactForm.resetSame();
    }
}
