import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("Test SQLite VIEW/MODEL")

    RowLayout {
        id: rowLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        spacing: 10

        Text {text: qsTr("Nome")}
        TextField {id: fnameField}
        Text {text: qsTr("Cognome")}
        TextField { id: snameField}
        Text {text: qsTr("Luogo Nascita")}
        TextField {id: luogonascitaField}

        Button {
            text: qsTr("Add")

            // Make a new entry in the database
            onClicked: {
                database.inserIntoTable(fnameField.text , snameField.text, luogonascitaField.text)
                myModel.updateModel() // And updates the data model with a new record
            }
        }
    }

    TableView {
        id: tableView
        anchors.top: rowLayout.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5

        TableViewColumn {
            role: "fname"
            title: "Nome"
        }
        TableViewColumn {
            role: "sname"
            title: "Cognome"
        }
        TableViewColumn {
            role: "luogonascita"
            title: "Luogo Nascita"
        }

        model: myModel

        // Setting lines in TableView to intercept mouse left click
        rowDelegate: Rectangle {
            anchors.fill: parent
            color: styleData.selected ? 'skyblue' : (styleData.alternate ? 'whitesmoke' : 'white');
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked: {
                    tableView.selection.clear()
                    tableView.selection.select(styleData.row)
                    tableView.currentRow = styleData.row
                    tableView.focus = true

                    switch(mouse.button) {
                    case Qt.RightButton:
                        contextMenu.popup() // Call the context menu
                        break
                    default:
                        break
                    }
                }
            }
        }
    }

    // The context menu offers deleting a row from the database
    Menu {
        id: contextMenu

        MenuItem {
            id: menu_remove
            text: qsTr("Remove")
            onTriggered: {
                /* Call the dialog box that will clarify the intention to remove the row from the database
                 * */
                dialogDelete.open()
            }
        }
        MenuItem {
            id: menu_remove_check
            text: qsTr("Remove Check transaction")
            onTriggered: {
                /* Call the dialog box that will clarify the intention to remove the row from the database
                 * */
                dialogDeleteCheck.open()
            }
        }

    }
    // Dialog of confirmation the removal line from the database
    MessageDialog {
        id: dialogDelete
        title: qsTr("Remove record")
        text: qsTr("Confirm the deletion of log entries")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel

        // If the answer ...
        onAccepted: {
            /* ... remove the line by id, which is taken from the data model
             * on the line number in the presentation
             * */
            database.removeRecord(myModel.getId(tableView.currentRow), 0)
            myModel.updateModel();
        }
    }
    // Dialog of confirmation the removal line from the database
    MessageDialog {
        id: dialogDeleteCheck
        title: qsTr("Remove record whith check")
        text: qsTr("Confirm the deletion of log entries")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel

        // If the answer ...
        onAccepted: {
            /* ... remove the line by id, which is taken from the data model
             * on the line number in the presentation
             * */
            database.removeRecord(myModel.getId(tableView.currentRow), 1)
            myModel.updateModel();
        }
    }


}
