# PokeCan

## Objectives
Pokecan is a trash can that can automatically detect the level of trash inside itself, and if it is full, it will walk along the path that is set by user and will dump the trash into a larger trash can. After it dumps all trash out, the Pokecan will walk back to its original location.

## Hardware implementation
1. Motors

* There are totally six motors.
* Two adhered to the base by hot melt glue gun to control the ups and downs
* Two are bolted on the tubes by screws to control dumping.
* And two are under the base to move Pokecan around.

2. Wheels

* The two of the wheels that we use are plastic wheels that printed by 3D printer. To strengthen the friction
for the purpose of no slipping, we use sandpapering first and attach them to the rubber by resin.
* And the others, we utilize the steering wheels to change the direction with a low additional friction.
* In detail, as we put the wheels to the base, the degree of tightening screws is also a problem. If looser,
they will be idle running. If tighter, the wheels will be broken since the plastic printed out was not strong
enough.

3. Plastic tubes

* There are two plastic tubes on the left and right sides. And inside the tubes are two pulleys bolted on the
up and down sides. We use a long line combined with two pulleys and connected with the motor below.
* To be true, the tubes are the most important ones that make our mechanical components stable. If they
aren’t “perfectly” parallel, the motors cannot strike a balance. This means that the controller IC will be
overloaded and things will breakdown (we burned down half side of a MCP23017!). Then we will be
freaked out because we need to disconnect all the things and “reconstruct” it.

4. Sensors

* In this project, we use three different sensors.
* The first is ultrasonic distance sensor. The function of this sensor is to detect height, so that the system will
know whether the trash can is already full or not.
* The second sensor is the Hall effect sensor. The Hall effect sensors’ functions are to detect whether the
movement of one side is faster than the another side or not and to determine the distance Pokecan has
moved.
* The other one is a micro switch, but we used it like a sensor. It take the responsibility of checking the
height of the trash can. There are totally two on the up and down side. When the trash can touched the
upper sensor, it will stop and dump. On the other hand, when it touched the lower one, it will also stop
and turn to the moving mode to the original location.

5. Battery Pack

* We thought that making a rechargeable Li-ion battery controller is cool, thus we checked out some circuits
for charging a Li-ion battery, doing switching/load sharing between DC input/Battery power, and boosting
the voltage of the battery pack (3.7V) to 5V which the MCU needs. Then we designed our own circuit. The
main components are:
    i. Microchip MCP73833 charge management IC: to charge and protect the Li-ion battery.
    ii. TI LM2621 switching voltage boost IC: to boost the voltage from 3.7V to 5V.
    iii. P-Channel MOSFET: when an external power supply is provided, the power source of main system
will switch to external supply by this PMOS.

* Then we got the PCB fabricated in 10pcs and ordered the components we needed. Finally the board is
completed and it can really act as a USB battery pack to charge a phone! However it can only draw about
1A of current due to the restrictions of LM2621 and the inductor. It’s okay to use STM32F401 MCU with
this battery pack; however Raspberry Pi draws about 2.5A so in the end we used a battery pack we bought
online instead of the one we made ourselves.
* For the motors, we have 4 motors using 6V and 2 motors using 12V. Thus we have 3 non-rechargeable
packs, each with 4 slots for 1.5V AA battery.

6. STM32F401

* This was the microcontroller unit we’ve used. It’s based on ARM’s Cortex M4 CPU architecture. All
programs in it are written in C/C++. However just 5 days before the demo, the MCU died due to some
shorted circuit. Thus we switched our platform to Raspberry Pi.

7. Raspberry Pi

* Despite its tiny size, it has full capabilities just like a desktop computer. We use it to control all other
hardware components of Pokecan. The operating system running inside is Raspbian, which is based on
Debian, a Linux distribution.

8. ICs

* L293D: This is a motor driver IC from STMicroelectronics/Texas Instruments. It takes 3 inputs: one to
control the voltage by PWM signal, and the other 2 one to control the direction of the current. With these
combined, we can easily control directions and speeds of the motors.
* PCA9685: It’s a 16 channel PWM output IC from NXP. I2C bus is used to control it. By sending a PWM pulse
signal to L293D, we can control the voltage output.
* MCP23008/MCP23017: It’s a simple input/output expander using I2C from Microchip. MCP23008 has 8
channels and MCP23017 has 16 channels. Since we need many inputs/outputs, using these can avoid
having many wires between Raspberry Pi and other components. For example, we have soldered a board
with 4 L293D and a MCP23017 on it. Then we just need 5 wires (3V3, 5V, GND, SCL, SDA) instead of dozens.
Unfortunately, half side of our MCP23017 has burned out, thus we utilized the unused channels in
PCA9685 as a simple output signal by setting the PWM duty cycle to 100%.
* US1881: This is a Hall effect sensor from Melexis. Its output will change when the polarity of the magnetic
field around it changes. It takes a 5V input so we put a voltage divider to make the output voltage to 3V3.
However we found out that the output was an open drain (floating/connected to ground) so just a pullup
resistor instead of a voltage divider is needed.
* SSD1306: It’s an OLED display controller using I2C/SPI (configurable) from Solomon Systech.
* HC-SR04: This is an ultrasonic distance sensor. The output is 5V so a voltage divider is needed. It will
output a pulse signal so by multiplying the signal length by the speed of sound wave we get the distance
from the sensor to the object in front of it. It’s interesting that we didn’t found out the name of the
manufacturer of this IC/module!
