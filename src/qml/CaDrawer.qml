import QtQuick
import QtQuick.Controls

Drawer {
    id: root
    //default property alias content: page.children
    required property string title
    Button {
        anchors {
            top: parent.top
            topMargin: 5
            right: parent.right
            rightMargin: 5
        }

        background: Image {
            source: "qrc:/crossIcon"
            sourceSize: "32x32"
        }
        onClicked: close()
    }
}
