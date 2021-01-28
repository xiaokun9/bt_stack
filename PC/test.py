import sys
import time

import serial.tools.list_ports
import serial
from PyQt5.QtCore import QThread
from PyQt5.QtWidgets import QApplication, QMainWindow, QComboBox, QMessageBox
from pc import *
import datetime
import struct
from enum import Enum

ser = serial.Serial()
fd = None

class Hci_Filed(Enum):
  HCI_CMD_HOST_TO_CONTROL = 0x02
  HCI_CMD_CONTROL_TO_HOST = 0x03
  HCI_ACL_HOST_TO_CONTROL = 0x00
  HCI_ACL_CONTROL_TO_HOST = 0x01
  HCI_SCO_HOST_TO_CONTROL = 0x00
  HCI_SCO_CONTROL_TO_HOST = 0x01
class MyThread(QThread):  # 线程1
  def __init__(self):
    super().__init__()

  def run(self):
    global fd
    while(1):
      arr = ser.read_until(expected = b'\xFF\xFF')
      print(len(arr))
      print("wwwwwwwww")
      print(arr)
      if arr[0] == 0x04:
        continue
        fd.write(struct.pack(">I", len(arr) - 2))
        fd.write(struct.pack(">I", len(arr) - 2))
        fd.write(struct.pack(">I", Hci_Filed.HCI_CMD_CONTROL_TO_HOST.value))
      elif arr[0] == 0x01:
        continue
        fd.write(struct.pack(">I", len(arr) - 2))
        fd.write(struct.pack(">I", len(arr) - 2))
        fd.write(struct.pack(">I", Hci_Filed.HCI_CMD_HOST_TO_CONTROL.value))
      else:
        pass
      if fd == None:
        continue
      #print(len(arr),len(arr),"\x00000001","\x00000000",int(round(time.time() * 1000000)))
      #fd.write(struct.pack(">I",len(arr)-2))
      #fd.write(struct.pack(">I", len(arr) - 2))
      #
      fd.write(struct.pack(">I", 0))
      fd.write(struct.pack(">Q", round(time.time() * 1000000)))
      fd.write(arr[:-2])




class Pyqt5_Serial(QMainWindow, Ui_MainWindow):
  def __init__(self):
    super(Pyqt5_Serial,self).__init__()
    self.setupUi(self)
    self.init()
    self.comboBox_2.setCurrentIndex(13)
    self.update_serial()
    #self.ser = serial.Serial()
    #self.ser.read_until()

  def init(self):
    self.comboBox.clicked.connect(self.update_serial)
    self.pushButton_3.clicked.connect(self.click_ev)
    self.write_thread = MyThread()
    self.checkBox_4.stateChanged.connect(self.openbtsnoop)
  def openbtsnoop(self,enable):
    global fd
    _time = time.strftime("%Y_%m_%d_%H_%M_%S.log", time.localtime())
    print(_time)
    if (enable == 2) :
      fd= open(_time,"wb+")
      fd.write(b"\x62\x74\x73\x6e\x6f\x6f\x70\x00\x00\x00\x00\x01\x00\x00\x03\xea") #btsnoop
    else:
      fd.close()
    print(enable)

  def click_ev(self,enable):
    if enable == True:
      ser.port = self.comboBox.currentText()
      ser.baudrate = int(self.comboBox_2.currentText())
      ser.stopbits = int(self.comboBox_3.currentText())
      ser.bytesize = int(self.comboBox_4.currentText())
      ser.parity = self.comboBox_5.currentText()
      try:
        ser.open()
      except:
        QMessageBox.critical(self,"error","串口打开失败")
        return None
      self.pushButton_3.setText(r"关闭串口")
      self.pushButton_3.setStyleSheet(
        "color:red;"
      )
      self.write_thread.start()
    else:
      self.write_thread.terminate()
      try:
        ser.close()
      except:
        pass
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
  #https://blog.csdn.net/windowsyun/article/details/80001488