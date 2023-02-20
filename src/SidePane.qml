import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/users"

Drawer {
    width: .7 * parent.width
    height: parent.height

    //TheCurrentUser.onComponentReady: addUserById(TheCurrentUser.userId)

    Component.onCompleted: addUserById(TheCurrentUser.userId)

    ListView {
        id: listView
        width: parent.width
        height: parent.height/2
        anchors.top: parent.top
        //Layout.fillWidth: true
        //Layout.alignment: Qt.AlignHCenter
        headerPositioning: ListView.OverlayHeader
        header: Component {
            Rectangle {
                z: 2
                width: parent.width
                height: 30
                Label {
                    text: "Informacion Personal"
                    font.pointSize: 12
                    font.bold: true
                }
            }
        }
        model: userModel
        delegate: Component {
            ColumnLayout {
                required property string email
                required property var person_info
                width: listView.width
                height: listView.height / 2

                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: "Usario: " + person_info.username
                    font.pointSize: 11
                }

                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: person_info.forename + " " + person_info.surname
                    font.pointSize: 11
                }

                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: email
                    font.pointSize: 11
                }
            }
        }
    }

    ListModel { id: userModel }

    BusyPopup { id: busyIndicator }

    Request {
        id: request
        base: "http://127.0.0.1:5000/api/users"
        onSent: busyIndicator.open()
        onSucceeded: r => {
                         print("SidePane: " + JSON.stringify(r));
                         busyIndicator.close();
                     }
        //requestData.sendMessage({"username": loginSettings.username, "model": userModel})
    }


    Button {
        anchors.top: listView.bottom
        anchors.topMargin: 20
        text: "Cerrar sesion"
        flat: true
        onClicked: {
            close();
            TheCurrentUser.clear();
            mainStackView.replace("qrc:/login/Login.qml");
        }
    }

    //ColumnLayout { anchors.fill: parent }

    function addUserById(userId) {

        print("addUserById: " + userId);
        request.get("/" + userId, null, r => {
                        print("addUserById: " + JSON.stringify(r));
                        userModel.append(r.user)});
    }
}
