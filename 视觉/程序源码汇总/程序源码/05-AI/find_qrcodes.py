import sensor, image, time, lcd

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)

clock = time.clock()
while(True):
    clock.tick()
    img = sensor.snapshot()
    for code in img.find_qrcodes():
        img.draw_rectangle(code.rect(), color = 127, thickness=3)
        img.draw_string(code.x(),code.y()-20,code.payload(),color=(255,0,0),scale=2)
        print(code)
    lcd.display(img)
    #print(clock.fps())
