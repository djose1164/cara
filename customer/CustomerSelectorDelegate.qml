import QtQuick
import QtQuick.Controls
import Utils
import Customer

ItemDelegate {
    property string fullname
    property int customerId

    icon.source: "person.svg"
    text: `<h3>${fullname}</h3><h5>${customerId}</h5>`
}
