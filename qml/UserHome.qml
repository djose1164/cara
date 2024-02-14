import QtQuick
import User

Home {
    id: root

    Column {
        id: column
        bottomPadding: 50

        Welcome {
            id: welcome
            width: root.width
            fullName: userStore.user.forename ?? "Invitado"
        }

        Offers {
            id: offers
            width: root.width
            height: root.height - welcome.height - 40
            stackView: root.stackView
        }
    }

    LocalUserStore {
        id: userStore
    }
}
