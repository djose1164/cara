import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Utils
import "qrc:/qt/qml/Cara/Utils/utils.js" as JS

GroupBox {
    id: root

    readonly property bool empty: !contactItem.email && !contactItem.phoneNumber && !contactItem.homePhone
    readonly property bool same: internal.oldEmail === contactItem.email
                                 && internal.oldPhoneNumber === contactItem.phoneNumber
                                 && internal.oldHomePhone === contactItem.homePhone
    property alias email: contactItem.email
    property alias phoneNumber: contactItem.phoneNumber

    title: "Informacion de Contacto"
    Component.onCompleted: {
        root.setOldProperties();
    }

    ContactInfoItem {
        id: contactItem
        anchors.fill: parent
    }

    QtObject {
        id: internal
        property string oldPhoneNumber
        property string oldEmail
        property string oldHomePhone
    }

    function fillFields(contact: var) {
        contactItem.phoneNumber = contact.telephone;
        contactItem.email = contact.email;
        contactItem.homePhone = contact.homephone;
    }

    function validate(): void {
        contactItem.validate();
    }

    function setOldProperties() {
        internal.oldEmail = contactItem.email;
        internal.oldPhoneNumber = contactItem.phoneNumber;
        internal.oldHomePhone = contactItem.homePhone;
    }

    function resetSame() {
        root.setOldProperties();
    }

    function toJson() {
        return contactItem.toJson();
    }
}
