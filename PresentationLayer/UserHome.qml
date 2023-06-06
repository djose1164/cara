import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"
import com.cara

Home {
    id: root

    readonly property string fullName: TheCurrentUser.isUser ?
                                           ` ${TheCurrentUser.forename} ${TheCurrentUser.surname}`
                                         : ""

    Column {
        id: column

        spacing: 20

        Welcome {
            id: welcome
            width: root.width
            fullName: root.fullName
        }

        Offers {
            id: offers
            width: root.width
            height: root.height*.4
            stackView: root.stackView
        }

        RecentOrders {
            id: recentOrders
            width: root.width
            stackView: root.stackView
            bar: root.bar
        }
    }
}
