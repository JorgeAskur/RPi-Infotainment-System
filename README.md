# Infotainment System with Raspbery Pi and STM32F103C8T6

This project was developed for the System on a chip Design class in colaboration with Intel at Tecnológico de Monterrey and it should resemble a infotainment system present in a car. 

The system is controlled by various buttons connected to the STM32 and by the touch screen of a tablet connected via VNC to the RPi 4, which uses the pygame library to play songs. It can also download additional songs thanks to the pytube library, which also gives us the data of the song which is displayed in the GUI as well as the thumbnail of the song which is obtained with urllib. Finally the name of the song and its duration are displayed in a OLED screen connected to the STM32F103C8T6.

## Electronic Schematic 

<p align="center">
  <img src="https://user-images.githubusercontent.com/74482029/207171437-e8e5d40c-1e32-4916-9251-ebea46f7181c.png">
</p>

In his project we used:
* UART communication
* GPIO ports
* I2C communication
* GUI design with Tkinter
* C and Python programming 
* Various libraries to download and play songs
* VNC connection

## List of materials
* Raspberry Pi 4
* STM32F103C8T6
* 128 x 64 OLED screen
* Buttons
* TTL - UART cable
* Various resistors
* Jumpers
* Tablet

## Evidences
<p align="center">
  <img width="320" height="240" src="https://user-images.githubusercontent.com/74482029/207173000-edfc9b34-6a42-4337-9931-723ac7b0eb64.png">
  <img width="320" height="240" src="https://user-images.githubusercontent.com/74482029/207173276-771b4736-2d52-48be-b4a4-c35c8c8c8823.png">
</p>

## Team Members
* Jorge Askur Vazquez Fernandez
* Sebastian Joya Paez
* Jose Miguel Flores Gonzalez


