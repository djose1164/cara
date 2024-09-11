import QtQuick
import QtQuick.Controls
import Cara.Controls
import Cara.Review

BackPage {
    id: root

    required property int productId

    title: "Reseñas y calificaciones"

    states: [
        State {
            name: "empty"
            when: listView.count === 0 && reviewModel.status === ReviewModel.Fetched
            PropertyChanges {
                target: msgLabel
                visible: true
            }
        },
        State {
            name: "fetching"
            when: reviewModel.status === ReviewModel.Fetching
            PropertyChanges {
                target: busyIndicator
                running: true
            }
        }
    ]

    Component.onCompleted: {
        root.state = "fetching";
        reviewModel.fetchReviewsByProductId(root.productId);
    }

    Label {
        id: msgLabel
        width: root.width * .9
        anchors.centerIn: parent
        horizontalAlignment: Label.AlignHCenter
        visible: false
        text: "No hay ninguna reseña para mostrar."
        wrapMode: Label.Wrap
        font.pointSize: 14
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 20
        model: reviewModel
        delegate: CustomerReview {
            required reviewText
            required reviewer
            required rating
            required createdAt
            required customerId
            required reviewId
            required property int index

            width: listView.width
            canDelete: true
            onReviewDeleted: reviewModel.remove(index)
        }
    }

    ReviewModel {
        id: reviewModel
    }
}
