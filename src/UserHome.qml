import QtQuick
import QtQuick.Controls
import "qrc:/users"

Home {
    id: root

    readonly property string fullName: TheCurrentUser.forename + " "
                                       + TheCurrentUser.surname

    Label {
        id: lbl
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        text: `¡Bienvenido ${fullName}, mira lo que tenemos disponible para ti!`
        font.pointSize: 13
        wrapMode: Text.WordWrap
    }

    Image {
        source: "https://doc-0k-7c-docs.googleusercontent.com/docs/securesc/erln2delfvfgligmo6vqfnid9vffloqo/cjnrbhoqdkkramht8mscih0po4uv4dsf/1677280200000/08630701631404921892/08930711371427038035Z/1_YYrWVLH7rFmzGC4ZLmHDEwOdrdiywqg?e=download&uuid=0a86f6e9-ad74-41a4-9315-b9508c8d329b&nonce=79jpb2nr49nd2&user=08930711371427038035Z&hash=bvcrqtj54lmbe1boq3hirckp0gdnddhu"
        asynchronous: true
    }
}
