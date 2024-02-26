import QtQuick
import QtQuick.Controls
import Utils

Column {
    property alias reviewModel: reviews.model

    spacing: 10

    Label {
        text: "Reseñas"
        font.bold: true
        font.pointSize: 16
    }

    StarRating {
        id: starRating
        enabled: false
        selectedStars: root.rating
    }

    Reviews {
        id: reviews
        productId: root.productId
    }

    ReviewApi {
        id: reviewApi
        readonly property var emptyReview: {
            "customer_id": customerId,
            "product_id": productId
        }
        Component.onCompleted: fetchAllReviews(productId)
        onFailed: r => openDialogWithText(r.message);
        onAllReviewsFetched: r => {
            const ratings = r.map(item => item.rating);
            const model = reviewSection.reviewModel;
            setProductRating(ratings);
            model.append(r.filter(item => item.customer_id !== customerId));
            const res = r.find(item => item.customer_id === customerId);
            if (res)
                model.insert(0, res);
            else if (ratings?.length)
                model.insert(0, emptyReview);
        }
        function setProductRating(ratings) {
            const sum = ratings.reduce((total, num) => total + num, 0);
            root.rating = (Math.round(sum / ratings.length));
        }
    }
}
