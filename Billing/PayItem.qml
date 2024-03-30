import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Utils
import Billing

// TODO: Make two views: one for just seeing & the another for editing.
Item {
    id: root

    enum StateView {
        LoadingView,
        CreditedView,
        SuccessView
    }

    required property payment payment_
    property alias stateView: stackLayout.currentIndex

    signal refresh(newPayment: payment)

    states: [
        State {
            name: "unpaid"
            PropertyChanges {
                target: stackLayout
                currentIndex: PayItem.UnpaidView
            }
        },
        State {
            name: "credited"
            PropertyChanges {
                target: stackLayout
                currentIndex: PayItem.CreditedView
            }
        },
        State {
            name: "busy"
            PropertyChanges {
                target: stackLayout
                currentIndex: PayItem.LoadingView
            }
        },
        State {
            name: "success"
            PropertyChanges {
                target: stackLayout
                currentIndex: PayItem.SuccessView
            }
        }

    ]

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        // LoadingView
        BusyIndicator {}

        // CreditedView
        CreditedView {
            payment_: root.payment_
            onCreditClicked: stackLayout.currentIndex = PayItem.LoadingView
            onSuccess: newPayment => {
                stackLayout.currentIndex = PayItem.SuccessView;
                root.refresh(newPayment);
            }
        }

        PaySuccess {
            Layout.alignment: Qt.AlignCenter
        }
    }

    function resetView() {
        stackLayout.currentIndex = PayItem.CreditedView;
    }
}
