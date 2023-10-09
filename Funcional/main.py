import serial

# Porta serial à qual a ESP32 está conectada (por exemplo, 'COM3' no Windows ou '/dev/ttyUSB0' no Linux)
porta_serial = 'COM14'  # Altere para a porta correta

# Taxa de transmissão (baud rate)
baud_rate = 115200  # Certifique-se de que corresponde à configuração da ESP32

try:
    # Inicialize a comunicação com a porta serial
    ser = serial.Serial(porta_serial, baud_rate)

    while True:
        # Leia uma linha da porta serial
        linha = ser.readline().decode('utf-8').strip()
        
        # Imprima a linha lida
        print('Linha recebida:', linha)

except serial.SerialException as e:
    print('Ocorreu um erro ao tentar abrir a porta serial:', str(e))
finally:
    # Feche a porta serial ao sair
    pass
