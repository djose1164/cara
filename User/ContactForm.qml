import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils
import "qrc:/qt/qml/Utils/utils.js" as JS

ColumnLayout {
    id: root
    spacing: 20

    property alias email: email.text
    property alias phoneNumber: phoneNumber.text

    readonly property var contact: {
        "telephone": phoneNumber.text,
        "email": email.text,
    }
    readonly property bool empty: !root.email && !phoneNumber.text
    readonly property bool same: internal.oldEmail === root.email
                                 && internal.oldPhoneNumber === root.phoneNumber

    Component.onCompleted: {
        root.setOldProperties();
    }

    GroupBox {
        Layout.fillWidth: true
        title: "Informacion de Contacto"

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

            NumberTextField {
                id: phoneNumber
                Layout.fillWidth: true
                placeholderText: "Numero telefonico"
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

    QtObject {
        id: internal
        property string oldPhoneNumber
        property string oldEmail
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

    function validate(): void {
        if (JS.isEmpty(phoneNumber.text))
            throw "El numero telefonico no puede estar vacio.";
        else if (JS.isEmpty(email.text))
            throw "El correo electronico no puede estar vacio";
        else if (!email.acceptableInput)
            throw "El Correo electronico no es valido.";
    }

    function setOldProperties() {
        internal.oldEmail = root.email;
        internal.oldPhoneNumber = root.phoneNumber;
    }

    function resetSame() {
        root.setOldProperties();
    }
}
