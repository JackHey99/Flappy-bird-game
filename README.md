# Finite State Machine
**Jack Heyward**

**ID#2174132**

**Embedded systems**

![image](https://user-images.githubusercontent.com/53545740/69512921-e93a7200-0faa-11ea-8c48-462c1f62e6bb.png)
Figure 1. Format of report and project implementation (Presentation Processes, 2013)

This project followed a four-step waterfall model as seen in figure 1, which this report is based off

# Requirements

The aim of this project is to create an embedded systems-based game using a Teensy, 128x64 OLED display and other selected components. The game must include a home screen, game screen, paused screen and game over screen which must display a score. The program must be based on a finite state machine using the enum, switch and case functions. Libraries such as the Adaftuit_SDD1306 can be used to control the display.

# Design

## Schematics and design


![image](https://user-images.githubusercontent.com/53545740/69512942-fa837e80-0faa-11ea-8c1f-a5a27b8add50.png)
Figure 2. Schematic of component configuration

The display used for this project was the SDD1306 128x64 OLED. This model uses the i2c protocol. The y axis is connected to an analog input and switch of the joystick connected to a digital input with internal pull up resistors activated. The controller used is the Teensy LC which was programmed using visual studio code with the Arduino framework.

### I2C protocol

The Inter-Integrated Circuit (I2C) protocol is a master-slave bus topology network. It is a wired, synchronous half duplex serial communication, which means there is a dedicated clock line to synchronise the two devices and a data line that can send data one way at a time. Each device on the network has its own address which allows the data to be sent only to the relevant devices. See figure 3 for basic representation.

![image](https://user-images.githubusercontent.com/53545740/69512946-feaf9c00-0faa-11ea-9475-24e32b08c345.png)
Figure 3. I2C transmission (DLN ware, 2016)

### Button configuration and Pull up resistors

The microcontrollers default pin mode is the input state. When the controller is in the input state it has high impedance, which means the circuit allows a relatively small amount of current through for the applied voltage. When a button is connected from an input pin to ground, a series current limiting resistor does not need to be added because of the high impedance limiting the current. However, a pullup resistor does need to be added to the line as seen in figure 1. Without a pullup resistor the input line acts as an antenna which picks up the surrounding interference. The interference will cause the pin to float and appear noisy and can cause undesired readings on the microcontroller. To stop the floating a pullup resistor typically between 1K-10K ohms is connected from the input line to the 3.3V pin on the controller (see figure 3). This causes the input line to have a constant 3.3V along it while the button is not pushed which is read as logic 1 by the microcontroller. When the button is pushed the current is sinking through the connection to ground and the voltage drops to 0V which is read as a logic 0 by the microcontroller.

![image](https://user-images.githubusercontent.com/53545740/69512949-02432300-0fab-11ea-8963-2175dc2858ee.png)
Figure 4 Pull up resistor configuration and transmission signal.

When using mechanical buttons as an input for a microcontroller a process called de-bouncing needs to be performed. When a mechanical button is pushed there is always a 'bounce'. The typical reading for a mechanical button being pushed can be seen in figure 7. We cannot remove the bouncing effect however, because we are using a microcontroller, we can add additional code to the program to ignore the bounce. This is done by creating an 'ignore time' which tells the program to ignore any input changes for a set time after the initial change. The ignore time is different for each button however, for the button used in this project an ignore time of 80-120 milliseconds was used.

Figure 5 Button de-bounce waveform. From (Electronics tutorials,2018)

## Libraries

The libraries used were the Adafruit_SSD1306 and Adafruit_GFX. The Adafruit_SSD1306 is used to control the display and has functions such as scroll, fast line and the Adafruit_GFX is used to draw graphics such as lines, circles, rectangles, etc.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image010.jpg)<![endif]>

Figure 6. Adafruit_SSD1306 library

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image012.jpg)<![endif]>

Figure 7. Adafruit_GFX library

To add an image style graphic to the display a bit map needs to be created which the Adafruit_GFX library can deal with. A website called image2cpp was used where a bitmap image is loaded in and a bit array is generated.

URL: https://javl.github.io/image2cpp/

## State transition diagram

Below is the state transition diagram for the proposed game. There are four state, start screen, end screen, game playing, and game paused as outlined in the requirements. There are three events, button push, block hit and score display finished. Not all actions are included however the basic parts of the game are included.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image014.jpg)<![endif]>

Figure 8. State transition diagram for flappy bird game

## Pseudo code

**Start screen:**

<![if !supportLists]>· <![endif]>Display title

<![if !supportLists]>· <![endif]>Prompt user to start game

<![if !supportLists]>· <![endif]>Show small image

**Game playing:**

<![if !supportLists]>· <![endif]>Create blocks

<![if !supportLists]>· <![endif]>Are blocks different size from last ones?

<![if !supportLists]>· <![endif]>Move blocks from right to left

<![if !supportLists]>· <![endif]>Get bird location

<![if !supportLists]>· <![endif]>If bird makes it through blocks increase score

<![if !supportLists]>· <![endif]>If bird hits block end game

**Game paused:**

<![if !supportLists]>· <![endif]>Show paused title

<![if !supportLists]>· <![endif]>Show small image

<![if !supportLists]>· <![endif]>Prompt user to continue game

**Game over:**

<![if !supportLists]>· <![endif]>Show game over title

<![if !supportLists]>· <![endif]>Show score

<![if !supportLists]>· <![endif]>Wait for 5 seconds before going to start screen

# Implementation

## Program

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image016.jpg)<![endif]>

Figure 9 setup

Figure 9 is the setup of the variables and pins, setup of the bit array for the bird image and including of the libraries.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image018.jpg)<![endif]>

Figure 10 more setup and state machine

Figure 10 is the setup of the display address and configuring the input pull up resistor and interrupt.

The void loop contains the state machine where the four states are contained using the switch function.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image020.jpg)<![endif]>

Figure 11. Home screen, final score and game setup

Figure 11 controls the home screen display and final score display. The game is broken into five separate parts, obstacle position, create obstacle, bird position, display current screen and score.

## <![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image022.jpg)<![endif]>

Figure 12. paused screen and game

Figure 12 controls the paused screen and part of the game. The bird position is gathered by reading the potentiometer value and mapping it to the height of the display 64 pixels. A floating average is used to stop the fluctuations in the potentiometer causing the bird to hover up and down. The brick position moves from right to left 2 pixels at a time to give the illusion the frame rate is faster than it is. The original 1 pixel at a time was not fast enough. Each time an obstacle is created it is checked to make sure it is a different size to the previous as the random number generator can create several of the same values in a row.

## <![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image024.jpg)<![endif]>

Figure 13 display the screen

To avoid flickering on the display all the objects on the screen are displayed on the screen at the same time. The screen gets cleared then all graphics are loaded to be displayed using the display.display() function. The score is increase if the bird successfully makes if through the blocks. If the block is hit the game is over and the score is displayed on the end screen.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image025.png)<![endif]>

Figure 14. Button interrupt and debounce

Figure 14 is the interrupt which is triggered by the push of the button on the joystick. A debounce delay of 80 milliseconds in introduced to filter out the button bouncing as mentioned above.

## Difficulties

Some limitations were encountered during this project. The Teensy LC is not as powerful as the 3.2 version causing the frame rates to be considerably slower than the 3.2 version. The scrolling function included in the Adafruit_SDD1306 library also caused a glitch where the 0,0-curser point or origin would start scrolling. To work around this, I manually moved the blocks across the screen which fixed the issue.

Another small issue encountered what the floating of the potentiometer. Using the floating-point average mentioned above solved this issue and the ratios can be adjusted to get a smoother operation depending on the performance of the controller and potentiometer.

# Testing

## Testing

To first get an understanding of how the library works the example program was tested. It runs through all the different functions the library provides. Once an understanding of how the library works was gathered the process of creating the flappy bird game started.

The first step was to create the blocks moving across the screen with different sizes. Once this was tested and working a square box was used as the bird character. This was used to test the ability to navigate through the obstacles and the scoring system. Once the game was fully tested and working the home screen, pause screen, end screen and bit maps were added to give the finishing touches.

### Wave form

Figure 15 below is the SCL (yellow line) and SDA (green line) transmission waveform. This is an interesting issue that arose during testing. The green lines slight increase to 0.5V in the fifth square is an odd issue which I could not fully determine the cause of. My best guess is the hardware limitations of the Teensy LC mention previously or some cross talk between wires and pins switching.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image027.jpg)<![endif]>

Figure 15. SCL and SDA transmission line waveform

### Resistors

The pull up resistor attached to the SCL and SDA lines are identified to be 4.7kΩ. When an additional resistor is added it is in parallel with the onboard resistance reducing the total resistance. This creates a ‘stronger’ connection to the pull up supply rail charging the capacitive line much faster.

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image029.jpg)<![endif]>

Figure 16. Schematic of SDD1306 display (Haoyu electronics, 2019)

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image031.jpg)<![endif]>

Figure 17. Clock without additional pull up resistors

<![if !vml]>![](file:///C:/Users/jackh/AppData/Local/Temp/msohtmlclip1/01/clip_image033.jpg)<![endif]>

Figure 18. Clock with additional pull up resistors

As seen in figure 17 and 18 the clock line with additional pull up resistor’s charges much faster that the line without additional pull up resistors.

## Improvements

There are many improvements that could be made to this project.

The first improvement that could be made is a way to get from the pause screen to the home screen. If the user wants to end the game the must hit a block. Adding a path from the pause screen to the home screen or end screen could make the game more user friendly.

Another improvement that could be added is a way of making the game harder. There could be an easy, medium and hard setting which alters the speed of the blocks moving.

The code could me made more streamline. Some parts of the code such as checking if the block sizes are different loops around until a value that satisfies the criteria is meet. This slows down the program execution and display response.

An improved controller such as the Teensy 3.2 could also allow for better user feel.

Lastly splitting the game so the state machine is in one program and the game is in another file with a header file could make the code more user-friendly to read if future improvements or share was going to happen.

## Conclusion

The aim of this project was to create an embedded systems-based game using a Teensy, 128x64 OLED display and other selected components. The game had to include a home screen, game screen, paused screen and game over screen which must display a score. The program had to be based on a finite state machine using the enum, switch and case functions. Libraries such as the Adaftuit_SDD1306 could be used to control the display. The final game was biased on the flappy bird game, the system used a joystick to control the position of the bird and the built-in button was used to control going between states. There are some improvements that could be made however, the current program is a working prototype that has been designed to gain an understanding of how finite state machines work.

# References

DLN ware. (2016). _I2C Bus_ . Retrieved from DLN ware: http://dlnware.com/i2c

Haoyu electronics. (2019). _SSD1306 0.96" 128×64 OLED Display – I2C/SPI Interface_. Retrieved from Haoyu electronics: https://www.hotmcu.com/ssd1306-096-128×64-oled-display-–-i2cspi-interface-p-144.html

Presentation Process. (2013, October 25). _Waterfall model_. Retrieved from Youtube: [https://www.youtube.com/watch?v=_ZKvvaZEFKE](https://www.youtube.com/watch?v=_ZKvvaZEFKE)

Electronic Tutorials. (2018, April 25). Input Interfacing Circuits Connect to the Real World. Retrieved from [https://www.electronics-tutorials.ws/io/input-interfacing-circuits.html](https://www.electronics-tutorials.ws/io/input-interfacing-circuits.html)
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTMyMzE0Nzg2NywxNTk3NTQyNDE3XX0=
-->