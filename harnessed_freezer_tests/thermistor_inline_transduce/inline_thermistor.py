from scipy import interpolate

class InlineThermistor():
    """
    Class representing an inline thermistor

    T [C], Res [kOhm]
    -40, 209.6
    -30, 120.4
    -20, 71.67
    -10, 44.09
    0,   27.94
    10,  18.19
    20,  12.14
    30,  8.285
    40,  5.775
    50,  4.103
    60,  2.968
    70,  2.182
    80,  1.629
    90,  1.234
    100, 0.947
    """

    def __init__(self, v_in=5.0, res_series=10000, adc_bits=14):

        self.v_in       = v_in
        self.res_series = res_series
        self.adc_bits   = adc_bits

        self.res_vals   = [ 947.0, 1234, 1629, 2182, 2968, 4103, 5775, 8285, 12140, 18190, 27940, 44090, 71670, 120400, 209600]
        self.temp_vals  = [   100,   90,   80,   70,   60,   50,   40,   30,    20,    10,     0,   -10,   -20,    -30,    -40]

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
