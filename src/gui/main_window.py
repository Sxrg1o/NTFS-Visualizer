import sys
import os
from PyQt6.QtWidgets import QApplication, QMainWindow
from widgets.menubar import MenuBar
from widgets.welcome import Ui_MainWindow

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NTFS Visualizer")
        self.setGeometry(100, 100, 1200, 900)
        
        self.ui = Ui_MainWindow()
        self.menu_bar = MenuBar(self)
        
        self.menu_bar.connect_dark_theme()
        self.menu_bar.connect_light_theme()
        
        self.current_theme = "light"
        self.load_theme(self.current_theme)
        
        self.ui.setupUi(self)

    def load_theme(self, theme):
        self.current_theme = theme
        
        current_dir = os.path.dirname(os.path.abspath(__file__))
        css_path = os.path.join(current_dir, 'assets', 'styles.css')
        
        try:
            with open(css_path, 'r') as file:
                css_content = file.read()
                
                if theme == "dark":
                    self.setStyleSheet("")  
                    self.setProperty("class", "dark") 
                else:
                    self.setStyleSheet("")  
                    self.setProperty("class", "")  

                self.setStyleSheet(css_content)

                self.style().unpolish(self)
                self.style().polish(self)
                self.update()
                
        except FileNotFoundError:
            print(f"Error finding CSS file at {css_path}")
        except Exception as e:
            print(f"Error loading theme: {str(e)}")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())