pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root

    property double total
    readonly property double commissionRate: spinBox.value / 100

    title: "Ganancias"
    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        
        RowLayout {
            spacing: 20
            Layout.fillWidth:true
            
            SpinBox {
                id: spinBox
                from: 1
                to: 100
                value: 5
                stepSize: 5
                editable: true
                validator: IntValidator {}
            }
            
            Label {
                text: "% de ganancia del vendedor"
                font.pointSize: 12
                wrapMode: Label.Wrap
                Layout.fillWidth: true
            }
        }
        
        Label {
            text: "Total de Ganancia Neto - % de ganancia: <b>$RD%1</b>"
            .arg(root.total.toFixed(2))
            font.pointSize: 12
            wrapMode: Label.Wrap
            Layout.fillWidth: true
        }
    }
}
