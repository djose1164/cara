add_library(ReviewLib STATIC)
qt_add_qml_module(ReviewLib
    URI Cara.Review
    QML_FILES
        CommentMenuButton.qml
        CommentRating.qml
        CustomerReview.qml
        PostReview.qml
        ReviewApi.qml
        ReviewHeader.qml
        ReviewsPage.qml
    SOURCES
        restreviewrepository.h restreviewrepository.cpp
        review.h review.cpp
        reviewcontroller.h reviewcontroller.cpp
        reviewmodel.h reviewmodel.cpp
        reviewrepository.h
)

target_include_directories(ReviewLib PRIVATE ../authentication ../Utils ../)

target_link_libraries(ReviewLib PRIVATE NetworkLib)
