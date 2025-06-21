import touchscreen as ts
import lcd, image
import time

lcd.init()
ts.init()

img = image.Image()
img.draw_string(100, 0, "Please touch the screen", color=(0, 0, 255), scale=1)

status_last = ts.STATUS_IDLE
x_last = 0
y_last = 0

while True:
    (status, x, y) = ts.read()
    if status_last != status:
        print(status, x, y)
        status_last = status

    if status == ts.STATUS_MOVE:
        img.draw_line(x_last, y_last, x, y)
    elif status == ts.STATUS_PRESS:
        img.draw_line(x, y, x, y)
    lcd.display(img)
    x_last = x
    y_last = y
