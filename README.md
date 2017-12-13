# GPS Tracking
The goal of this project was to collect GPS data on one board, communicate that information across a large distance and then recieve it on another board to understand where the transmitter board is located.

## Parts
- 2x Arduino Uno
- 2x Adafruit RFM95W LoRa Radio 
- Connection Wires
- 2x breadboard (one for each module)
- 1x GPS Receiver - GP-20U7 (Sparkfun)

## Overview
The GPS unit communicates with Arduino 1 via UART using software serial. This information is parsed and then the location information is sent to the LoRa board via SPI. The other LoRa board recieves the information, communicates it to Arduino 2 (Again SPI) and the the information is sent over the arduino serial port to a computer. A python script on the computer reads the location information and generates a graph of the current location, a trail indicating where the device has moved from, and an optional heatmap from all previous times the devices were connected. Every location data point is saved in a CSV file on the computer.

## Note
![alt text](https://github.com/melonisj/gps_tracking/blob/master/IMG_20171212_130548.jpg)
The attached image shows the setup connected to a logic analyzer. This useful logic analyzer called the Logic 8 from Saleae (https://www.saleae.com) was very useful for debugging problems with the SPI connection between Arduino and Lora Adafruit RFM95W LoRa Radio. Turns out one of the primary SPI pins on my Arduino was broken which I likely would not have figured out without this analyzer


