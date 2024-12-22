import os
from firebase_admin import credentials, initialize_app

# Путь к файлу JSON относительно текущего скрипта
cred_path = os.path.join(os.path.dirname(__file__), 'python-596c2-firebase-adminsdk-5keys-234ba198ec.json')
cred = credentials.Certificate(cred_path)
initialize_app(cred)

print("Firebase инициализирован")
