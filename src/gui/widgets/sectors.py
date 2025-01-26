from PyQt6 import QtCore, QtGui, QtWidgets


class Sectors:
    def __init__(self, main_window):
        self.main_window = main_window
        self.setup_sectors()

    def setup_sectors(self):

        self.widget = QtWidgets.QWidget(parent=self.main_window)
        self.widget.setGeometry(QtCore.QRect(0, 40, 1200, 70))
        self.widget.setObjectName("widget")

        self.pushButton = QtWidgets.QPushButton(parent=self.widget)
        self.pushButton.setGeometry(QtCore.QRect(0, 0, 400, 70))
        font = QtGui.QFont()
        font.setPointSize(28)
        self.pushButton.setFont(font)
        self.pushButton.setObjectName("pushButton")

        self.pushButton_2 = QtWidgets.QPushButton(parent=self.widget)
        self.pushButton_2.setGeometry(QtCore.QRect(400, 0, 400, 70))
        font = QtGui.QFont()
        font.setPointSize(28)
        self.pushButton_2.setFont(font)
        self.pushButton_2.setObjectName("pushButton_2")

        self.pushButton_3 = QtWidgets.QPushButton(parent=self.widget)
        self.pushButton_3.setGeometry(QtCore.QRect(800, 0, 400, 70))
        font = QtGui.QFont()
        font.setPointSize(28)
        self.pushButton_3.setFont(font)
        self.pushButton_3.setObjectName("pushButton_3")

        self.main_window.setSectors(self.widget)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        self.pushButton.setText(_translate("Form", "Boot Sector"))
        self.pushButton_2.setText(_translate("Form", "MFT"))
        self.pushButton_3.setText(_translate("Form", "Data Area"))
