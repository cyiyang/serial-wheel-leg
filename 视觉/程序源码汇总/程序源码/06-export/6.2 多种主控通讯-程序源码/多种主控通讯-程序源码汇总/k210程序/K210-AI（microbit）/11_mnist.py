import sensor, image, time, lcd
from maix import KPU
import gc

from modules import ybserial
import time

serial = ybserial()

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.skip_frames(time = 100)
clock = time.clock()

kpu = KPU()
kpu.load_kmodel("/sd/KPU/mnist/uint8_mnist_cnn_model.kmodel")

msg_ =""
while True:
    gc.collect()
    img = sensor.snapshot()
    img_mnist1=img.to_grayscale(1)
    img_mnist2=img_mnist1.resize(112,112)
    img_mnist2.invert()
    img_mnist2.strech_char(1)
    img_mnist2.pix_to_ai()

    out = kpu.run_with_output(img_mnist2, getlist=True)
    max_mnist = max(out)
    index_mnist = out.index(max_mnist)
    msg_ = str(index_mnist)
    send_data ="$"+"11"+ msg_ +','+"#"
    time.sleep_ms(5)
    serial.send(send_data)
    score = KPU.sigmoid(max_mnist)
    if index_mnist == 1:
        if score > 0.999:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    elif index_mnist == 5:
        if score > 0.999:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    else:
        display_str = "num: %d" % index_mnist
        print(display_str, score)
        img.draw_string(4,3,display_str,color=(0,0,0),scale=2)


    lcd.display(img)

kpu.deinit()
