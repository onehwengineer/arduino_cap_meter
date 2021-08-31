## Arduino Capacitance Meter

### arduino_cap_meter
Displays capacitance readings

### arduino_cap_meter_to_perc
Displays capacitance and corresponding % values. <br>
Based on linear projection of cap to moisture %. <br>
Formula : <br>
Moisture Percent = 99%/(MAX_CAP - MIN_CAP) x (cap_measured - MIN_CAP)
