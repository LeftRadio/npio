#!/usr/bin/env python3

import npio
import time

PIN_NUM = 7

npio.setmode(npio.BOARD)

npio.setup (PIN_NUM,npio.IN)

# while True:
#     npio.output(PIN_NUM,True)
#     time.sleep(1)
#     npio.output(PIN_NUM,False)
#     time.sleep(1)

