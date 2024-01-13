import QtQuick
import QtQuick.Layouts

Column {
    id: root
    readonly property alias model: repeater.model
    property int productId

    width: parent.width

    Repeater {
        id: repeater
        model: ListModel { id: reviewsModel }
        delegate: ReviewDelegate { productId: root.productId }
    }
}
