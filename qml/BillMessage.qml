import QtQuick
import QtQuick.Controls
import core

Item {
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

    Component.onCompleted: print(CurrentUser.user.customerId, CurrentUser.user.userType)

    function statusText() : string {
        if (!CurrentUser.isUser)
            return "Necesitas registrarte para poder ver tus ordenes recientes.";
        else if (!CurrentUser.isCostumer)
            return "Aun no haz realizado ninguna orden.";

        return "Error en mostrar todas las ordenes";
    }
}
