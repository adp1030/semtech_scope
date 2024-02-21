# Thermistor temperature measurement

We purchased TDK B57861E0103A010 NTC thermistors. These have a rated temperature of 25 C and a nominal resistance of 10k Ohm at this temp. The resistance of the thermistor decreases with warmer temperatures.

Datasheet: https://product.tdk.com/system/files/dam/doc/product/sensor/ntc/ntc_assy/data_sheet/50/db/ntc/ntc_probe_ass_e861.pdf

Their resistance very rapidly increases below roughly -15 C, which could pose some difficulties for us.

We plan to transduce the resistance to a measureable voltage via a simple voltage divider (i.e. a resistor in series with the two thermistor leads to 5V and GND.) A reasonable value for this will probably be 10k Ohm. Note that the smaller this series resistance, the larger the fluctuations in voltage drop we can measure across the resistor, but also the higher the current through the thermistor, which causes increased self-heating. This effect shouldn't be too problematic for us (given that our water-filled copper pipes have very substantial heat capacity), but it's worth keeping in mind.

The datasheet offers a detailed temperature vs. resistance table. However, we want a general formula so we can have our firmware convert arbitrary measurements to temperatures.

## Exponential R-to-T approximation

The general technical information datasheet (https://www.tdk-electronics.tdk.com/download/531116/19643b7ea798d7c4670141a88cd993f9/pdf-general-technical-information.pdf) offers a rough approximation to this curve in Section 3.1.1 (formula 1).

This is: 
```
generally:
res(T) = R_rated * exp(B*((1/(T+273.15))-(1/(T_rated+273.15))))
for ours:
res(T) = 10000 * exp(3988*((1/(T+273.15))-(1/(25+273.15))))
```

However, the error in this approximation definitely acculumates outside of the 25 C range.

## More nuanced R-to-T calculation

There is a more nuanced transformation for intermediate values given here:

https://www.tdk-electronics.tdk.com/download/531128/71487ad65dd78f1844cf52efcee5836d/pdf-standardizedrt.pdf

Which uses alpha values obtained here (our sensor has R/T characteristic curve 8016):

https://www.tdk-electronics.tdk.com/inf/50/db/ntc/NTC_Mini_sensors_S861.pdf

This appears to give an exponential curve for each interval between the values given in the table. A simple piecewise linear interpolation may suffice for our purposes.

## Temperature vs. resistance table

As given in [datasheet](https://product.tdk.com/system/files/dam/doc/product/sensor/ntc/ntc_assy/data_sheet/50/db/ntc/ntc_probe_ass_e861.pdf)
```
Temp[deg C],R_Nom[Ohm],R_Min[Ohm],R_Max[Ohm],∆R[±%]
-40,336500,317050,355950,5.8
-35,242590,229440,255740,5.4
-30,177000,168020,185980,5.1
-25,130370,124180,136560,4.7
-20,97070,92772,101370,4.4
-15,72929,69923,75936,4.1
-10,55330,53211,57449,3.8
-5,42315,40814,43816,3.5
0,32650,31581,33719,3.3
5,25388,24623,26152,3.0
10,19900,19351,20449,2.8
15,15708,15313,16103,2.5
20,12490,12205,12775,2.3
25,10000,9800.0,10200,2.0
30,8057.0,7874.1,8239.9,2.3
35,6531.3,6369.1,6693.6,2.5
40,5327.0,5183.7,5470.3,2.7
45,4368.7,4242.4,4495.0,2.9
50,3603.0,3491.9,3714.1,3.1
55,2986.2,2888.5,3084.0,3.3
60,2488.0,2402.0,2574.0,3.5
65,2083.0,2007.4,2158.7,3.6
70,1752.0,1685.4,1818.6,3.8
75,1481.4,1422.5,1540.2,4.0
80,1258.0,1206.0,1310.0,4.1
85,1072.3,1026.3,1118.4,4.3
90,917.70,876.92,958.48,4.4
95,788.52,752.30,824.74,4.6
100,680.00,647.78,712.22,4.7
105,588.59,559.87,617.31,4.9
110,511.20,485.55,536.85,5.0
115,445.41,422.46,468.35,5.2
120,389.30,368.74,409.86,5.3
125,341.70,323.22,360.18,5.4
130,300.90,284.25,317.55,5.5
135,265.44,250.43,280.45,5.7
140,234.80,221.24,248.36,5.8
145,208.32,196.05,220.59,5.9
150,185.30,174.18,196.42,6.0
155,165.35,155.24,175.45,6.1
```