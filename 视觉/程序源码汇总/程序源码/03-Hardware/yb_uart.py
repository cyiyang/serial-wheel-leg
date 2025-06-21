from fpioa_manager import fm
from machine import UART
import time

# binding UART2 IO:6->RX, 8->TX
fm.register(6, fm.fpioa.UART2_RX)
fm.register(8, fm.fpioa.UART2_TX)

yb_uart = UART(UART.UART2, 115200, 8, 0, 0, timeout=1000, read_buf_len=4096)

write_bytes = b'$hello yahboom#\n'
last_time = time.ticks_ms()

try:
    while True:
        # send data per 500ms
        if time.ticks_ms() - last_time > 500:
            last_time = time.ticks_ms()
            yb_uart.write(write_bytes)
        # read and print data
        if yb_uart.any():
            read_data = yb_uart.read()
            if read_data:
                print("read_data = ", read_data)
except:
    pass

yb_uart.deinit()
del yb_uart
