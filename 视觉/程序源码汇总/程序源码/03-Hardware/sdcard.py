import sensor
import lcd
import image


lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.run(1)
sensor.skip_frames(10)
print("init ok")

img_path = "/sd/image-1.jpg"

img = sensor.snapshot()
print("save image")
img.save(img_path)


print("read image")
img_read = image.Image(img_path)
lcd.display(img_read)
print("ok")

