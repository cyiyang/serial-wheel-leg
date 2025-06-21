from modules import ybkey
import time

KEY = ybkey()

while True:
    state = KEY.is_press()
    print("key press:", state)
    time.sleep_ms(100)
