import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Salesperson
import Cara.User
import Cara.Controls
import Cara.Customer

Page {
    id: root

    required property int adminId
    readonly property T.StackView stackView: StackView.view

    header: TitleHeader {
        title: root.title
        backBtn.onClicked: stackView.pop()
    }

    title: "Nuevo Vendedor"

    component Separator : Item {
        height: 1
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            id: gridLayout
            width: scrollView.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            Separator {}

            UserCredentialsForm {
                id: credentialsForm
                Layout.fillWidth: true
            }

            PersonalInformationForm {
                id: nameForm
                Layout.columnSpan: 2
                Layout.fillWidth: true
            }

            Button {
                text: "Crear"
                Layout.columnSpan: 2
                Layout.fillWidth: true
                onClicked: addSalesperson()
            }

            Separator {}
        }
    }

    SalespersonController {
        id: service
        onAdded: {
            SalespersonManager.add(JSON.parse(data).salesperson)
            message.text = "¡Exito al agregar al vendedor!";
            message.informativeText = "";
            message.open();
            clearForm();
        }
        onErrorOccurred: {
            const content = JSON.parse(data);
            message.text = "No se ha podido agregar al vendedor.";
            message.informativeText = `Error ${content.code}: ${content.message}`;
            message.open();
        }
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onButtonClicked: close()
    }

    function addSalesperson() {
        validate();
        const payload = {
            username: credentialsForm.username,
            password: credentialsForm.password,
            email: credentialsForm.email,
            forename: nameForm.forename,
            surname: nameForm.surname,
            organizationId: SalespersonSettings.organizationId,
            adminId
        }

        service.addAssociateSalesperson(payload);
    }

    function validate(): void {
        try {
            credentialsForm.validate()
            nameForm.validate();
        } catch (e) {
            message.text = e;
            message.open();
        }
    }

    function clearForm() {
        credentialsForm.clear()
        nameForm.clear();
    }
}
