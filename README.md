# SMART-Response-XE-Arduino-Basic-Port

This is a port of Robin Edwards Arduino Basic for the SMART Response XE.
In addition to his functionality I have added:
1) Batt: an operator that returns the voltage of the battery in mV (print batt, a=batt)
2) MSAVE/MLOAD: Saves current program to memory locations in external memory. There are 10 locations (0-9)
   These commands return the number of bytes stored or retrieved. (msave 6)
3) Bye: puts the unit into sleep mode. Current, 5mA operating, 30uA in sleep.
4) ATN: the c function atan to round out the trig functions.
