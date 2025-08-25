import QtQuick

Card {
    id: root

    property string fullname
    property int entityId

    //implicitHeight: 75
    source: "qrc:/qt/qml/Cara/Customer/person.svg"
    text: `<h3>${root.fullname}</h3><h5>ID #${root.entityId}</h5>`
}
