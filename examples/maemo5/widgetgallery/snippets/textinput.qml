import Qt 4.7

Rectangle {
    id: page
    width: 300; height: 200

    TextInput {
        id: helloText
        text: "Edit me!"
        anchors.verticalCenter: page.verticalCenter
        anchors.horizontalCenter: page.horizontalCenter
    }
}
