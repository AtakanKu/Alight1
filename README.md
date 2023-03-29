# Alight1
A project for a home lighting system inspired by an artificial life algorithm.

The code was written on Arduino IDE 2.0.4.

## Hardware:
1) Arduino Uno R3
2) 8x32 LED dot matrix, created by four 1088AS chips on a breakout board.
3) Grove temperature sensor board(DHT11).

Connections(See list above for hardware numbers):  
|From:           |To:
|---------------:|-----------|
|(3)(pin GND)    |(1)(pin GND)|  
|(3)(pin SIG)    |(1)(pin 2)(Digital)|  
|(3)(pin VCC)    |(1)(pin Vin)(5V)|  
|||
|(2)VCC        |(1)(pin 5V)|  
|(2)GND        |(1)(pin GND)|  
|(2)DIN        |(1)(pin 11)(Digital)|  
|(2)CS         |(1)(pin 10)(Digital)|  
|(2)Clock      |(1)(pin 13)(Digital)|  

All connections by jumper wires.
