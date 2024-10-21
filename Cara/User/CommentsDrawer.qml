pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.Controls
import Cara.User
import Cara.Review

Drawer {
    id: root

    required property int userId
    required property int orderId

    implicitWidth: 100
    implicitHeight: 128

    Rectangle {
        id: topLine
        width: 30
        height: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 0
        color: "black"
        radius: width
    }

    ListView {
        id: listView
        width: root.width
        anchors.top: topLine.bottom
        anchors.bottom: parent.bottom
        clip: true
        model: CommentModel {
            id: commentModel
            repository: OrderCommentRepository { id: commentRepository }
            onErrorOccurred: errorString => {
                console.log("Error occurred on CommentModel")
                message.openWith("Error", errorString);
            }
        }
        delegate: CommentDelegate {}
        header: listViewHeader
        headerPositioning: ListView.OverlayHeader
        footer: listViewFooter
        footerPositioning: ListView.OverlayFooter

        BusyIndicator {
            anchors.centerIn: parent
            running: listView.count === 0 && commentModel.status === CommentModel.Busy
        }

        Label {
            width: listView.width * .8
            anchors.centerIn: parent
            visible: listView.count === 0 && commentModel.status !== CommentModel.Busy
            text: commentModel.status === CommentModel.Error ? "Ha ocurrido un error: "+commentModel.errorString
                                                             : "Esta orden no cuenta con ningún comentario para mostrar.";

            wrapMode: Label.Wrap
            font.pointSize: 14
            horizontalAlignment: Label.AlignHCenter
        }
    }

    component CommentDelegate: Item {
        required property var model

        width: listView.width
        height: Math.max(80, pane.implicitHeight*1.22)

        Pane {
            id: pane
            anchors.fill: parent
            anchors.margins: 5
            font.pointSize: 12
            Material.elevation: 6

            ColumnLayout {
                anchors.fill: parent

                ReviewHeader {
                    Layout.preferredWidth: pane.width * .99
                    customerNameTxt: model.author
                    dateTxt: model.createdAt.toLocaleString(Qt.locale(), Locale.NarrowFormat)
                }

                Label {
                    text: model.content
                    wrapMode: Label.Wrap
                    Layout.fillWidth: true
                }
            }
        }
    }

    Component {
        id: listViewFooter
        Rectangle {
            id: rectangle
            width: root.width
            implicitHeight: rowLayout.implicitHeight * 1.33
            color: "#F5F5F5"
            z: 2

            RowLayout {
                id: rowLayout
                anchors.fill: parent
                anchors.margins: 5

                TextField {
                    id: textField
                    placeholderText: "Escriba su comentario..."
                    Layout.fillWidth: true
                }

                Button {
                    flat: true
                    enabled: textField.length > 0
                    icon.source: "qrc:/icons/send.svg"
                    onClicked: root.handleSendClicked(rectangle.getComment())
                }
            }

            Connections {
                target: commentModel

                function onCommentAdded() {
                    textField.clear();
                }
            }

            function getComment() : var {
                return {
                    userId: root.userId,
                    content: textField.text,
                    orderId: root.orderId
                };
            }
        }
    }

    Component {
        id: listViewHeader
        Rectangle {
            id: rectangle
            width: listView.width
            implicitHeight: label.implicitHeight + separator.implicitHeight
            z: 2

            Column {
                CloseHeader {
                    id: label
                    width: listView.width
                    title: "Comentarios"
                    onCloseClicked: root.close()
                }
                ToolSeparator {
                    id: separator
                    width: listView.width
                    orientation: Qt.Horizontal
                }
            }
        }
    }

    MessageDialog {
        id: message

        function openWith(title, msg) {
            message.title = title;
            message.text = msg;
            message.open();
        }
    }

    function handleSendClicked(newComment: var) {
        if (root.userId < 1) {
            const msg = "Invalid userId: "+root.userId;
            message.openWith("Error", msg);
            console.error(msg);
        }

        if (root.orderId < 1) {
            const msg = "Invalid orderId: "+root.orderId;
            message.openWith("Error", msg);
            console.error(msg);
        }

        commentModel.addComment(newComment);
    }

    function fetchCommentsByOrderId(orderId: int) {
        if (orderId < 1)
            throw "Invalid orderId: " + orderId;
        commentModel.fetchCommentsByOrderId(orderId);

    }
}
