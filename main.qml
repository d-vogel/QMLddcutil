import QtQuick 2.7
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.3
ApplicationWindow
{
    width:300
    height:200
    visible: true

    TabView {
        objectName: "tabView"
        anchors.fill: parent
        frameVisible: false
        tabsVisible: false
        currentIndex: tabViewIndex

        Tab {
            ColumnLayout {
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Scanning for displays."
                }
                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                }
            }
        }

        Tab {
            ListView{
                anchors.fill: parent


                model: displayListItemModel
                delegate: Component{
                    ColumnLayout{
                        height: 80
                        anchors.left:parent.left
                        anchors.right:parent.right
                        RowLayout{
                            Layout.fillWidth: true
                            Text{
                                text:monitorName
                                width:implicitWidth
        //                        anchors.left:parent.left
                            }
                            Rectangle{
                                Layout.fillWidth: true
                            }

                            ComboBox{
                                width:implicitWidth
                                onCurrentIndexChanged: whitepointSelected=currentText
                                //anchors.right: parent.right
                                model:whitepointValues
                            }
                        }
                        Slider{
                            Layout.fillWidth: true
                            width:parent.width
                            value:brightness
                            onValueChanged: brightness=value
                            minimumValue: minBrightness
                            maximumValue: maxBrightness
                            stepSize: stepBrightness
                            tickmarksEnabled: true
                            updateValueWhileDragging: false

                        }
                    }
                }
            }
        }
    }
}
