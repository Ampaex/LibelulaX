![Logo](Misc/libelulaLogo.png)

# Abstract

This **end of master project** aims to create a **microcontroller-based autopilot** as an effective and low-cost solution for the fully automatic control of UAV type aircraft or unmanned aerial vehicles. Within the current market, solutions can be found that use a complex and **expensive** SoC type processor. Thus, this project seeks to implement its own solution using a **ESP32** type microcontroller with dual core, Bluetooth and Wi-Fi connectivity, much cheaper but without losing possibilities, with the reliability of an operating system like as **FreeRTOS**.
The final system have an interface providing a linux styled **command terminal** for communicating multiple commands and allowing communication for a looped simulation using the open source **FlightGear** simulator program, communicating with real hardware ***via USB**.
Finally, we will obtain a small-scale radio-controlled aircraft, being able to follow a **GPS route** and return again to the point to which it was launched, waiting to resume manual control for landing.
Everyting being computed at real-time by hardware and running on a **simulated environment**.
