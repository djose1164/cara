pragma Singleton
import QtQuick
import models

QtObject {
    id: root

    readonly property BillModel model: BillModel {}
}
