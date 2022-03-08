#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import asyncio
import serial
import serial_asyncio
import struct
from sbuslib import SBUSReceiver
import sys

def write_val(file, val):
    file.truncate()
    # file.flush()
    # file.seek(0)
    file.write(str(val))

def write_val_bin(file, val):
    file.truncate()
    var = struct.pack('i', val)
    file.write(var)

async def main(dev_path, bin_path):
    sbus = await SBUSReceiver.create(dev_path)
    i=0
    while True:
        frame = await sbus.get_frame()
        FLAP = frame.sbusChannels[0]
        RUDD = frame.sbusChannels[1]
        ARM = frame.sbusChannels[2]
        if i>10:
            RUDDw = open(bin_path+"rudder.bin", 'w+b')
            write_val_bin(RUDDw, RUDD)
            RUDDw.close()
            FLAPw = open(bin_path+"flap.bin", 'w+b')
            write_val_bin(FLAPw, FLAP)
            FLAPw.close()
            # print("SAF, VER, ARM", FLAP, RUDD, ARM)
            i=0
        i+=1


if __name__ == '__main__':

    dev_path = "/dev/ttyS0"
    bin_path = "./"

    if len(sys.argv)==2:
        dev_path = sys.argv[1]
        bin_path = "./"
    elif len(sys.argv)==3:
        dev_path = sys.argv[1]
        bin_path = sys.argv[2]

    print(dev_path,bin_path)

    loop = asyncio.get_event_loop()
    try:
        loop.run_until_complete(main(dev_path,bin_path))
        loop.run_forever()
        loop.close()
    except KeyboardInterrupt:
        loop.close()
