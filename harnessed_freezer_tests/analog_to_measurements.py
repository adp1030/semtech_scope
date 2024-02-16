import pandas as pd
import matplotlib.pyplot as plt

from pressure_transduce.pressure_sensor import PressureSensor
from thermistor_inline_transduce.inline_thermistor import InlineThermistor
from thermistor_surface_transduce.surface_thermistor import SurfaceThermistor


surface_therm   = SurfaceThermistor()
inline_therm    = InlineThermistor()
pressure_sensor = PressureSensor()


df = pd.read_csv('Data_2024_02_15_t10_08_45_copy.csv')

df['Timestamp'] = pd.to_datetime(df['Timestamp'], format='%Y_%m_%d_t%H_%M_%S')

# # Surface thermistor measurements
# df['Inlet_surface_res [Ohm]']  = df['A0'].apply(surface_therm.analog_to_res)
# df['Outlet_surface_res [Ohm]'] = df['A1'].apply(surface_therm.analog_to_res)

# # Inline thermistor measurements
# df['Inlet_inline_res [Ohm]']   = df['A2'].apply(inline_therm.analog_to_res)
# df['Outlet_inline_res [Ohm]']  = df['A3'].apply(inline_therm.analog_to_res)



# Surface thermistor measurements
df['Inlet_surface_temp [C]']  = df['A0'].apply(surface_therm.analog_to_temp)
df['Outlet_surface_temp [C]'] = df['A1'].apply(surface_therm.analog_to_temp)

# Inline thermistor measurements
df['Inlet_inline_temp [C]']   = df['A2'].apply(inline_therm.analog_to_temp)
df['Outlet_inline_temp [C]']  = df['A3'].apply(inline_therm.analog_to_temp)

# Pressure sensor measurements
df['Inlet_pressure [MPa]']   = df['A4'].apply(pressure_sensor.analog_to_pressure)
df['Outlet_pressure [MPa]']  = df['A5'].apply(pressure_sensor.analog_to_pressure)

df['Inlet_pressure [psi]']   = df['Inlet_pressure [MPa]']  * 145.038
df['Outlet_pressure [psi]']  = df['Outlet_pressure [MPa]'] * 145.038


print(df)



# Create subplots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

# Plot temperature
ax1.plot(df['Timestamp'], df['Inlet_surface_temp [C]'],  label='Inlet_surface')
ax1.plot(df['Timestamp'], df['Outlet_surface_temp [C]'], label='Outlet_surface')
ax1.plot(df['Timestamp'], df['Inlet_inline_temp [C]'],   label='Inlet_inline')
ax1.plot(df['Timestamp'][300:], df['Outlet_inline_temp [C]'][300:], label='Outlet_inline')
ax1.set_ylabel('Temperature [C]')
ax1.legend()

# Plot pressure
ax2.plot(df['Timestamp'], df['Inlet_pressure [psi]'],  label='Inlet')
ax2.plot(df['Timestamp'], df['Outlet_pressure [psi]'], label='Outlet')
ax2.set_ylabel('Pressure [psi]')
ax2.set_xlabel('Timestamp')
ax2.legend()

plt.suptitle('Temperature and Pressure Time Series')
plt.tight_layout()
plt.show()

# # Plot raw analog values
# plt.figure(figsize=(10, 6))
# plt.plot(df['Timestamp'], df['A0'], label='A0')
# plt.plot(df['Timestamp'], df['A1'], label='A1')
# plt.plot(df['Timestamp'], df['A2'], label='A2')
# plt.plot(df['Timestamp'], df['A3'], label='A3')
# plt.plot(df['Timestamp'], df['A4'], label='A4')
# plt.plot(df['Timestamp'], df['A5'], label='A5')
# plt.xlabel('Timestamp')
# plt.ylabel('Analog')
# plt.legend()
# plt.title('Analog vs. time')
# plt.show()
