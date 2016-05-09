#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
from sys         import argv
from postoutline import PostOutline

post  = PostOutline(argv[1])
time  = post.GetTime()
step  = post.GetTimeStep()
curr  = post.GetCurrent()
volt  = post.GetVoltage()
temp1 = post.GetTemperature(1)
temp2 = post.GetTemperature(2)
field = post.GetField()

plt.plot(time, temp1, "r")
plt.plot(time, temp2, "g")
plt.xlabel("Time [sec]")
plt.ylabel("Temperature [K]")
plt.grid()
plt.show()

plt.plot(time, curr, "r")
plt.xlabel("Time [sec]")
plt.ylabel("Current [A]")
plt.grid()
plt.show()

plt.plot(time, volt, "k")
plt.xlabel("Time [sec]")
plt.ylabel("Voltage [V]")
plt.grid()
plt.show()

plt.plot(time, step, "k")
plt.xlabel("Time [sec]")
plt.ylabel("Time Step [msec]")
plt.grid()
plt.show()
