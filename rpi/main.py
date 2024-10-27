from time import sleep
from machine import Pin

DELAY_MS = 100
TEST_DATA_FILE = 'sample_data_trimmed.csv'

led = Pin('LED', Pin.OUT)

with open(TEST_DATA_FILE) as file:
    data = file.read().splitlines()

count = 0
while True:
    for line in data:
        # print(count, line)
        print(line)
        sleep(DELAY_MS / 1000)
        led.toggle()
        count += 1
