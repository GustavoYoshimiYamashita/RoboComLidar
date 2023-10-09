import socket

HOST = '192.168.1.102'  # Substitua pelo IP do seu ESP32
PORT = 8080

while True:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        data = s.recv(1024)
        if not data:
            break
        print(data.decode())

    

