import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    property var mainHeader: MainHeader { id: mainHeader }
    readonly property var views: [homeStackView, searchStackView, orderStackView, billStackView]
    header: mainHeader
    StackLayout {
        anchors.fill: parent
        id: stackLayout

        StackView {
            id: homeStackView
            initialItem: "qrc:/home/Home.qml"
        }

        StackView {
            id: searchStackView
            initialItem: "qrc:/search/SearchPage.qml"
        }

        StackView {
            id: orderStackView
            initialItem: "qrc:/orders/AddOrderPage.qml"
        }

        StackView {
            id: billStackView
            initialItem: "qrc:/bills/BillPage.qml"
            onDepthChanged: {
                if (depth > 1) {
                    print("## depth: " + depth);
                    for (let i = 0; i < depth; ++i)
                        if (get(i).pageHeader)
                            root.header = get(i).pageHeader;
                }
            }
        }

        onCurrentIndexChanged: {
            const current = stackLayout.itemAt(currentIndex);
            root.header = current.get(0).pageHeader;
        }
    }

    footer: MainFooter {}

    SidePane {id: sideMenu}
}
