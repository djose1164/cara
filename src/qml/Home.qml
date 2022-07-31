import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// TODO: Display user info and later make it a page. Or start it right now in
// another qml.

Page {
    id: home
    property string currentUser

    header: Label {
        id: homeLbl
        text: "Inicio"
        font.pointSize: 30
        font.bold: true
    }


    ListView {
        id: listView
        anchors.fill: parent
        model: listModel
        delegate: Component {
            ColumnLayout {
                spacing: 20
                Label {
                    Layout.fillWidth: true
                    text: "username: " + username
                    font.pointSize: 30
                }

                Label {
                    Layout.fillWidth: true
                    text: "email: " + email
                    font.pointSize: 30
                }

                Label {

                    Layout.fillWidth: true
                    text: "forename: " + forename
                    font.pointSize: 30
                }

                Label {

                    Layout.fillWidth: true
                    text: "surname: " + surname
                    font.pointSize: 30
                }

                Label {

                    Layout.fillWidth: true
                    text: "telephone: " + telephone
                    font.pointSize: 30
                }
            }
        }
    }

    ListModel {
        id: listModel
    }

    footer: ToolBar {
        //height: parent.height
        //anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: "blue"
        }
    }

    BusyPopup {
        id: busyIndicator
    }

    Worker {
        id: requestData
        onReadyChanged: {
                busyIndicator.open()
                requestData.sendMessage({"type": "userInfo", "user": currentUser, "model": listModel})

        }
        onMessage: {
            busyIndicator.close();
            print("Hoalala")
        }
    }
}
