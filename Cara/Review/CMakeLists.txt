add_library(ReviewLib STATIC)
qt_add_qml_module(ReviewLib
    URI Cara.Review
    QML_FILES
        CommentMenuButton.qml
        CommentRating.qml
        CustomerReview.qml
        ReviewApi.qml
        ReviewHeader.qml
        PostReview.qml
    SOURCES
        restreviewrepository.h restreviewrepository.cpp
        review.h review.cpp
        reviewcontroller.h reviewcontroller.cpp
        reviewmodel.h reviewmodel.cpp
        reviewrepository.h
)

target_include_directories(ReviewLib PRIVATE ../utils)
