import subprocess
import re
# https://regex101.com/
ping_process = subprocess.Popen(["ping.exe","1.1.1.1","-n","1"], stdout = subprocess.PIPE)
ping_string = ping_process.communicate()[0].decode("utf-8")
print(ping_string)
ping_match = re.findall(r'time=(\d+)ms', ping_string)
print(ping_match[0])