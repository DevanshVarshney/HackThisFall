# HackThisFall
This project is for the Government or orgainsation whose security personels or employees works under the conditions in which there is always a risk associated with their life. So for this we are constantly monitoring the vitals of them and if any abnormal change occurs in thier vitals then the authority who is monitoring will get notified with their GPS location.
And in this project we also show some personal details of them like which department they belongs to. And this system can also works when their is no connecticity to the internet like for soldiers in the mountain or in the areas where there is no internet.
In the HackThisFall_Final.ino we haven't interfaced the MAX30100 sensor from esp32 to raspberrypi via mqtt because of timing issue.
HackThisFall_Final.ino is the final file which interfaces the gps, temperature sensor with ESP32 and then send data to the raspberrypi through mqtt.
oximeter_I2CLCD.ino is the file which only integerates MAX30100 with LCD and Arduino Uno thorugh I2C.
And all other final files are just part or testing of some sensor and part of other programmes.
Thank You
