pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.Controls

Item {
    id: root

    property var sellerModel
    property int sellerId: -1
    readonly property alias selected: entitySelector.isSelectedView

    implicitWidth: entitySelector.implicitWidth
    implicitHeight: entitySelector.implicitHeight

    component EmptyDelegate : RedirectLabel {
        title: "Seleccionar vendedor asociado"
        onClicked: drawer.open()
    }

    component SelectedDelegate : ColumnLayout {
        Label {
            text: qsTr("Vendedor seleccionado")
            font.pointSize: 14
            Layout.fillWidth: true
        }

        EntityCard {
            id: selectedDelegate
            Layout.fillWidth: true
            onClicked: drawer.open()

            Connections {
                target: drawer

                function onSellerSelected(seller) {
                    selectedDelegate.fullname = seller.fullname;
                    selectedDelegate.entityId = seller.sellerId;
                    root.sellerId = seller.sellerId;
                }
            }
        }
    }

    EntitySelector {
        id: entitySelector
        anchors.fill: parent
        emptyDelegate: EmptyDelegate {}
        selectedDelegate: SelectedDelegate {}
    }

    Drawer {
        id: drawer

        signal sellerSelected(seller: var)

        width: ApplicationWindow.window.width
        height: ApplicationWindow.window.height
        anchors.centerIn: Overlay.overlay
        edge: Qt.BottomEdge
        interactive: false
        topPadding: 0

        ListView {
            id: listView
            anchors.fill: parent
            model: root.sellerModel
            spacing: 20
            header: DrawerHeader {
                width: drawer.width
                title: "Seleccionar vendedor asociado"
                onCloseClicked: drawer.close()
            }
            headerPositioning: ListView.OverlayHeader
            delegate: Item {
                required property string fullname
                required property int sellerId
                width: listView.width
                implicitHeight: entityCard.implicitHeight

                EntityCard {
                    id: entityCard
                    fullname: parent.fullname
                    width: parent.width * .9
                    anchors.horizontalCenter: parent.horizontalCenter
                    entityId: sellerId
                    onClicked: {
                        entitySelector.setSelectedView();
                        drawer.sellerSelected({fullname, sellerId});
                        drawer.close();
                    }
                }
            }
        }
    }
}
