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
    self.comboBox_2.setCurrentIndex(13)
    self.update_serial()
  def init(self):
    self.comboBox.clicked.connect(self.update_serial)
    self.pushButton_3.clicked.connect(self.click_ev)
  def click_ev(self,enable):
    print(enable)
    if enable == True:
      self.pushButton_3.setText(r"关闭串口")
      self.pushButton_3.setStyleSheet(
        "color:red;"
      )
    else:
      self.pushButton_3.setText(r"打开串口")
      self.pushButton_3.setStyleSheet(
        "color:black;"
      )
  def update_serial(self): # update serial
    self.Com_Dict = {}
    port_list = list(serial.tools.list_ports.comports())
    self.comboBox.clear()
    for port in port_list:
      self.Com_Dict["%s" % port[0]] = "%s" % port[1]
      self.comboBox.addItem(port[0])
    if len(self.Com_Dict) == 0:
      self.comboBox.clear()

if __name__ == '__main__':
  myapp = QApplication(sys.argv)
  myDlg = Pyqt5_Serial()
  myDlg.show()
  sys.exit(myapp.exec_())