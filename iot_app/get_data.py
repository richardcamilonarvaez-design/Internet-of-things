import serial
import time
arduino_port = 'COM3' 
baud_rate = 9600
ser = serial.Serial(
    arduino_port, 
    baud_rate, 
    timeout=1
)
time.sleep(2) 
while True:
    data = ser.readline().decode('utf-8').rstrip();
    print(data);
