import sys
import serial.tools.list_ports
import serial
from PyQt5.QtWidgets import QApplication, QMainWindow, QComboBox
from pc import *

class Pyqt5_Serial(QMainWindow, Ui_MainWindow):
  def __init__(self):
    super(Pyqt5_Serial,self).__init__()
    self.setupUi(self)
    self.init()
  def init(self):
    self.pushButton_3.clicked.connect(self.port_check)
  def port_check(self):
    print("asdasd\r\n")

if __name__ == '__main__':
  myapp = QApplication(sys.argv)
  myDlg = Pyqt5_Serial()
  myDlg.show()
  sys.exit(myapp.exec_())