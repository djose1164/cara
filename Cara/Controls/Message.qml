import QtQuick.Dialogs

MessageDialog {
    onButtonClicked: close()
    function show(message: string) {
        text = message;
        open();
    }
}
