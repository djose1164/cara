import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Salesperson
import User
import "qrc:/utils"
import "qrc:/customers"

Page {
    id: root

    required property int adminId
    readonly property StackView stackView: StackView.view

    header: TitleHeader {
        title: root.title
        backBtn.onClicked: stackView.pop()
    }

    title: "Nuevo Vendedor"

    ColumnLayout {
        id: gridLayout
        anchors {
            fill: parent
            topMargin: root.height * .1
            bottomMargin: root.height * .1
            leftMargin: 10
            rightMargin: 10
        }
        spacing: 20

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
    }

    AssociateSalespersonService {
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
