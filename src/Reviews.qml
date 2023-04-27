import QtQuick
import QtQuick.Layouts

Repeater {
    id: root

    property int productId

    width: 200
    height: 200
    model: ListModel { id: reviewsModel }
    delegate: ReviewDelegate { productId: root.productId }
}
