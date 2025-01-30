from PyQt6.QtWidgets import QWidget, QVBoxLayout, QTabWidget, QLabel

from widgets.bootSector.boot_sector import BootSectorTab

class Tabs(QWidget):
    def __init__(self):
        super().__init__()
        self.setup_ui()

    def setup_ui(self):
        self.tab_widget = QTabWidget()
        
        # Create the boot sector tab
        self.boot_sector_tab = BootSectorTab()
        self.mft_tab = QWidget()
        self.data_area_tab = QWidget()

        # Add tabs
        self.tab_widget.addTab(self.boot_sector_tab, "Boot Sector")
        self.tab_widget.addTab(self.mft_tab, "MFT")
        self.tab_widget.addTab(self.data_area_tab, "Data Area")

        # Setup other tabs
        mft_layout = QVBoxLayout()
        mft_layout.addWidget(QLabel("MFT Tab Content"))
        self.mft_tab.setLayout(mft_layout)

        data_area_layout = QVBoxLayout()
        data_area_layout.addWidget(QLabel("Data Area Tab Content"))
        self.data_area_tab.setLayout(data_area_layout)

        # Main layout
        main_layout = QVBoxLayout()
        main_layout.addWidget(self.tab_widget)
        self.setLayout(main_layout)

    def set_current_tab(self, tab_name):
        for i in range(self.tab_widget.count()):
            if self.tab_widget.tabText(i) == tab_name:
                self.tab_widget.setCurrentIndex(i)
                break

    def get_boot_sector_tab(self):
        return self.boot_sector_tab