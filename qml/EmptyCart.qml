import QtQuick
import QtQuick.Controls
import User

Item {
    id: root

    Label {
        anchors.fill: parent
        text: {
            if (UserSettings.userTypeId === UserEntity.Customer)
                return "El carrito esta vacio, intenta agregar algunos productos.";
            else
                return "Necesitas inicar sesion para agregar productos a tu carrito."
                        + '<br><br><a href="qrc:/qt/qml/Authentication/LoginPage.qml">Iniciar sesion</a>';
        }
        font.pointSize: 16
        horizontalAlignment: Label.AlignHCenter
        verticalAlignment: Label.AlignVCenter
        wrapMode: Label.Wrap
        onLinkActivated: url => appStack.pushItem(url)
    }
}
