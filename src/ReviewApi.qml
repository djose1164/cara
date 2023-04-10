import QtQuick
import "qrc:/utils"

CaraApi {
    id: api
    signal reviewPosted(resp: var)
    signal reviewFetched(rating: var)
    signal reviewDeleted

    endpoint: "/rating"

    function postReview(review: var) {
        const success = r => {
            reviewPosted(r);
            api.succeeded.disconnect(success);
        }
        api.succeeded.connect(success);

        api.post("/", review);
    }

    function fetchReview(customerId, productId) {
        const success = ({ rating }) => {
            reviewFetched(rating);
            api.succeeded.disconnect(success);
        }
        api.succeeded.connect(success);


        api.get(`/?customer_id=${customerId}&product_id=${productId}`);
    }

    function fetchReviewById(ratingId) {
        const success = ({ rating }) => {
            reviewFetched(rating);
            api.succeeded.disconnect(success);
        }
        api.succeeded.connect(success);


        api.get(`/${ratingId}`);
    }

    function deleteReview() {
        const success = () => {
            reviewDeleted();
            api.succeeded.disconnect(success);
        }
        api.succeeded.connect(success);

        api.delete_(`/${reviewId}`);
    }
}
