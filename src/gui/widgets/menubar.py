from PyQt6 import QtWidgets, QtGui, QtCore

class MenuBar:
    def __init__(self, main_window):
        self.main_window = main_window
        self.setup_menubar()

    def setup_menubar(self):
        # Create main menu bar
        self.menubar = QtWidgets.QMenuBar(parent=self.main_window)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1200, 24))
        self.menubar.setObjectName("menuBar")

        # Create main menu items
        self.menu_file = QtWidgets.QMenu(parent=self.menubar)
        self.menu_file.setObjectName("menuFile")
        self.menu_file.setTitle("File")

        self.menu_open = QtWidgets.QMenu(parent=self.menu_file)
        self.menu_open.setObjectName("menuOpen")
        self.menu_open.setTitle("Open")


        self.menu_view = QtWidgets.QMenu(parent=self.menubar)
        self.menu_view.setObjectName("menuView")
        self.menu_view.setTitle("View")


        self.menu_options = QtWidgets.QMenu(parent=self.menubar)
        self.menu_options.setObjectName("menuOptions")
        self.menu_options.setTitle("Options")

        self.menu_theme = QtWidgets.QMenu(parent=self.menu_options)
        self.menu_theme.setObjectName("menuTheme")
        self.menu_theme.setTitle("Theme")


        self.menu_help = QtWidgets.QMenu(parent=self.menubar)
        self.menu_help.setObjectName("menuHelp")
        self.menu_help.setTitle("Help")

        # File menu
        self.action_generate = QtGui.QAction(parent=self.main_window)
        self.action_generate.setObjectName("actionGenerate")
        self.action_generate.setText("Generate")

        self.action_file = QtGui.QAction(parent=self.main_window)
        self.action_file.setObjectName("actionFile")
        self.action_file.setText("File")

        self.action_partition = QtGui.QAction(parent=self.main_window)
        self.action_partition.setObjectName("actionPartition")
        self.action_partition.setText("Partition")

        # Create actions for Options menu
        self.theme_dark = QtGui.QAction(parent=self.main_window)
        self.theme_dark.setObjectName("actionDarkTheme")
        self.theme_dark.setText("Dark Theme")

        self.theme_light = QtGui.QAction(parent=self.main_window)
        self.theme_light.setObjectName("actionLightTheme")
        self.theme_light.setText("Light Theme")

        # Add actions to Open submenu
        self.menu_open.addAction(self.action_file)
        self.menu_open.addAction(self.action_partition)

        # Add actions to Theme submenu
        self.menu_theme.addAction(self.theme_dark)
        self.menu_theme.addAction(self.theme_light)

        # Add items to File menu
        self.menu_file.addAction(self.menu_open.menuAction())
        self.menu_file.addAction(self.action_generate)
        self.menu_file.addSeparator()

        # Add items to Options menu
        self.menu_options.addAction(self.menu_theme.menuAction())
        self.menu_options.addSeparator()

        self.menubar.addAction(self.menu_file.menuAction())
        self.menubar.addAction(self.menu_view.menuAction())
        self.menubar.addAction(self.menu_options.menuAction())
        self.menubar.addAction(self.menu_help.menuAction())

        self.main_window.setMenuBar(self.menubar)

    def connect_generate_action(self, slot):
        """Connect the generate action to a slot"""
        self.action_generate.triggered.connect(slot)

    def connect_file_action(self, slot):
        """Connect the file action to a slot"""
        self.action_file.triggered.connect(slot)

    def connect_partition_action(self, slot):
        """Connect the partition action to a slot"""
        self.action_partition.triggered.connect(slot)

    def connect_dark_theme(self):
        self.theme_dark.triggered.connect(lambda: self.toggle_theme("dark"))

    def connect_light_theme(self):
        self.theme_light.triggered.connect(lambda: self.toggle_theme("light"))

    def toggle_theme(self, theme):
        if theme == "dark":
            self.theme_dark.setDisabled(True) 
            self.theme_light.setDisabled(False)  

        elif theme == "light":
            self.theme_light.setDisabled(True) 
            self.theme_dark.setDisabled(False)  
            
        self.main_window.load_theme(theme)