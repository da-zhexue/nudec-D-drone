import sensor, time
from machine import UART

scanned_qrs = []

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # can be QVGA on M7...
sensor.skip_frames(30)
sensor.set_auto_gain(False) # must turn this off to prevent image washout...
clock = time.clock()
uart = UART(1,115200)

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8) # strength of 1.8 is good for the 2.8mm lens.
    for code in img.find_qrcodes():
        if code.payload() not in scanned_qrs:
            scanned_qrs.append(code.payload())
            print("发现新二维码:",code.payload())
            uart.write(code.payload())
