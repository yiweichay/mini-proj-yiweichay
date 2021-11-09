# Individual project - Energy saving automatic outside light

## Learning outcomes

The principal learning objectives for this project are:

- Implement a working microprocessor based system to achieve a more complex real world task
- Develop your ability to independently plan, organise and structure your code 
- Improve your grasp of the C language and writing your own functions

## Brief

Outside lights that respond to ambient light levels are commonplace (i.e. street lights, porch lights, garden lights etc). These types of lights switch on at dusk and then turn off at dawn. However, energy and money can be saved if these lights are switched off during the small hours of the morning (for example, between 1am and 5am), when there are very few people around. Many councils in the UK have implemented/trialled this idea for street lights (https://www.bbc.co.uk/news/uk-england-derbyshire-16811386). Your task is to use the knowledge of microcontrollers and hardware that you have gained in labs 1-3 from this module to develop a fully automated solution.

## Specification
Design and program a device that meets the following requirements:

1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
1. Displays the current hour of day on the LED array in binary
1. Turns the light off between approx. 1am and 5am
1. Adjusts for daylight savings time
1. Maintain synchronicity with the sun indefinitely
1. Be fully automatic (requires zero maintenance after installation)

Please use this GitHub repo to manage your software development and submit your individual project code.

## Supplementary information and help
At first the task may seem quite straightforward but there are several points that often prove more tricky. The first is how to test code during development? You could test in real world conditions but you would be limited to one test cycle per day and this would severely slow down your development and debugging progress. To get around this you could implement a "testing mode" and pretend that a day lasts 24 seconds. This could be done using a #define directive to switch between "normal" and "testing" modes for your code.

Adjusting for daylight savings time is not too tricky. The clocks always change (in the UK) on the last Sunday in March (they go forward an hour) and the last Sunday in October (they go back an hour). One method of achieving this is to initialise what day it is when device is first switched on (using manual input) and then keep track of the days that pass and what the day of the week it is. Another method might be to automatically figure out what time of year it is (see below). Also don't forget about leap years! 

No clock is perfect, they can all run slightly fast/slow and can by influenced be external factors such as temperature. Ultimately this will result in drift over time and eventually the time will drift so far out of sync with real time that it is meaningless. For the purposes of our device the main requirement is that it remains in sync with the sun. You could use light from the sun to keep your clock in sync. Although the length of daylight varies considerably during the year, the midpoint between dusk and dawn only varies by a few minutes. This is termed solar midnight approx. 12am or solar noon approx. 12pm. One method of staying in sync with the sun is to use the LDR and record/calculate when these times occur and adjust your clock accordingly. The length of daylight also tells us information about what time of year it is and can be used to help us know when to adjust for daylight savings time.

![Day length](gifs/day-length-london.jpg)
http://wordpress.mrreid.org/2010/10/31/why-change-the-clocks/

## Code is operated as follows:

1. Initialisation
Set the current date in the code and initialise the current hour of the day using RF2 button. Once you've got the current hour of the day, start the clock by hitting on button RF3 once. The LED array flashes a few times and the clock starts. 

2. Monitoring light level with LDR
In led.c, the set_led() function monitors the light level and turns on LED RH3 when it is dark and vice versa when it is light.

3. Displaying the current hour of the day
Timer is used to keep track of the time of the day. The hour is displayed on the LED array.

4. Turning light off between 1-5am
An interrupt was used to ensure that the LED (RH3) turns off regardless of the light level between 1-5am.

5. Adjusting for daylight saving time
A new file day.c was used to store all the functions that were used to adjust for daylight saving time. Functions were used to set the current day, date, month and year. To source for the last Sunday of the month, the function calculateDaysToTarget calculates the numnber of days until 1st of March or 1st of October. The function dateOfLastSunday calculates the number of days to the first Sunday in the target month (march/oct) and increment count by 7 to find the last Sunday of the month. Daylight saving on will take place at 1am on the last sunday of March while daylight saving off takes place at 2am on the last Sunday of October. 

6. Remaining synchronicity
For the first loop (first day of the clock starting), the actual noon is initialised at 12pm in the code. The code checks the actual noon everyday by storing the time at which dusk and dawn takes place. The midday value is then calculated. If the midday value does not match the solar noon (set at 12pm), the clock will move forwards/backwards the next day, adjusting and remaining synchronicity. 

## How to test the code?

1. The timer prescaler is lowered to ensure that the time moves faster for me to test my code.
2. On the LCD screen, the first variable shown is the date at which the next daylight saving will take place. This is to check and ensure whether the date in which daylight saving takes place is correct. 
3. On the second line of the LCD, the variable next to the time is the calculated midday value. This is to check whether the clock moves forwards/backwards when the time reaches the midday value and hence adjust for synchronicity. 








