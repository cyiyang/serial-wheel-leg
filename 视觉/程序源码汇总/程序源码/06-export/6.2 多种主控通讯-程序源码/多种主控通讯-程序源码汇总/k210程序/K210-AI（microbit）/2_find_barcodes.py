import sensor, image, time, math, lcd

from modules import ybserial
import time

serial = ybserial()


lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565) #GRAYSCALE
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)
sensor.set_auto_gain(True)
#sensor.set_auto_whitebal(True)
clock = time.clock()



def barcode_name(code):
    if(code.type() == image.EAN2):
        return "EAN2"
    if(code.type() == image.EAN5):
        return "EAN5"
    if(code.type() == image.EAN8):
        return "EAN8"
    if(code.type() == image.UPCE):
        return "UPCE"
    if(code.type() == image.ISBN10):
        return "ISBN10"
    if(code.type() == image.UPCA):
        return "UPCA"
    if(code.type() == image.EAN13):
        return "EAN13"
    if(code.type() == image.ISBN13):
        return "ISBN13"
    if(code.type() == image.I25):
        return "I25"
    if(code.type() == image.DATABAR):
        return "DATABAR"
    if(code.type() == image.DATABAR_EXP):
        return "DATABAR_EXP"
    if(code.type() == image.CODABAR):
        return "CODABAR"
    if(code.type() == image.CODE39):
        return "CODE39"
    if(code.type() == image.PDF417):
        return "PDF417"
    if(code.type() == image.CODE93):
        return "CODE93"
    if(code.type() == image.CODE128):
        return "CODE128"

msg =""
num=0
while(True):
    clock.tick()
    img = sensor.snapshot()
    fps = clock.fps()
    codes = img.find_barcodes()
    for code in codes:
        img.draw_rectangle(code.rect(), color=(0, 255, 0))
        print_args = (barcode_name(code), code.payload(), (180 * code.rotation()) / math.pi, code.quality())
        print("Barcode %s, Payload \"%s\", rotation %f (degrees), quality %d" % print_args)

        msg = code.payload()
        num=len(msg)
        print (num)

    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)

    if msg !="":
        send_data = "$"+"02"+str(num)+msg+','+"#"
        msg = ""
        print(send_data)
        serial.send(send_data)
    time.sleep_ms(5)
    lcd.display(img)
