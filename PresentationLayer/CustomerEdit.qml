import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/utils"

Drawer {
    id: root

    readonly property var customer: {
        "forename": forename.text,
        "surname": surname.text,
        "contact": contactForm.contact,
    };

    property int customerId

    onCustomerIdChanged: {
        if (customerId < 0)
            return;

        customerApi.fetchCustomerById(customerId);
    }

    Row {
        id: header
        anchors.top: root.top
        width: root.width
        height: 50

        CloseButton {
            onClicked: close()
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: "Actualizar Datos"
            font.pointSize: 14
            font.bold: true
        }
    }

    ScrollView {
        id: content
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            leftMargin: 15
        }
        height: root.height - header.height
        contentWidth: availableWidth

        ColumnLayout {
            anchors.fill: parent
            anchors.rightMargin: 15
            GroupBox {
                title: "Informacion Personal"
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent
                    TextField {
                        id: forename
                        placeholderText: "Nombre"
                        Layout.fillWidth: true
                    }

                    TextField {
                        id: surname
                        placeholderText: "Apellido"
                        Layout.fillWidth: true
                    }
                }
            }

            ContactForm {
                id: contactForm
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            Button {
                Layout.fillWidth: true
                Layout.topMargin: 15
                Layout.bottomMargin: 15
                text: "Actualizar"
                onClicked: {
                    busyIndicator.open();

                    contactForm.removeEmptyFields();

                    customerApi.updateCustomer(customerId, customer);
                }
            }
        }
    }

    Popup {
        id: busyIndicator
        width: 200
        height: 200
        modal: true
        anchors.centerIn: Overlay.overlay

        BusyIndicator { anchors.centerIn: parent }
    }

    MessageDialog {
        id: dialog
        text: "Actualizado con exito!"
    }

    ClientRequest {
        id: customerApi

        onCustomerUpdated: {
            busyIndicator.close();
            dialog.open();
        }
        onCustomerFetched: function(customer) {
            forename.text = customer.forename;
            surname.text = customer.surname;
            if (customer.contact)
                contactForm.fillFields(customer.contact);
        }
    }
}
