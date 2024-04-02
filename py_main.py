import subprocess
import re
import time
import serial 

Arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1) 

def Arduino_write_read(x): 
	Arduino.write(bytes(x, 'utf-8')) 
	time.sleep(0.05)
	data = Arduino.readline().decode("utf-8")
	return data

# ping every 1 second
time_a = time.perf_counter()
while True:
    if time.perf_counter() - time_a >= 1:
        # ping_process = subprocess.Popen(["ping.exe","192.168.13.19","-n","1"], stdout = subprocess.PIPE)
        ping_process = subprocess.Popen(["ping.exe","119.252.37.13","-n","1"], stdout = subprocess.PIPE)
        ping_string = ping_process.communicate()[0].decode("utf-8")
        ping_match = re.findall(r'time[=<](\d+)ms', ping_string)
        if ping_match != []:
            ping_match[0] = ping_match[0] + 'z'
            print(f'Python : {ping_match[0]}')
            print(f'Arduino: {Arduino_write_read(ping_match[0])}')
        else:
            print('python ping error')
            print(Arduino_write_read('999z'))
        time_a = time.perf_counter()
