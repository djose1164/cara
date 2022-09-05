import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Page {
    id: root
    header: PageHeader { title: "Actualizar Ordenes" }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        //BusyIndicator { anchors.fill: parent }

        TextField {
            id: clientField
            placeholderText: "Nombre de cliente"
        }

        GridView {
            id: gridView
            delegate: UpdateOrderDelegate {}
            model:  clientList
        }
    }



}
