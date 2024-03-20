import pandas as pd
import matplotlib.pyplot as plt
import plotly.graph_objects as go
from plotly.subplots import make_subplots

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

    columns_to_exclude = [f'A{i}' for i in range(6)]

    # Create a new DataFrame without the specified columns
    new_df = df.drop(columns=columns_to_exclude)

    return new_df


def plot_data_plotly(df):

    # Create subplots
    fig = go.Figure()

    # Create figure with subplots & shared x-axes
    fig = make_subplots(rows=2, cols=1, vertical_spacing=0.05, shared_xaxes=True,)

    # Plot temperature
    fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Temp surface Z.1 [C]'], mode='lines', name='Zone 1 surface'), row=1, col=1)
    fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Temp inline Z.1 [C]'],  mode='lines', name='Zone 1 inline'),  row=1, col=1)
    fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Temp surface Z.3 [C]'], mode='lines', name='Zone 3 surface'), row=1, col=1)
    fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Temp inline OUT [C]'],  mode='lines', name='OUT inline'),     row=1, col=1)
    fig.update_yaxes(title_text="Temperature [C]", range=[-30, 30], row=1, col=1)

    # Plot pressure
    fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Pressure Z.2 [psi]'], mode='lines', name='Zone 2'), row=2, col=1)
    fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Pressure OUT [psi]'], mode='lines', name='OUT'),    row=2, col=1)
    fig.update_yaxes(title_text="Pressure [psi]", row=2, col=1)

    # Set layout
    fig.update_layout(
        template="none",   #  "plotly", "plotly_white", "plotly_dark", "ggplot2", "seaborn", "simple_white", "none"
        title='Temperature and Pressure Time Series',
        showlegend=True
    )

    # Show the plot
    fig.show()



if __name__ == '__main__':

    fname_raw = None #"data/Data_2024_02_21_t09_30_55.csv"
    fname_con = "data/Transduced_data_03_06.csv"

    if fname_raw is not None:
        df = transduce_raw_data(fname_raw)
        df.to_csv(fname_con, float_format='%.3f', index=False)

    df_transduced = pd.read_csv(fname_con)

    print(df_transduced)
    plot_data_plotly(df_transduced)

