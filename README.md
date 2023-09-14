# ITI_Embedded_Training_Graduation_Project


# System Description
![Screenshot 2023-09-14 130402](https://github.com/MahmoudSamy511/ITI_Embedded_Final_Project/assets/95527475/6d5a979b-12ff-4702-b272-68b596c5447a)


## MCU1

### Components
- Push Button
- Temperature Sensor
- DC Motor (as a fan)

### Description
1. **MCU1** reads the state from **EEPROM** before reading the temperature to act accordingly as an abnormal state or not.
2. **MCU1** reads the temperature and stores the state as normal initially in **EEPROM**, then moves the fan (DC motor) with an acceptable speed according to states and sends the temperature to **MCU2** using UART.
3. If the user presses the push button and the temperature is between 40°C and 50°C, **MCU1** sends a code to **MCU2** as a signal to shut down the machine using UART.

## MCU2

### Components
- Servo Motor
- 3 LEDs (Green, Yellow, Red)
- Potentiometer
- DC Motor (as a machine)
- Buzzer

### Description
1. **MCU2** receives messages using UART from **MCU1** and acts according to them.
2. The potentiometer controls the speed of the machine (DC motor).

### States

#### Normal State
- If the temperature is lower than 20°C, **MCU1** will stop the fan, and **MCU2** will light up the **green LED**.
- Between 20°C and 40°C, the Fan in **MCU1** is moved with speed increasing according to the increasing temperature, which has a maximum speed at 40°C, and **MCU2** lights up the **yellow LED**.
- Between 40°C and 50°C, **MCU1** moves the fan with maximum speed, and **MCU2** lights up the **red LED** and stops the machine by pressing the push button.
- If the temperature is greater than 50°C, both **MCU1** and **MCU2** enter into an emergency state.

#### Emergency State
- Begin counting time.
- **MCU1** moves the fan with maximum speed.
- **MCU2** lights up the **red LED** and activates the buzzer.

#### Abnormal State
- **MCU1** moves the fan with maximum speed.
- **MCU2** lights up the **red LED**, opens the servo with 90 degrees, stops the machine, and activates the buzzer.

If the emergency state is activated for 7 seconds (you can use the timer with overflow mode to check the temperature 14 times with a period of 500ms), follow these steps:
1. Write the abnormal state in **EEPROM**.
2. **MCU1** activates the watchdog timer with the smallest time to reset **MCU1**.
3. **MCU2** monitors the abnormal condition for 5 seconds.

# Schematic
![image](https://github.com/MahmoudSamy511/ITI_Embedded_Final_Project/assets/95527475/743a56a1-4cb8-47ae-9f9b-a15e78d3d7f3)

