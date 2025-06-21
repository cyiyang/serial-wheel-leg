import lcd
import time

lcd.init()
lcd.clear(lcd.BLUE)


lcd.rotation(0)
lcd.draw_string(30, 30, "Hello Yahboom", lcd.WHITE, lcd.BLUE)
time.sleep(1)

lcd.clear(lcd.BLUE)
lcd.rotation(1)
lcd.draw_string(30, 30, "Hello Yahboom", lcd.WHITE, lcd.BLUE)
time.sleep(1)

lcd.clear(lcd.BLUE)
lcd.rotation(2)
lcd.draw_string(30, 30, "Hello Yahboom", lcd.WHITE, lcd.BLUE)
time.sleep(1)

lcd.clear(lcd.BLUE)
lcd.rotation(3)
lcd.draw_string(30, 30, "Hello Yahboom", lcd.WHITE, lcd.BLUE)
time.sleep(1)

lcd.clear(lcd.BLUE)
lcd.rotation(0)
lcd.draw_string(100, 100, "Hello Yahboom", lcd.WHITE, lcd.BLUE)
time.sleep(1)

