from PyQt6.QtWidgets import (QWidget, QVBoxLayout, QGridLayout, QLabel,
    QSizePolicy, QFrame, QPushButton, QHBoxLayout)

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

        frame = QFrame()
        frame.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        frame.setObjectName("BodyFrame")

        grid_layout = QGridLayout(frame)
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

        main_layout.addWidget(frame)

        button_layout = QVBoxLayout()
        button_layout.setContentsMargins(0, 50, 0, 0)
        button_layout.setSpacing(15) 

        button1 = QPushButton("Structured")
        button2 = QPushButton("Raw")

        button1.setFixedSize(150, 40)
        button2.setFixedSize(150, 40)

        button_layout.addWidget(button1)
        button_layout.addWidget(button2)
        button_layout.addStretch()  

        container_layout.addWidget(table_widget)
        container_layout.addLayout(button_layout)

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