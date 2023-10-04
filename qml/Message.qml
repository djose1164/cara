import QtQuick.Dialogs

MessageDialog {
    onAccepted: close()
    function show(message: string) {
        text = message;
        open();
    }
}
