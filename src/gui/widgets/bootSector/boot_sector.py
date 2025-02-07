from PyQt6.QtWidgets import (QWidget, QVBoxLayout, QGridLayout, QLabel,
    QSizePolicy, QFrame, QPushButton, QHBoxLayout, QTextEdit)
from PyQt6.QtGui import QFont 
import bindings

class BootSectorTab(QWidget):
    def __init__(self):
        super().__init__()
        self.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        self.setup_ui()

    def setup_ui(self):
        container_layout = QHBoxLayout(self)
        container_layout.setContentsMargins(20, 20, 50, 100)
        container_layout.setSpacing(100)  

        table_widget = QWidget()
        main_layout = QVBoxLayout(table_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)

        self.frame = QFrame() 
        self.frame.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        self.frame.setObjectName("BodyFrame")

        grid_layout = QGridLayout(self.frame)
        grid_layout.setSpacing(0)
        grid_layout.setContentsMargins(0, 0, 0, 0)
        self.grid_layout = grid_layout

        headers = ["Description", "Value", "Bytes"]
        for col, header_text in enumerate(headers):
            header = QLabel(header_text)
            header.setObjectName("HeaderLabel")
            header.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
            grid_layout.addWidget(header, 0, col)

        grid_layout.setColumnStretch(0, 2)
        grid_layout.setColumnStretch(1, 2)
        grid_layout.setColumnStretch(2, 1)

        main_layout.addWidget(self.frame)

        button_layout = QVBoxLayout()
        button_layout.setContentsMargins(0, 50, 0, 0)
        button_layout.setSpacing(15) 

        self.button_structured = QPushButton("Structured")
        self.button_raw = QPushButton("Raw")
        self.button_structured.setCheckable(True)
        self.button_raw.setCheckable(True)
        self.button_structured.setChecked(True)

        self.button_structured.setFixedSize(150, 40)
        self.button_raw.setFixedSize(150, 40)

        self.button_structured.clicked.connect(self.on_structured_clicked)
        self.button_raw.clicked.connect(self.on_raw_clicked)

        button_layout.addWidget(self.button_structured)
        button_layout.addWidget(self.button_raw)
        button_layout.addStretch()  

        container_layout.addWidget(table_widget)
        container_layout.addLayout(button_layout)

        self.raw_text_edit = QTextEdit()
        self.raw_text_edit.setReadOnly(True)
        self.raw_text_edit.setVisible(False)
        self.raw_text_edit.setFont(QFont("Courier", 12)) 
        main_layout.addWidget(self.raw_text_edit)

    def on_structured_clicked(self):
        if not self.button_structured.isChecked():
            self.button_structured.setChecked(True)
        self.button_raw.setChecked(False)
        self.show_boot_sector_data()
        self.frame.setVisible(True) 
        self.raw_text_edit.setVisible(False)  
    
    def on_raw_clicked(self):
        if not self.button_raw.isChecked():
            self.button_raw.setChecked(True)
        self.button_structured.setChecked(False)
        self.show_raw_boot_sector_data()
        self.frame.setVisible(False)  
        self.raw_text_edit.setVisible(True)  

    def update_data(self, data):
        for i in reversed(range(1, self.grid_layout.rowCount())):
            for j in range(3):
                item = self.grid_layout.itemAtPosition(i, j)
                if item is not None:
                    widget = item.widget()
                    if widget is not None:
                        widget.deleteLater()

        for row, (desc, value, bytes_range) in enumerate(data, 1):
            desc_label = QLabel(desc)
            desc_label.setStyleSheet("QLabel { padding: 8px; }")
            desc_label.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
            self.grid_layout.addWidget(desc_label, row, 0)

            value_label = QLabel(value)
            value_label.setStyleSheet("QLabel { padding: 8px; }")
            value_label.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
            self.grid_layout.addWidget(value_label, row, 1)

            bytes_label = QLabel(bytes_range)
            bytes_label.setStyleSheet("QLabel { padding: 8px; }")
            bytes_label.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
            self.grid_layout.addWidget(bytes_label, row, 2)

    def show_raw_boot_sector_data(self):
        try:
            raw_data = bindings.boot_sector_hex()
            print(raw_data)
            self.raw_text_edit.setPlainText(raw_data)
        except Exception as e:
            print(f"Error al mostrar los datos del sector de arranque: {str(e)}")

    def show_boot_sector_data(self):
        try:
            boot_sector_data = bindings.boot_sector_dict()

            if not boot_sector_data:
                return
            data = [
                ("Jump Instruction:", boot_sector_data["jump_instruction"], "0-2"),
                ("OEM ID:", boot_sector_data["oem_name"], "3-10"),
                ("Bytes Per Sector:", str(boot_sector_data["bytes_x_sector"]), "11-12"),
                ("Sectors Per Cluster:", str(boot_sector_data["sectors_x_cluster"]), "13-13"),
                ("Reserved Sectors:", str(boot_sector_data["reserved_sectors"]), "14-15"),
                ("Media Descriptor:", boot_sector_data["media_descriptor"], "21-21"),
                ("Total Sectors:", str(boot_sector_data["sectors_x_volume"]), "40-47"),
                ("MFT Cluster Number:", str(boot_sector_data["cluster_MFT_start"]), "48-55"),
                ("Size of MFT Entry:", str(boot_sector_data["entry_size"]), "64-64"),
                ("Size of Index Record:", str(boot_sector_data["index_size"]), "68-68"),
                ("Serial number:", f"{boot_sector_data['serial_number']:016X}", "72-79")
            ]

            self.update_data(data)
        except Exception as e:
            print(f"Error al mostrar los datos del sector de arranque: {str(e)}")
