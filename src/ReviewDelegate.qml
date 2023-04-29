import QtQuick

Review {
    required property string review
    required property int id
    required property int rating
    required property string name
    required property string posted_date
    required property int customer_id
    required property int index

    Component.onCompleted: setCommentText(review ? review : "");

    reviewText: review
    reviewRating: rating
    customerName: name
    reviewPostedDate: posted_date
    reviewId: id
    reviewCustomerId: customer_id
}
