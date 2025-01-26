from PyQt6.QtWidgets import QWidget, QVBoxLayout, QGridLayout, QLabel, QSizePolicy, QFrame
from PyQt6.QtGui import QFont

class BootSectorTab(QWidget):
    def __init__(self):
        super().__init__()
        self.setup_ui()

    def setup_ui(self):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(50, 50, 300, 50)
        main_layout.setSpacing(0)

        frame = QFrame(self)
        frame.setGeometry(10, 10, 900, 645)
        frame.setFrameShape(QFrame.Shape.StyledPanel)
        frame.setFrameShadow(QFrame.Shadow.Raised)

        grid_layout = QGridLayout(frame)
        grid_layout.setContentsMargins(10, 20, 10, 20)
        grid_layout.setSpacing(0)

        self.add_headers(grid_layout)

        self.add_data(grid_layout)

        main_layout.addWidget(frame)

    def add_headers(self, grid_layout):
        header_frame = QFrame(parent=grid_layout.parent())
        header_frame.setFrameShape(QFrame.Shape.StyledPanel)
        header_frame.setFrameShadow(QFrame.Shadow.Raised)
        header_frame.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        header_layout = QGridLayout(header_frame)
        header_layout.setContentsMargins(0, 0, 0, 0)
        header_layout.setSpacing(0)

        headers = ["Property", "Value", "ByteRange"]
        for col, header_text in enumerate(headers):
            header_label = QLabel(header_text, parent=header_frame)
            header_label.setFont(self.get_bold_font())
            header_layout.addWidget(header_label, 0, col)

        grid_layout.addWidget(header_frame, 0, 0, 1, -1)

    def add_data(self, grid_layout): # Test data
        data = [
            ("Jump Instruction", "EB 52 90", "0-2"),
            ("OEM ID", "NTFS", "3-10"),
            ("Bytes Per Sector", "512", "11-12"),
            ("Sectors Per Cluster", "8", "13-13"),
            ("Reserved Sectors", "0", "14-15"),
            ("Media Descriptor", "F8", "21-21"),
            ("Sectors Per Track", "63", "24-25"),
            ("Number of Heads", "255", "26-27"),
            ("Hidden Sectors", "0", "28-31"),
            ("Total Sectors", "2097152", "40-47"),
            ("MFT Cluster Number", "785432", "48-55"),
            ("MFT Mirror Cluster Number", "785433", "56-63"),
            ("Size of MFT Entry", "1024", "64-64"),
            ("Size of index record", "4096", "68-68"),
            ("Volume Serial Number", "6FD0 CD18 09CE 186E", "72-79"),
        ]

        for row, (label, value, byteRange) in enumerate(data, start=1):
            label_widget = QLabel(label, parent=grid_layout.parent())
            label_widget.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
            grid_layout.addWidget(label_widget, row, 0)

            value_widget = QLabel(value, parent=grid_layout.parent())
            value_widget.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Preferred)
            grid_layout.addWidget(value_widget, row, 1)

            bytes_widget = QLabel(byteRange, parent=grid_layout.parent())
            bytes_widget.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Preferred)
            grid_layout.addWidget(bytes_widget, row, 2)

    def get_bold_font(self):
        bold_font = QFont()
        bold_font.setBold(True)
        return bold_font