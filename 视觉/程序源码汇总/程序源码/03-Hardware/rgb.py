from modules import ybrgb
import time
RGB = ybrgb()

while True:
    RGB.set(1, 0, 0)
    time.sleep(.5)
    RGB.set(0, 1, 0)
    time.sleep(.5)
    RGB.set(0, 0, 1)
    time.sleep(.5)
    RGB.set(1, 1, 1)
    time.sleep(.5)
    RGB.set(0, 0, 0)
    time.sleep(.5)
