🌱 Ergonomic Compost Maker

An Arduino-based Ergonomic Compost Maker designed to monitor soil nutrient levels (NPK) along with temperature and humidity to support efficient and healthy composting.
This system helps in understanding compost quality in real time, making the composting process smarter, easier, and more ergonomic.

🔧 Project Overview

The system uses:

An RS485 Modbus Soil NPK sensor to measure Nitrogen, Phosphorus, and Potassium

A DHT22 sensor to monitor temperature and humidity

Two I2C LCD displays for clear and real-time visualization

All sensor data is processed using an Arduino Uno and refreshed automatically at fixed intervals.

🌡️ Parameters Monitored

Nitrogen (N) – mg/kg

Phosphorus (P) – mg/kg

Potassium (K) – mg/kg

Temperature – °C

Humidity – %

🧰 Hardware Used

Arduino Uno

Soil NPK Sensor (RS485, Modbus)

MAX485 RS485 Module

DHT22 Temperature & Humidity Sensor

16×2 I2C LCD (NPK Display)

16×2 I2C LCD (Temperature & Humidity Display)

Breadboard and Jumper Wires

12V DC Power Supply (for NPK sensor)

🖥️ Display Output

LCD 1 shows: Soil N, P, K values

LCD 2 shows: Temperature and Humidity

Data updates every 2 seconds for continuous monitoring.

🌾 Applications

Ergonomic compost monitoring

Smart composting systems

Organic waste management

Smart agriculture projects

Academic and IoT-based learning projects

🚀 Future Scope

WiFi-based data monitoring

Mobile application integration

Cloud data logging

Compost quality prediction
