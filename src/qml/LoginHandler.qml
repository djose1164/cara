import QtQuick
import QtQuick.Controls

Item {
    anchors.fill: parent
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: 0

        Login {
            id: login
        }

        SignUp {
            id: signUp
        }
    }
}
