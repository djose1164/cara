import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 20

    readonly property var contact: {
        "telephone": phoneNumber.text,
        "email": email.text,
        "address": addressForm.address,
    }

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

    AddressForm {
        id: addressForm
        Layout.fillWidth: true
    }

    function removeEmptyFields() {
        if (contact.email === "")
            delete contactForm.contact.email;
        if (contact.telephone === "")
            delete contactForm.contact.telephone;
    }
}
