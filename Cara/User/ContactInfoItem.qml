import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls

Item {
    id: root

    default property alias contentChildren: columnLayout.data
    property int sideMargin: 15
    property alias email: email.text
    property alias phoneNumber: phoneNumber.text
    property alias homePhone: homePhone.text

    implicitWidth: columnLayout.implicitWidth
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.rightMargin: root.sideMargin
        anchors.leftMargin: root.sideMargin
        spacing: 20

        NumberTextField {
            id: phoneNumber
            Layout.fillWidth: true
            placeholderText: "Numero celular (movil)"
        }

        NumberTextField {
            id: homePhone
            Layout.fillWidth: true
            placeholderText: "Numero telefónico (casa)"
        }

        TextField {
            id: email
            Layout.fillWidth: true
            placeholderText: "Correo electrónico"
            validator: RegularExpressionValidator {
                regularExpression: /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
            }
        }
    }

    function validate() {
        const msgTemplate = "El campo de %1 no puede estar vacio.";
        if (phoneNumber.text.length === 0)
            throw new Error(msgTemplate.arg(phoneNumber.placeholderText))
        else if (!email.acceptableInput)
            throw new Error(email.placeholderText + " invalido.")
    }

    function toJson() {
        return {
            telephone: phoneNumber.text,
            homephone: homePhone.text,
            email: email.text.length > 0 ? email.text : null
        };
    }
}
