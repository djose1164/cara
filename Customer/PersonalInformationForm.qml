import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import Utils
import Customer

import "qrc:/main"
import "qrc:/qt/qml/Utils/utils.js" as JS

GroupBox {
    id: root

    property alias forename: forename.text
    property alias surname: surname.text

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

    function validate(): void {
        if (JS.isEmpty(forename.text))
            throw "El nombre no puede estar vacio.";
        else if (JS.isEmpty(surname.text))
            throw "El apellido no puede estar vacio.";
    }

    function clear(): void {
        forename.clear();
        surname.clear();
    }
}

