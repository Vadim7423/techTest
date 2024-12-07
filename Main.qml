import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import App

Rectangle {
  id: root

  height: 480
  width: 640

  RowLayout {
    height: parent.height
    width: parent.width

    Item {
      id: buttons

      Layout.preferredHeight: 100
      Layout.preferredWidth: 200

      ColumnLayout {
        height: parent.height
        spacing: 20
        width: parent.width

        Label {
          id: rate

          Layout.alignment: Qt.AlignCenter
          font.pointSize: 18
          text: MainController.rate
        }

        Button {
          Layout.alignment: Qt.AlignCenter
          text: "Добавить"

          onClicked: {
            countsModel.insertRows(0, 1);
          }
        }

        Button {
          Layout.alignment: Qt.AlignCenter
          text: "Удалить"

          onClicked: {
            if (items.currentIdx !== -1) {
              countsModel.removeRows(items.currentIdx, 1);
              items.currentIdx = -1;
            }
          }
        }

        Button {
          Layout.alignment: Qt.AlignCenter
          text: "Сохранить"

          onClicked: {
            countsModel.saveData();
          }
        }
      }
    }

    ListView {
      id: items

      property int currentIdx: -1

      Layout.preferredHeight: parent.height
      Layout.preferredWidth: 200
      model: countsModel

      delegate: Button {
        height: 50
        width: 200

        background: Rectangle {
          color: parent.activeFocus || items.currentIdx === index ? "#f5c158" : "#faf9f7"
        }

        onActiveFocusChanged: {
          if (activeFocus) {
            items.currentIdx = index;
          }
        }

        Text {
          anchors.centerIn: parent
          text: name ? name : ""
        }
      }
    }
  }
}
