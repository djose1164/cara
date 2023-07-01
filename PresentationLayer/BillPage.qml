import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    readonly property UserEntity user: CurrentUser.user

    title: "Facturas"
    header: BillPageHeader {
        stackView: root.stackView
        onOpenFilter: filter.open()
        //customerModel: baseModel
    }

    states: [
        State {
            name: "userNotLoggedIn"
            when: user.userType === UserEntity.GUEST
            PropertyChanges {
                target: bills
                currentIndex: 0
            }
        },
        State {
            name: "userLoggedIn"
            when: user.userType === UserEntity.USER && CurrentUser.accessToken
            PropertyChanges {
                target: bills
                currentIndex: user.customerId ? 1 : 0
            }
        },
        State {
            name: "adminLoggedIn"
            when: CurrentUser.isAdmin && CurrentUser.accessToken !== ""
            PropertyChanges {
                target: bills
                currentIndex: 2
            }
            StateChangeScript {
                name: "populate"
                script: bills.gotoCustomersView();
            }
        }
    ]

    Bills {
        id: bills
        stackView: root.stackView
    }

    Filter {
        id: filter
        //customerModel: baseModel
        onFiltered: data => orderView.model = data
    }

    function updateCustomerModel() {
        if (TheCurrentUser.isAdmin)
            bills.findAll();
        else if (TheCurrentUser.isCostumer)
            bills.findOrdersByCustomerId(TheCurrentUser.customerId);
    }
}
