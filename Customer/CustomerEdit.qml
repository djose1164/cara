import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import Utils
import "qrc:/utils"

Drawer {
    id: root

    readonly property var customer: {
        "forename": personalInfoForm.forename,
        "surname": personalInfoForm.surname,
        "contact": contactForm.contact,
    };

    property int customerId
    required property StackView stackView

    onAboutToShow: {
        if (customerId < 0)
            return;

        customerApi.fetchCustomerById(customerId);
    }

    DrawerHeader {
        id: header
        anchors.top: root.top
        width: root.width
        title: "Actualizar Datos"
        onCloseClicked: root.close()
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

            PersonalInformationForm {
                id: personalInfoForm
            }

            ContactForm {
                id: contactForm
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            AddressForm {
                id: addressForm
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            Button {
                Layout.fillWidth: true
                Layout.topMargin: 15
                Layout.bottomMargin: 25
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
        onButtonClicked: close()
    }

    CustomerAPI {
        id: customerApi

        onCustomerUpdated: {
            busyIndicator.close();
            dialog.open();
        }
        onCustomerFetched: function(customer) {
            personalInfoForm.forename = customer.forename;
            personalInfoForm.surname = customer.surname;

            if (customer)
                contactForm.fillFields(customer);
            if (customer.address)
                addressForm.fillFields(customer.address);
        }
    }
}
