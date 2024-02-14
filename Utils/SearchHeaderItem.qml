import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property bool closeBtnVisible: true
    property alias title: titleLbl.text
    readonly property alias leadingBtn: leadingBtn
    readonly property alias searchText: searchField.text
    readonly property alias displayText: searchField.displayText

    signal closeClicked
    signal searchClosed
    signal textEdited

    states: [
        State {
            name: "searching"
            PropertyChanges {
                target: titleLbl
                visible: false
            }
            PropertyChanges {
                target: stackLayout
                currentIndex: 1
                Layout.fillWidth: true
            }
            PropertyChanges {
                target: leadingBtn
                icon.source: "qrc:/left_arrow"
            }
        },
        State {
            name: "disabled"
            PropertyChanges {
                target: searchBtn
                enabled: false
            }
        }
    ]

    implicitWidth: header.childrenRect.width
    implicitHeight: 50
    onCloseClicked: searchField.text = ''

    Rectangle {
        id: header
        anchors.fill: parent
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: leadingBtn
                visible: root.state === "searching" || closeBtnVisible
                icon.source: "qrc:/crossIcon"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeading
                onClicked: leadingBtnHandler()
            }

            Label {
                id: titleLbl
                horizontalAlignment: Label.AlignHCenter
                text: "Buscar"
                font.pointSize: 14
                font.bold: true
                Layout.fillWidth: true
            }

            StackLayout {
                id: stackLayout
                Layout.alignment: Qt.AlignVCenter | Qt.AlignTrailing
                Layout.rightMargin: 5
                Layout.maximumHeight: header.height * .8
                Layout.preferredWidth: searchBtn.implicitWidth * .8

                SearchButton {
                    id: searchBtn
                    onClicked: {
                        root.state = "searching";
                    }
                }

                Item {
                    SearchField {
                        id: searchField
                        width: parent.width
                        height: parent.height * .85
                        anchors.centerIn: parent
                        visible: !header.midBtnVisible
                        placeholderText: "Nombre"
                        onTextEdited: root.textEdited()
                    }
                }
            }
        }
    }

    function leadingBtnHandler() {
        if (root.state !== "searching") {
            root.closeClicked();
        } else {
            endSearch();
        }
    }

    function endSearch() {
        root.state = "";
        searchField.text = "";
        searchClosed();
    }
}
