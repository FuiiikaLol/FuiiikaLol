import sys
from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QStackedWidget, QLineEdit, QLabel, QGraphicsOpacityEffect
from PySide6.QtCore import Qt, QPropertyAnimation, QRect, QPoint
from PySide6.QtGui import QPainter, QFont, QColor
import firebase_admin
from firebase_admin import credentials, auth

print("Инициализация Firebase")

# Инициализация Firebase
cred = credentials.Certificate("L:\\python\\Launcher\\python-596c2-firebase-adminsdk-5keys-234ba198ec.json")
firebase_admin.initialize_app(cred)

print("Firebase инициализирован")

class MainWidget(QWidget):
    def __init__(self):
        super().__init__()
        print("Инициализация MainWidget")
        self.setStyleSheet("background-color: #000000;")

        # Создание стека виджетов
        self.stacked_widget = QStackedWidget()

        # Инициализация экранов
        self.main_screen = self.create_main_screen()
        self.login_screen = self.create_auth_screen("Login", self.show_main_screen, self.login_user)
        self.register_screen = self.create_auth_screen("Register", self.show_main_screen, self.register_user)
        self.coming_soon_screen = self.create_coming_soon_screen()

        # Добавление экранов в стек
        self.stacked_widget.addWidget(self.main_screen)
        self.stacked_widget.addWidget(self.login_screen)
        self.stacked_widget.addWidget(self.register_screen)
        self.stacked_widget.addWidget(self.coming_soon_screen)

        # Главный макет
        layout = QVBoxLayout(self)
        close_button_layout = QHBoxLayout()

        self.close_button = QPushButton("Close", self)
        self.close_button.setFixedSize(80, 40)
        self.close_button.setStyleSheet("""
            background-color: #000000;
            color: #FFFFFF;
            border: none;
            font-size: 16px;
        """)
        self.close_button.setFont(QFont("Comic Sans MS", 16))
        self.close_button.clicked.connect(self.animate_close)
        close_button_layout.addStretch()
        close_button_layout.addWidget(self.close_button)

        layout.addLayout(close_button_layout)
        layout.addWidget(self.stacked_widget)

        self.setLayout(layout)

        # Анимация переходов между виджетами
        self.opacity_effect = QGraphicsOpacityEffect(self.stacked_widget)
        self.stacked_widget.setGraphicsEffect(self.opacity_effect)
        self.fade_in = QPropertyAnimation(self.opacity_effect, b"opacity")
        self.fade_in.setDuration(500)
        self.fade_in.setStartValue(0)
        self.fade_in.setEndValue(1)

        print("MainWidget инициализирован")

    def create_main_screen(self):
        main_screen = QWidget()
        layout = QVBoxLayout(main_screen)
        layout.setAlignment(Qt.AlignCenter)

        login_button = QPushButton("Login", main_screen)
        login_button.setStyleSheet("background-color: #000000; color: #FFFFFF; border: none; font-size: 32px;")
        login_button.setFont(QFont("Comic Sans MS", 32))
        login_button.clicked.connect(lambda: self.fade_to_widget(self.login_screen))
        layout.addWidget(login_button)

        register_button = QPushButton("Register", main_screen)
        register_button.setStyleSheet("background-color: #000000; color: #FFFFFF; border: none; font-size: 24px;")
        register_button.setFont(QFont("Comic Sans MS", 24))
        register_button.clicked.connect(lambda: self.fade_to_widget(self.register_screen))
        layout.addWidget(register_button)

        print("Main screen создан")

        return main_screen

    def create_auth_screen(self, title, back_action, submit_action=None):
        screen = QWidget()
        layout = QVBoxLayout(screen)
        layout.setAlignment(Qt.AlignCenter)

        back_button = QPushButton("Back", screen)
        back_button.setStyleSheet("background-color: #000000; color: #FFFFFF; border: none; font-size: 16px;")
        back_button.setFont(QFont("Comic Sans MS", 16))
        back_button.clicked.connect(back_action)
        layout.addWidget(back_button)

        email_label = QLabel("Email", screen)
        email_label.setStyleSheet("color: #FFFFFF; font-size: 16px;")
        email_label.setFont(QFont("Comic Sans MS", 16))
        layout.addWidget(email_label)

        email_input = QLineEdit(screen)
        email_input.setStyleSheet("background-color: #333333; color: #FFFFFF; padding: 10px; border: 1px solid #FFFFFF;")
        layout.addWidget(email_input)

        password_label = QLabel("Password", screen)
        password_label.setStyleSheet("color: #FFFFFF; font-size: 16px;")
        password_label.setFont(QFont("Comic Sans MS", 16))
        layout.addWidget(password_label)

        password_input = QLineEdit(screen)
        password_input.setEchoMode(QLineEdit.Password)
        password_input.setStyleSheet("background-color: #333333; color: #FFFFFF; padding: 10px; border: 1px solid #FFFFFF;")
        layout.addWidget(password_input)

        submit_button = QPushButton(title, screen)
        submit_button.setStyleSheet("background-color: #444444; color: white; border: none; padding: 10px;")
        submit_button.setFont(QFont("Comic Sans MS", 16))
        if submit_action:
            submit_button.clicked.connect(lambda: submit_action(email_input.text(), password_input.text()))
        layout.addWidget(submit_button)

        self.auth_status_label = QLabel("", screen)
        self.auth_status_label.setStyleSheet("color: #FFFFFF; font-size: 14px;")
        layout.addWidget(self.auth_status_label)

        print(f"{title} screen создан")

        return screen

    def create_coming_soon_screen(self):
        screen = QWidget()
        layout = QVBoxLayout(screen)
        layout.setAlignment(Qt.AlignCenter)

        label = QLabel("Coming Soon", screen)
        label.setStyleSheet("color: #FFFFFF; font-size: 32px;")
        label.setFont(QFont("Comic Sans MS", 32))
        layout.addWidget(label)

        print("Coming soon screen создан")

        return screen

    def register_user(self, email, password):
        try:
            user = auth.create_user(email=email, password=password)
            self.auth_status_label.setText("Successfully registered")
        except Exception as e:
            self.auth_status_label.setText("Registration failed")
        self.show_main_screen()

    def login_user(self, email, password):
        try:
            user = auth.get_user_by_email(email)
            if user and password:  # Проверяем пароль (упрощённая проверка)
                self.fade_to_widget(self.coming_soon_screen)
            else:
                self.auth_status_label.setText("Authorization failed")
        except Exception as e:
            self.auth_status_label.setText("Authorization failed")

    def fade_to_widget(self, widget):
        self.stacked_widget.setCurrentWidget(widget)
        self.fade_in.start()

    def show_main_screen(self):
        self.fade_to_widget(self.main_screen)

    def animate_close(self):
        self.fade_out = QPropertyAnimation(self, b"windowOpacity")
        self.fade_out.setDuration(500)  # Длительность анимации
        self.fade_out.setStartValue(1)  # Начальная прозрачность
        self.fade_out.setEndValue(0)  # Конечная прозрачность
        self.fade_out.finished.connect(QApplication.quit)  # Завершение приложения после анимации
        self.fade_out.start()

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.drag_pos = event.globalPos() - self.frameGeometry().topLeft()
            event.accept()

    def mouseMoveEvent(self, event):
        if event.buttons() == Qt.LeftButton:
            self.move(event.globalPos() - self.drag_pos)
            event.accept()

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("")
        self.resize(800, 600)
        self.setWindowFlags(Qt.FramelessWindowHint)

        # Центрирование окна
        screen_geometry = QApplication.primaryScreen().geometry()
        x = (screen_geometry.width() - self.width()) / 2
        y = (screen_geometry.height() - self.height()) / 2
        self.move(x, y)

        self.main_widget = MainWidget()
        self.setCentralWidget(self.main_widget)

        print("MainWindow инициализирован")

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.fillRect(self.rect(), QColor(0, 0, 0))  # Черный фон

if __name__ == "__main__":
    print("Программа начинает работу")
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    print("Программа запускает событийный цикл")
    sys.exit(app.exec())
    print("Программа завершена")
