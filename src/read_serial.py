R"py(
# uncomment above to run the file normally
# needed for #include

from serial import Serial
from pathlib import Path
from sys import argv
from time import sleep

# https://github.com/jjbel/exercycle/blob/main/serial_reader.py
# https://pyserial.readthedocs.io/en/latest/tools.html#serial.tools.list_ports.ListPortInfo
# rpi doesn't showup in ListPortInfo object

PORT = argv[1]
file = Path('data.txt')
contents = ''

print(f"""Hello!
This program reads data from the RPI through a serial port.
Using serial port: {PORT}
Writing data to file: {file}

Close this window when you are done.
Happy flying!
""")

# TODO what baudrate to use?
with Serial(PORT, 9600) as ser:
    while True:
        # remove extra newline
        r = ser.readline().decode()[:-1]
        contents += r
        file.write_text(contents)
        # sleep(1.0)
)py"
