import QtQuick
import QtQuick.Controls
import User

Item {
    id: root

    property var user: UserSettings.entity()

    Label {
        width: parent.width * .8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: height/2
        wrapMode: Label.WordWrap
        textFormat: Label.RichText
        text: statusText()
        font.pointSize: 15
        horizontalAlignment: Label.AlignHCenter
        onLinkActivated: url => appStack.pushItem(url)
    }

    function statusText() : string {
        if (user.userType === UserEntity.Guest)
            return "Necesitas registrarte para poder ver tus ordenes recientes."
                    + "<br><br>" + '<a href="qrc:/qt/qml/Authentication/SignUp.qml">Ir a registrarme</a>';
        else if (user.isCustomer)
            return "Aun no haz realizado ninguna orden.";

        return "Error en mostrar todas las ordenes";
    }
}
