import sys
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QLabel,
    QPushButton, QVBoxLayout
)

############## Just testing how to use PyQt :) ##############

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NTFS Visualizer")
        self.setGeometry(100, 100, 800, 600)  
        
        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)

        self.layout = QVBoxLayout()
        self.central_widget.setLayout(self.layout)

        self.init_ui()

    def init_ui(self):
        self.clear_layout()

        self.label = QLabel("Hello")
        self.button = QPushButton("Click me")
        
        self.layout.addWidget(self.label)
        self.layout.addWidget(self.button)

        self.button.clicked.connect(self.on_button_click)

    def on_button_click(self):
        self.clear_layout()

        message_label = QLabel("You clicked me!")
        back_button = QPushButton("Back")

        self.layout.addWidget(message_label)
        self.layout.addWidget(back_button)

        back_button.clicked.connect(self.init_ui)

    def clear_layout(self):
        for i in reversed(range(self.layout.count())):
            widget_to_remove = self.layout.itemAt(i).widget()
            if widget_to_remove:
                self.layout.removeWidget(widget_to_remove)
                widget_to_remove.deleteLater()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
