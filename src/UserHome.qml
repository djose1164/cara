import QtQuick
import QtQuick.Controls
import "qrc:/users"

Home {
    id: root

    readonly property string fullName: TheCurrentUser.forename + " "
                                       + TheCurrentUser.surname

    Label {
        id: lbl
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        text: `¡Bienvenido ${fullName}, mira lo que tenemos disponible para ti!`
        font.pointSize: 13
        wrapMode: Text.WordWrap

    }
}
