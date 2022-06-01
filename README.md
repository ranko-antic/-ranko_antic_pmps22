# STM32F407xx - HAL API Project
Design of microprocessor systems / Faculty of Electrical Engineering (University of Tuzla)

## Specification
### Components:

  1. STM32F407VG Discovery Board	
  2. Breadboard
  3. DHT11 Sensor
  4. 1 Two-Channel Relay
  5. Fan (Ventilator)
  6. Lightbulb (Heater)
  7. Battery -> 9 [V]
  8. LCD1602 Module
  9. 2 Push-Buttons
  10. Resistors 220 Ω & 1 kΩ
  11. Conductors – Connecting Cables

### Functionality

The system consists of a DHT11 sensor (digital temperature and humidity sensor), which is used to read the value of the current room temperature that is printed on the component -> LCD display. Above the printed value of the current room temperature is the desired temperature (with the initial value set to 20°C) whose value can be modified, ie. regulated by 2 push-buttons. The while(1) loop begins with sending the start signal to DHT11, after which the response received from the DHT11 sensor is recorded, which indicates the presence of the sensor. If DHT11 is present, it will send 40 bits of data which includes integral RH data, decimal RH data, integral temperature data, decimal temperature data and the checksum. This means that a total of 5 bytes will be read from the sensor. Since the current temperature value and the desired value are printed on the lcd, the humidity value is ignored. Temperature data is from the integral part, which is immediately converted to float and then calls the display function to print desired and current temperature on the lcd screen. Using the "set_TEMP" function, the read pressures on one of the two push buttons result in the desired temperature value being incremented or decremented by 1. And within the remaining function called "check_TEMP", based on the relationship between the desired value and the current temperature, it is necessary to turn on the heater or fan, depending on the results of the comparison.

