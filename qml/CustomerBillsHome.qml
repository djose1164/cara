import QtQuick
import QtQuick.Controls
import core

BillHomePage {
    header: null
    customerId: CurrentUser.user.customerId
    stackView: billStack
    Component.onCompleted: print("customerId "+customerId)
}
