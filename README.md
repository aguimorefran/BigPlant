# BigPlant

The project came simply by staring at my plant and thinking, how could I get rid of checking everyday the soil of my plant and water it? and I thought yeah, I'll build something.

## The main idea
To check everyday the soil moisture of the pot and water it depending on this moisture. I also wanted it to water only at sunset so it is only watered when the light intensity is below a threshold value. Furthermore, I wanted to see the values in real time and keep them safe so in a near future I could use it to improve the system, so I added a ESP-01 module to upload the data.


## Hardware used
- Arduino Nano chinese clone. As the brains of the project.
- Analog moisture sensor. It reads the moisture level of the pot depending on the amount of water present on it. I used an analog sensor capacitive sensor of good quality to prevent rust and get more precise values. This is the one used: https://www.dfrobot.com/product-1385.html
- LDR. Simple photoresistor to measure light intensity.
- DHT22. Digital sensor to measure air temp and air humidity.
- RGB led to check quickly how the state of the machine is. I used RED for dry soil moist, BLUE for medium soil moist and GREEN for good soil moist. Also, when uploading data or in watering status, use another colors.
- 12V water pump. No need for explanation.
- Relay module. To avoid using transistors. Just a simple relay.
- ESP-01. For uploading the data.

## ThingSpeak
After hours of hours of reviewing IoT platforms I made up my mind with ThingSpeak. It's very simple to use and to upload the data. Also you can use MatLab to contrast data, make analysis and cool graphs. Who doesn't love graphs?

This is the TS page for this project: https://thingspeak.com/channels/440044


### TODO
Implement a basic web page with the ESP-01 to interact with and build a basic app to send and receive the data directly on the phone.
