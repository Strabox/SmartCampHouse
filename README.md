# SmartCampHouse [![Build Status](https://travis-ci.org/Strabox/SmartCampHouse.svg?branch=master)](https://travis-ci.org/Strabox/SmartCampHouse)

This project consist in developing a remote system to manage a water tank and field irrigation using the Arduino and Android platforms plus a few common sensors and devices.

## Works Stages

* **Current:** For now there is only one Arduino that controls all the system in one place and the electric instalation provides the range of actuation.
  - **Current:** Developing the code and testing the system in "laboratory".
  - **Future:** Deploy the system in the real world and evaluate it.
* **Future:** Use two or more Arduinos to cooperate in an ad-hoc network to distribute the load and bring the Arduinos near the respective actuators and sensor. This will be good to fault tolerance and possibility to add arduinos in places far away powered by batteries. I am thinking in use Zigbee boards to build the network.

## Libraries Used

The project is powered by:
* [Easyuino](https://github.com/Strabox/Easyuino) that I have developed too where I hide the complexity of each sensor/device/module to write simple and objective code in this project.
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson) to serialize data from the Android application to the arduino and vice-versa.
