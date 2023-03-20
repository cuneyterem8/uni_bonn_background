import matplotlib.pyplot as plt
import math as m
import geopy.distance

from math import sin, cos, sqrt, atan2, radians
import csv

# The radius of earth in an approximate
Total_Line = 0

# The coordinates are based on the transmitter that is a fixed antenna located at 50◦42025.96”N, 7◦5'49.13”E
Coordinates = (50 + (42 / 60) + (25.96 / 3600), 7 + (5 / 60) + (49.13 / 3600))


# Initial Benchmarks for distances
d0 = 1
d1 = 200
d2 = 500

# Transmitter antenna height
Ht = 50
# Receiver antenna height
Hr = 2

# Path loss distance exponent for each field
n0 = 2.0
n1 = 3.0
n2 = 3.0

Lambda = 1.35

# Path loss at reference distance d0
L0 = 19.377

# For small distances, we use Crossover distance dc
dc = (4 * m.pi * Hr * Ht / Lambda)

# Reading the data from ex1.csv
ExcelData_File = open('ex1.csv', 'r')


Data_Reader = csv.reader(ExcelData_File)
signal = []
data = []


for row in Data_Reader:
    data.append(row)
    Total_Line += 1
    signal.append(float(row[3]))

# Getting Three Log Distance Model (TLD)
def getTLD():
    count = 0
    while count < Total_Line:
        # We use geopy distance web services for locating coordinates
        distance = geopy.distance.distance(Coordinates, (data[count][1], data[count][2])).meters
        if distance < d0:
            PLWithTLD[count] = 0
        elif d0 <= distance < d1:
            PLWithTLD[count] = L0
            PLWithTLD[count] += (10 * n0 * m.log10(distance / d0))

        elif d1 <= distance < d2:
            PLWithTLD[count] = L0
            PLWithTLD[count] += (10 * n0 * m.log10(d1 / d0))
            PLWithTLD[count] += (10 * n1 * m.log10(distance / d1))

        elif distance >= d2:
            PLWithTLD[count] = L0
            PLWithTLD[count] += (10 * n0 * m.log10(d1 / d0))
            PLWithTLD[count] += (10 * n1 * m.log10(d2 / d1))
            PLWithTLD[count] += (10 * n1 * m.log10(distance / d2))

        count = count + 1

# A method to deal with Path loss based on two geographic coordinates using Geopy geocoding web services
def getFT():
    count = 0
    while count < Total_Line:
        distance = geopy.distance.distance(Coordinates, (data[count][1], data[count][2])).meters
        if distance <= dc:
            PLWithFT[count] = getFSP(distance)
        elif distance > dc:
            PLWithFT[count] = getTRG(distance)
        count += 1


# A method to return Free Space Propagation Model (FSP)
def getFSP(d):
    return 10 * m.log10((d * 4 * m.pi / Lambda) ** 2)

# A method to return e Two-Ray Ground Propagation Model (TRG)
def getTRG(d):
    return 10 * m.log10((d * d / (Hr * Ht)) ** 2)


# Path Loss with FT
PLWithFT =Total_Line * [None]

# Path Loss with TLD
PLWithTLD = Total_Line * [None]


# Calling FT and TLD methods
getFT()
getTLD()

# Plotting Path Loss with Measurements showing FSP/TRG and TLD
PathLM = plt.subplot(211)
PathLM.plot(PLWithFT, color='green')
PathLM.plot(PLWithTLD, color='#f542c8')
PathLM.legend(('FSP/TRG', 'TLD'), loc='lower left')
PathLM.set_ylabel('Path Loss')
PathLM.set_xlabel('Measurements')

# Plotting Signal Strength with Measurement
SignalM = plt.subplot(212)
SignalM.set_ylabel('Signal Strength')
SignalM.set_xlabel('Measurements')
SignalM.plot(signal,color='#f542c8')

# Showing the plots
plt.show()
