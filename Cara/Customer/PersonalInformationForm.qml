import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Customer


GroupBox {
    id: root

    property alias forename: personalInfo.forename
    property alias surname: personalInfo.surname
    readonly property bool same: internal.oldForename === root.forename
                                 && internal.oldSurname === root.surname

    title: "Informacion Personal"
    Layout.fillWidth: true
    Component.onCompleted: {
        root.setOldProperties();
    }

    PersonalInfoItem {
        id: personalInfo
        anchors.fill: parent
    }

    QtObject {
        id: internal
        property string oldForename
        property string oldSurname
    }

    function validate(): void {
        personalInfo.validate();
    }

    function clear(): void {
        personalInfo.clear();
    }

    function resetSame() {
        setOldProperties();
    }

    function setOldProperties() {
        internal.oldForename = root.forename;
        internal.oldSurname = root.surname;
    }

    function toJson() {
        return personalInfo.toJson();
    }

    function fillFields(person: var) {
        personalInfo.fillFields(person);
    }
}

