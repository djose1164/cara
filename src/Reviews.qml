import QtQuick

Repeater {
    id: root

    property bool showDefult
    property int productId

    delegate: reviewDelegate
    clip: true

    Review {
        id: _review
        anchors.fill: parent
        anchors.top: parent.top
        visible: root.count === 0
        productId: root.productId
    }

    Component {
        id: reviewDelegate
        Review {
            required property string review
            required property int id
            required property int rating
            required property string name
            required property string posted_date
            required property int customer_id
            required property int index

            Component.onCompleted: {
                setCommentText(review ? review : "");
            }
            width: root.width
            height: root.height*.35
            reviewText: review
            reviewRating: rating
            customerName: name
            reviewPostedDate: posted_date
            reviewId: id
            reviewCustomerId: customer_id
            productId: root.productId
        }
    }
}
