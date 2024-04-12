import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Utils
import Customer
import "qrc:/qt/qml/Utils/utils.js" as JS

GroupBox {
    id: root

    property alias forename: forename.text
    property alias surname: surname.text
    readonly property bool same: internal.oldForename === root.forename
                                 && internal.oldSurname === root.surname

    title: "Informacion Personal"
    Layout.fillWidth: true
    Component.onCompleted: {
        root.setOldProperties();
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

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

    QtObject {
        id: internal
        property string oldForename
        property string oldSurname
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

    function resetSame() {
        setOldProperties();
    }

    function setOldProperties() {
        internal.oldForename = root.forename;
        internal.oldSurname = root.surname;
    }
}

