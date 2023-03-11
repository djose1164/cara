import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    id: root
    required property string name
    required property int sell_price
    required property string image_url
    required property string description
    width: ListView.view.width * .8
    height: ListView.view.height

     StackLayout {
        id: stackLayout
        anchors.centerIn: parent
        BusyIndicator {}

    ColumnLayout {
        spacing: 10
        Image {
            Layout.alignment: Qt.AlignHCenter
            source: image_url
            sourceSize: `${root.height-50}x${root.height-50}`
            onProgressChanged: if (progress === 1.0) stackLayout.currentIndex = 1
        }

        Label {
            text: name
            font.pointSize: 16
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
        }
    }
    }
}
