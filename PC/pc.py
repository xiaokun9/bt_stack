# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'pc.ui'
#
# Created by: PyQt5 UI code generator 5.14.2
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets

from MycomboBox import MycomboBox


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(471, 444)
        MainWindow.setMinimumSize(QtCore.QSize(471, 444))
        MainWindow.setMaximumSize(QtCore.QSize(471, 444))
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.splitter_3 = QtWidgets.QSplitter(self.centralwidget)
        self.splitter_3.setOrientation(QtCore.Qt.Vertical)
        self.splitter_3.setObjectName("splitter_3")
        self.splitter_2 = QtWidgets.QSplitter(self.splitter_3)
        self.splitter_2.setOrientation(QtCore.Qt.Horizontal)
        self.splitter_2.setObjectName("splitter_2")
        self.listView = QtWidgets.QListView(self.splitter_2)
        self.listView.setObjectName("listView")
        self.splitter = QtWidgets.QSplitter(self.splitter_2)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")
        self.label = QtWidgets.QLabel(self.splitter)
        self.label.setObjectName("label")
        self.comboBox = MycomboBox(self.splitter)
        self.comboBox.setObjectName("comboBox")
        self.layoutWidget = QtWidgets.QWidget(self.splitter)
        self.layoutWidget.setObjectName("layoutWidget")
        self.gridLayout = QtWidgets.QGridLayout(self.layoutWidget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.label_2 = QtWidgets.QLabel(self.layoutWidget)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 0, 0, 1, 1)
        self.comboBox_2 = MycomboBox(self.layoutWidget)
        self.comboBox_2.setObjectName("comboBox_2")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.gridLayout.addWidget(self.comboBox_2, 0, 1, 1, 2)
        self.label_3 = QtWidgets.QLabel(self.layoutWidget)
        self.label_3.setObjectName("label_3")
        self.gridLayout.addWidget(self.label_3, 1, 0, 1, 1)
        self.comboBox_3 = MycomboBox(self.layoutWidget)
        self.comboBox_3.setSizeAdjustPolicy(QtWidgets.QComboBox.AdjustToContentsOnFirstShow)
        self.comboBox_3.setObjectName("comboBox_3")
        self.comboBox_3.addItem("")
        self.comboBox_3.addItem("")
        self.comboBox_3.addItem("")
        self.gridLayout.addWidget(self.comboBox_3, 1, 1, 1, 2)
        self.label_4 = QtWidgets.QLabel(self.layoutWidget)
        self.label_4.setObjectName("label_4")
        self.gridLayout.addWidget(self.label_4, 2, 0, 1, 1)
        self.comboBox_4 = MycomboBox(self.layoutWidget)
        self.comboBox_4.setObjectName("comboBox_4")
        self.comboBox_4.addItem("")
        self.comboBox_4.addItem("")
        self.comboBox_4.addItem("")
        self.comboBox_4.addItem("")
        self.gridLayout.addWidget(self.comboBox_4, 2, 1, 1, 2)
        self.label_5 = QtWidgets.QLabel(self.layoutWidget)
        self.label_5.setObjectName("label_5")
        self.gridLayout.addWidget(self.label_5, 3, 0, 1, 1)
        self.comboBox_5 = MycomboBox(self.layoutWidget)
        self.comboBox_5.setObjectName("comboBox_5")
        self.comboBox_5.addItem("")
        self.comboBox_5.addItem("")
        self.comboBox_5.addItem("")
        self.gridLayout.addWidget(self.comboBox_5, 3, 1, 1, 2)
        self.label_6 = QtWidgets.QLabel(self.layoutWidget)
        self.label_6.setObjectName("label_6")
        self.gridLayout.addWidget(self.label_6, 4, 0, 1, 1)
        self.pushButton_3 = QtWidgets.QPushButton(self.layoutWidget)
        self.pushButton_3.setCheckable(True)
        self.pushButton_3.setObjectName("pushButton_3")
        self.gridLayout.addWidget(self.pushButton_3, 4, 1, 1, 2)
        self.checkBox = QtWidgets.QCheckBox(self.layoutWidget)
        self.checkBox.setObjectName("checkBox")
        self.gridLayout.addWidget(self.checkBox, 5, 0, 1, 2)
        self.checkBox_2 = QtWidgets.QCheckBox(self.layoutWidget)
        self.checkBox_2.setObjectName("checkBox_2")
        self.gridLayout.addWidget(self.checkBox_2, 5, 2, 1, 1)
        self.checkBox_3 = QtWidgets.QCheckBox(self.layoutWidget)
        self.checkBox_3.setObjectName("checkBox_3")
        self.gridLayout.addWidget(self.checkBox_3, 6, 0, 1, 1)
        self.checkBox_4 = QtWidgets.QCheckBox(self.layoutWidget)
        self.checkBox_4.setObjectName("checkBox_4")
        self.gridLayout.addWidget(self.checkBox_4, 6, 2, 1, 1)
        self.checkBox_5 = QtWidgets.QCheckBox(self.layoutWidget)
        self.checkBox_5.setObjectName("checkBox_5")
        self.gridLayout.addWidget(self.checkBox_5, 7, 0, 1, 2)
        self.splitter_4 = QtWidgets.QSplitter(self.layoutWidget)
        self.splitter_4.setOrientation(QtCore.Qt.Horizontal)
        self.splitter_4.setObjectName("splitter_4")
        self.pushButton = QtWidgets.QPushButton(self.splitter_4)
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtWidgets.QPushButton(self.splitter_4)
        self.pushButton_2.setObjectName("pushButton_2")
        self.gridLayout.addWidget(self.splitter_4, 8, 0, 1, 3)
        self.lineEdit = QtWidgets.QLineEdit(self.splitter_3)
        self.lineEdit.setObjectName("lineEdit")
        self.verticalLayout.addWidget(self.splitter_3)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 471, 23))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "蓝牙工具"))
        self.label.setText(_translate("MainWindow", "串口选择："))
        self.label_2.setText(_translate("MainWindow", "波特率"))
        self.comboBox_2.setCurrentText(_translate("MainWindow", "110"))
        self.comboBox_2.setItemText(0, _translate("MainWindow", "110"))
        self.comboBox_2.setItemText(1, _translate("MainWindow", "300"))
        self.comboBox_2.setItemText(2, _translate("MainWindow", "600"))
        self.comboBox_2.setItemText(3, _translate("MainWindow", "1200"))
        self.comboBox_2.setItemText(4, _translate("MainWindow", "2400"))
        self.comboBox_2.setItemText(5, _translate("MainWindow", "4800"))
        self.comboBox_2.setItemText(6, _translate("MainWindow", "9600"))
        self.comboBox_2.setItemText(7, _translate("MainWindow", "14400"))
        self.comboBox_2.setItemText(8, _translate("MainWindow", "19200"))
        self.comboBox_2.setItemText(9, _translate("MainWindow", "38400"))
        self.comboBox_2.setItemText(10, _translate("MainWindow", "43000"))
        self.comboBox_2.setItemText(11, _translate("MainWindow", "57600"))
        self.comboBox_2.setItemText(12, _translate("MainWindow", "76800"))
        self.comboBox_2.setItemText(13, _translate("MainWindow", "115200"))
        self.comboBox_2.setItemText(14, _translate("MainWindow", "128000"))
        self.comboBox_2.setItemText(15, _translate("MainWindow", "230400"))
        self.comboBox_2.setItemText(16, _translate("MainWindow", "256000"))
        self.comboBox_2.setItemText(17, _translate("MainWindow", "460800"))
        self.comboBox_2.setItemText(18, _translate("MainWindow", "921600"))
        self.comboBox_2.setItemText(19, _translate("MainWindow", "1000000"))
        self.comboBox_2.setItemText(20, _translate("MainWindow", "2000000"))
        self.comboBox_2.setItemText(21, _translate("MainWindow", "3000000"))
        self.label_3.setText(_translate("MainWindow", "停止位"))
        self.comboBox_3.setItemText(0, _translate("MainWindow", "1"))
        self.comboBox_3.setItemText(1, _translate("MainWindow", "1.5"))
        self.comboBox_3.setItemText(2, _translate("MainWindow", "2"))
        self.label_4.setText(_translate("MainWindow", "数据位"))
        self.comboBox_4.setItemText(0, _translate("MainWindow", "8"))
        self.comboBox_4.setItemText(1, _translate("MainWindow", "7"))
        self.comboBox_4.setItemText(2, _translate("MainWindow", "6"))
        self.comboBox_4.setItemText(3, _translate("MainWindow", "5"))
        self.label_5.setText(_translate("MainWindow", "校验位"))
        self.comboBox_5.setItemText(0, _translate("MainWindow", "N"))
        self.comboBox_5.setItemText(1, _translate("MainWindow", "Old"))
        self.comboBox_5.setItemText(2, _translate("MainWindow", "Even"))
        self.label_6.setText(_translate("MainWindow", "串口操作"))
        self.pushButton_3.setText(_translate("MainWindow", "打开串口"))
        self.checkBox.setText(_translate("MainWindow", "16进制显示"))
        self.checkBox_2.setText(_translate("MainWindow", "DRT"))
        self.checkBox_3.setText(_translate("MainWindow", "RTS"))
        self.checkBox_4.setText(_translate("MainWindow", "保存为snoop"))
        self.checkBox_5.setText(_translate("MainWindow", "16进制发送"))
        self.pushButton.setText(_translate("MainWindow", "发送数据"))
        self.pushButton_2.setText(_translate("MainWindow", "PushButton"))
