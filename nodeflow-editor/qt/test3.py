import sys
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *


def create_variable_icon(color):
    # Create a rounded version of the original pixmap

    pixmap = QPixmap(100,50)

    mask = QBitmap((pixmap.size()))
    mask.fill(Qt.color0)

    painter = QPainter(mask)
    painter.setRenderHint(QPainter.Antialiasing)
    painter.setBrush(Qt.color1)
    painter.drawRoundedRect( mask.rect(),30,30)
    painter.end()

    pixmap.fill(color)
    pixmap.setMask(mask)

    return QIcon(pixmap)



if __name__ == '__main__':
    # Create the application
    app = QApplication(sys.argv)
    app.setStyle("Fusion")

    table_widget = QTableWidget()
    table_widget.setRowCount(4)
    table_widget.setColumnCount(2)
    table_widget.setShowGrid(False)
    table_widget.horizontalHeader().hide()
    table_widget.verticalHeader().hide()
    table_widget.setSelectionBehavior(QAbstractItemView.SelectRows)

    data = [
        ["new_variable", "float"],
        ["new_variable_1", "int"],
    ]



    var_item = QTableWidgetItem("new_variable")
    type_item = QTableWidgetItem(create_variable_icon(Qt.red),"float")
    type_item.setFlags(Qt.ItemIsSelectable | Qt.ItemIsEnabled)
    table_widget.setItem(0, 0, var_item)
    table_widget.setItem(0, 1, type_item)

    table_widget.show()

    sys.exit(app.exec_())
