import QtQuick
import Cara.User

Home {
    id: root

    Column {
        id: column
        bottomPadding: 50

        Welcome {
            id: welcome
            width: root.width
            fullName: UserSettings.forename ?? "Invitado"
        }

        Offers {
            id: offers
            width: root.width
            height: root.height - welcome.height - 40
            stackView: root.stackView
        }
    }
}
