import sensor, image, time, lcd

from modules import ybserial
import time

serial = ybserial()

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)


msg =""
num=0
n=0
clock = time.clock()
while(True):
    clock.tick()
    img = sensor.snapshot()
    for code in img.find_qrcodes():
        img.draw_rectangle(code.rect(), color = 127, thickness=3)
        img.draw_string(code.x(),code.y()-20,code.payload(),color=(255,0,0),scale=2)
        msg = code.payload()
        print(code)
        num=len(msg)
        n=num



    if num < 10:
        n="%02d"%num
        #print(n)

    if msg !="":
        send_data = "$"+"03"+str(n)+msg+','+"#"
        msg = ""
        serial.send(send_data)


    time.sleep_ms(5)
    lcd.display(img)
    #print(clock.fps())
