import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Utils
import Cara.Customer
import "qrc:/qt/qml/Cara/Utils/utils.js" as JS

GroupBox {
    id: root

    readonly property alias username: usernameField.text
    readonly property alias password: passwordField.text
    readonly property alias email: emailField.text

    title: "Credenciales"
    GridLayout {
        anchors.fill: parent
        columns: 2
        rowSpacing: 20
        columnSpacing: 10

        TextField {
            id: usernameField
            placeholderText: "Usuario"
            Layout.fillWidth: true
        }

        TextField {
            id: passwordField
            placeholderText: "Contraseña"
            Layout.fillWidth: true
        }

        TextField {
            id: emailField
            Layout.fillWidth: true
            Layout.columnSpan: 2
            placeholderText: "Correo electronico..."
            validator: RegularExpressionValidator {
                regularExpression: /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
            }
        }
    }

    function validate(): void {
        if (JS.isEmpty(usernameField.text))
            throw "El usuario no puede estar vacio.";
        else if (JS.isEmpty(passwordField.text))
            throw "La contraseña no puede estar vacia."
        else if (!emailField.acceptableInput)
            throw "El correo electronico no es valido."
    }

    function clear(): void {
        usernameField.clear();
        passwordField.clear();
        emailField.clear();
    }
}
