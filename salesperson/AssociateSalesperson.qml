import QtQuick
import QtQuick.Controls
import AssociateSalesperson
import "qrc:/utils"

Page {
    id: root

    readonly property StackView stackView: StackView.view

    title: "Vendedor Asociado"
    header: TitleHeader {
        title: root.title
        backBtn.visible: false
    }
    Component.onCompleted: service.fetchAllAssociateSalesperson()

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 15
        model: listModel
        delegate: salespersonDelegate
    }

    ListModel { id: listModel }

    RoundButton {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        text: "+"
        font.pointSize: 16
        font.bold: true
        onClicked: stackView.push("NewSalesperson.qml")
    }

    AssociateSalespersonService {
        id: service
        onFetched: {
            const salespersons = JSON.parse(data).associate_salespersons;
            listModel.append(salespersons);
            console.assert(listModel.count > 0 && listView.count > 0);
        }
    }

    Component {
        id: salespersonDelegate
        ItemDelegate {
            required property int id
            required property var person_info
            required property int credit_limit
            required property int credit_available
            required property int credit_consumed

            width: listView.width
            height: label.implicitHeight
            spacing: 5
            onClicked: stackView.push("delegate/AssociateSalespersonDelegate.qml", {
                                          salespersonId: id,
                                          personInfo: person_info,
                                          creditLimit: credit_limit,
                                          creditAvailable: credit_available,
                                          creditConsumed: credit_consumed
                                      })

            Label {
                id: label
                text: person_info.name + `\nID #${id}`
                padding: 12
                font.pointSize: 12
            }
        }
    }
}
