import serial

serial_port = 'COM5'  # Change this to match your Arduino's serial port
baud_rate = 115200

with serial.Serial(serial_port, baud_rate) as ser:
    try:
        with open('Weather Report.txt', 'w') as file:
            while True:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8')
                    print(line, end='', flush=True)
                    file.write(line)
    except KeyboardInterrupt:
        pass
