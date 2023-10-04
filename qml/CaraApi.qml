import QtQuick
import data
import core

Rest {
    property string endpoint: ""
    base: Constants.base + endpoint
}
