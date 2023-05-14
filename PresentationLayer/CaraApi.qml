import QtQuick
import com.cara 1.0
import "qrc:/singleton"

Rest {
    property string endpoint: ""
    base: Constants.base + endpoint
}
