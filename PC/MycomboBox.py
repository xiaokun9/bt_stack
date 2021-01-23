from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QComboBox


class MycomboBox(QComboBox):
    clicked = pyqtSignal()
    def showPopup(self):
        self.clicked.emit()
        super(MycomboBox,self).showPopup()
