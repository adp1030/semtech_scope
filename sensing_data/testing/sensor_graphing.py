# graph data from sensors saved as CSVs.
# Sensors are currently being read every 100ms.

import pandas as pd
import matplotlib.pyplot as plt

#import CSV
sensor_data = pd.read_csv('microscope_test_2.csv')

# graph pressure
# use iloc[x:y] to limit data range
sensor_data.plot(x='time', y='pressure', label='Pressure (psi)')
plt.xlabel("Time (s)")
plt.ylabel("Pressure (psi)")
plt.title("Pipe Pressure Sensor Data")
plt.savefig("pressure_data_plot_2.png")

# graph temp
sensor_data.plot(x='time', y='temp', label='Temperature (C)')
plt.xlabel("Time (s)")
plt.ylabel("Temperature (C)")
plt.title("Pipe Temperature Sensor Data")
plt.savefig("temp_data_plot_2.png")
