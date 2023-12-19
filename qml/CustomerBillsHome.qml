import QtQuick
import QtQuick.Controls
import User

BillHomePage {
    header: null
    customerId: userStore.user.customerId
    stackView: billStack
    Component.onCompleted: print("customerId "+customerId)

    LocalUserStore { id: userStore }
}
