
#import matplotlib.pyplot as plt
#import numpy as np

class PostOutline:

	def __init__(self, filename):
		thisfile = open(filename, "r")
		dataline = thisfile.readlines()
		thisfile.close()

		self.fData = {   "Time":[],  "Step":[], "Current":[], \
                  "Voltage":[],  "Heat":[],   "Temp1":[], \
                    "Temp2":[], "Field":[]};

		for eachline in dataline:
			eachline.strip()
			item = eachline.split()
			self.fData[   "Time"].append(float(item[0]))
			self.fData[   "Step"].append(float(item[1]))
			self.fData["Current"].append(float(item[2]))
			self.fData["Voltage"].append(float(item[3]))
			self.fData[   "Heat"].append(float(item[4]))
			self.fData[  "Temp1"].append(float(item[5]))
			self.fData[  "Temp2"].append(float(item[6]))
			self.fData[  "Field"].append(float(item[7]))

	def GetTime(self):
		return self.fData["Time"]

	def GetTimeStep(self):
		return self.fData["Step"]

	def GetCurrent(self):
		return self.fData["Current"]

	def GetVoltage(self):
		return self.fData["Voltage"]

	def GetHeat(self):
		return self.fData["Heat"]

	def GetTemperature(self, num):
		if num==1:
			return self.fData["Temp1"]
		else:
			return self.fData["Temp2"]

	def GetField(self):
		return self.fData["Field"]
