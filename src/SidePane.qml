import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Drawer {
    width: .8 * parent.width
    height: parent.height

    ListView {
        id: listView
        anchors.fill: parent
        headerPositioning: ListView.OverlayHeader
        header: Component {
            Rectangle {
                z: 2
                width: parent.width
                height: 30
                Label {
                    text: "Informacion Personal"
                    font.pointSize: 18
                    font.bold: true
                }
            }
        }
        model: userModel
        delegate: Component {
            ColumnLayout {
                width: parent.width
                spacing: 20

                Label {
                    Layout.fillWidth: true
                    text: "Usario: " + username
                    font.pointSize: 14
                }

                Label {
                    Layout.fillWidth: true
                    text: "Email: " + email
                    font.pointSize: 14
                }

                Label {

                    Layout.fillWidth: true
                    text: "Nombre: " + forename
                    font.pointSize: 14
                }

                Label {

                    Layout.fillWidth: true
                    text: "Apellido: " + surname
                    font.pointSize: 14
                }

                Label {

                    Layout.fillWidth: true
                    text: "Telefono: " + telephone
                    font.pointSize: 14
                }
            }
        }
    }

    ListModel { id: userModel }

    BusyPopup {
        id: busyIndicator
    }

    /*WorkerScript {
        id: requestData
        source: "qrc:/js/login.mjs"
        onReadyChanged: {
            busyIndicator.open()
            requestData.sendMessage({"username": loginSettings.username, "model": userModel})

        }
        onMessage: busyIndicator.close()
    }*/
}
