from modules import ybserial
import time

serial = ybserial()

serial.send_byte(0x31)
serial.send_byte(0x0D)

array = [0x30, 0x31, 0x32, 0x33, 0x0D]
serial.send_bytearray(array)


text = 'Hello Yahboom'
num = serial.send(text)
print("num:", num)

num = 0
count = 0
CMD_1 = "$A#"
CMD_2 = "$BB#"


while True:
    time.sleep_ms(1000)
    count = count + 1
    if count % 2 == 1:
        num = serial.send(CMD_1)
    else:
        num = serial.send(CMD_2)
    print("num:%d, count:%d" % (num, count))
