import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 20

    readonly property var contact: {
        "telephone": phoneNumber.text,
        "email": email.text,
    }

    GroupBox {
        Layout.fillWidth: true
        title: "Informacion de Contacto"

        ColumnLayout {
            anchors.fill: parent

            TextField {
                id: phoneNumber
                Layout.fillWidth: true
                placeholderText: "Numero telefonico"
                validator: IntValidator {}
            }

            TextField {
                id: email
                Layout.fillWidth: true
                placeholderText: "Correo electronico..."
                validator: RegularExpressionValidator {
                    regularExpression: /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
                }
            }
        }
    }

    function removeEmptyFields() {
        if (contact.email === "")
            delete contactForm.contact.email;
        if (contact.telephone === "")
            delete contactForm.contact.telephone;
    }

    function fillFields(contact: var) {
        phoneNumber.text = contact.telephone;
        email.text = contact.email;
    }
}
