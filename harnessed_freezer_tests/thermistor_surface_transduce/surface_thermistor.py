from scipy import interpolate

class SurfaceThermistor():
    """
    Class representing a surface thermistor

    T [C], Res [Ohm]
    -40,336500,
    -35,242590,
    -30,177000,
    -25,130370,
    -20,97070,
    -15,72929,
    -10,55330,
    -5,42315,
    0,32650,
    5,25388,
    10,19900,
    15,15708,
    20,12490,
    25,10000,
    30,8057.0,
    35,6531.3,
    40,5327.0,
    45,4368.7,
    50,3603.0,
    55,2986.2,
    60,2488.0,
    65,2083.0,
    70,1752.0,
    75,1481.4,
    80,1258.0,
    85,1072.3,
    90,917.70,
    95,788.52,
    100,680.00
    """

    def __init__(self, v_in=5.0, res_series=10000, adc_bits=14):

        self.v_in       = v_in
        self.res_series = res_series
        self.adc_bits   = adc_bits

        self.res_vals   = [680.0, 788.52, 917.7, 1072.3, 1258.0, 1481.4, 1752.0, 2083.0, 2488.0, 2986.2, 3603.0, 4368.7, 5327.0, 6531.3, 8057.0, 10000, 12490, 15708, 19900, 25388, 32650, 42315, 55330, 72929, 97070, 130370, 177000, 242590, 336500]
        self.temp_vals  = [100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0, -5, -10, -15, -20, -25, -30, -35, -40]

        # See: https://stackoverflow.com/questions/31543775/how-to-perform-cubic-spline-interpolation-in-python
        self.res_to_temp_interp = interpolate.splrep( self.res_vals, self.temp_vals )

    def analog_to_res(self, analog_int):
        v_measured = self.v_in * analog_int / ((2**self.adc_bits) - 1)
        res_therm  = self.res_series * ((self.v_in / v_measured) - 1)
        return res_therm

    def analog_to_temp(self, analog_int):
        """
        Return temperature from given analog_int measurement

        Inputs:
            analog_int: non-negative integer reported from ADC
        Returns:
            temp_measured: deg C
        """
        res_therm = self.analog_to_res(analog_int)
        temp_measured = interpolate.splev(res_therm, self.res_to_temp_interp)
        return temp_measured
