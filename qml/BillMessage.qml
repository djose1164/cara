import QtQuick
import QtQuick.Controls

Item {
    id: root

    property var user

    Label {
        width: parent.width * .8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: height/2
        wrapMode: Label.WordWrap
        text: statusText()
        font.pointSize: 15
        horizontalAlignment: Label.AlignHCenter
    }

    function statusText() : string {
        if (user.userType === UserEntity.Guest)
            return "Necesitas registrarte para poder ver tus ordenes recientes.";
        else if (user.isCustomer)
            return "Aun no haz realizado ninguna orden.";

        return "Error en mostrar todas las ordenes";
    }
}
