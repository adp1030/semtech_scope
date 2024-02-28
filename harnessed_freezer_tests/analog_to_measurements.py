import pandas as pd
import matplotlib.pyplot as plt

from pressure_transduce.pressure_sensor import PressureSensor
from thermistor_inline_transduce.inline_thermistor import InlineThermistor
from thermistor_surface_transduce.surface_thermistor import SurfaceThermistor




def transduce_raw_data(csv_filename):
    """
    Return dataframe of transduced data given CSV of raw analog readings.
    """        
    surface_therm   = SurfaceThermistor()
    inline_therm    = InlineThermistor()
    pressure_sensor = PressureSensor()

    df = pd.read_csv(csv_filename)

    df['Timestamp'] = pd.to_datetime(df['Timestamp'], format='%Y_%m_%d_t%H_%M_%S')

    # Surface thermistor measurements
    df['Temp surface Z.1 [C]'] = df['A0'].apply(surface_therm.analog_to_temp)
    df['Temp surface Z.3 [C]'] = df['A1'].apply(surface_therm.analog_to_temp)

    # Inline thermistor measurements
    df['Temp inline Z.1 [C]']  = df['A2'].apply(inline_therm.analog_to_temp)
    df['Temp inline OUT [C]']  = df['A3'].apply(inline_therm.analog_to_temp)

    # Pressure sensor measurements
    df['Pressure Z.2 [psi]']   = df['A4'].apply(pressure_sensor.analog_to_pressure_psi)
    df['Pressure OUT [psi]']   = df['A5'].apply(pressure_sensor.analog_to_pressure_psi)


    df['Temp inline OUT [C]'] = df['Temp inline OUT [C]'].where((df['Temp inline OUT [C]'] > -50), other=pd.NA)

    return df


def plot_data(df):

    #df = df[0:1000]
    # Create subplots
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    # Plot temperature
    ax1.plot(df['Timestamp'], df['Temp surface Z.1 [C]'], label='Zone 1 surface')
    ax1.plot(df['Timestamp'], df['Temp inline Z.1 [C]'],  label='Zone 1 inline')
    ax1.plot(df['Timestamp'], df['Temp surface Z.3 [C]'], label='Zone 3 surface')
    ax1.plot(df['Timestamp'], df['Temp inline OUT [C]'],  label='OUT inline')
    ax1.set_ylabel('Temperature [C]')
    ax1.legend()

    # Plot pressure
    ax2.plot(df['Timestamp'], df['Pressure Z.2 [psi]'],  label='Zone 2')
    ax2.plot(df['Timestamp'], df['Pressure OUT [psi]'],  label='OUT')
    ax2.set_ylabel('Pressure [psi]')
    ax2.set_xlabel('Timestamp')
    ax2.legend()

    plt.suptitle('Temperature and Pressure Time Series')
    plt.tight_layout()
    plt.show()


if __name__ == '__main__':

    df = transduce_raw_data('Data_2024_02_21_t09_30_55_fixed.csv')
    #df = transduce_raw_data('Data_2024_02_15_t10_08_45.csv')
    #df = pd.read_csv('Transduced_data.csv')

    print(df)
    plot_data(df)

    df.to_csv('Transduced_data_drip_test.csv', index=False)
