import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"
import core

Home {
    id: root

    readonly property string fullName: TheCurrentUser.isUser ?
                                           ` ${TheCurrentUser.forename} ${TheCurrentUser.surname}`
                                         : ""

    Column {
        id: column
        bottomPadding: 50

        Welcome {
            id: welcome
            width: root.width
            fullName: root.fullName
        }

        Offers {
            id: offers
            width: root.width
            height: root.height - welcome.height - 40
            stackView: root.stackView
        }
    }
}
