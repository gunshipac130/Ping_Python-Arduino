import subprocess
import re
import time

# ping every 1 second
time_a = time.perf_counter()
while True:
    if (time.perf_counter() - time_a >= 1000) {
        ping_process = subprocess.Popen(["ping.exe","1.1.1.1","-n","1"], stdout = subprocess.PIPE)
        ping_string = ping_process.communicate()[0].decode("utf-8")
        print(ping_string)
        ping_match = re.findall(r'time=(\d+)ms', ping_string)
        print(ping_match[0])
        time_a = time.perf_counter()
    }
