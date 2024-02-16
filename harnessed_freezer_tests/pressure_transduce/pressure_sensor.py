class PressureSensor():
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

    def __init__(self, v_in=5.0, adc_bits=14):

        self.v_in       = v_in
        self.adc_bits   = adc_bits

    def analog_to_pressure(self, analog_int):
        """
        Return temperature from given analog_int measurement

        Inputs:
            analog_int: non-negative integer reported from ADC
        Returns:
            temp_measured: deg C
        """
        v_measured = self.v_in * analog_int / ((2**self.adc_bits) - 1)
        pressure = (v_measured/5.0 - 0.1)/0.6666667    # from datasheet, in MPa
        return pressure
