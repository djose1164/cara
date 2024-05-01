import QtQuick
import QtQuick.Controls
import Cara.Utils

Item {
    id: root

    property string fullname
    property int customerId

    signal clicked

    implicitHeight: 75
    Card {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        source: "qrc:/qt/qml/Cara/Customer/person.svg"
        text: `<h3>${fullname}</h3><h5>ID #${customerId}</h5>`
        onClicked: root.clicked()
    }
}
