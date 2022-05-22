# laser-security-project
A school project model built to emphasis on our day to day security. In this model if anyone interrupts the laser then a picture of the intruder is taken and sent to a heroku flask server, which with the help of flask mail sends a mail to the owner with the picture of the intruder. This repo is a resource storage of our original physical project model made to serve the purpose of recreating the original model in future or improving the same model. This repo is not meant to be updated any time soon so don't expect any elaborate support from us, but contributions and improvements from your side are welcome.
## The Hardware
### Materials required (For prototype)
1. A lot of jumper wires M-M M-F F-F
2. ESP32_CAM AiThinker Board with OV264
3. FT232RL USB to UART TTL
4. Buzzer module
5. Laser module
6. LM393 LDR module
7. A power supply (5v) if required
8. Breadboard
9. Pushbutton
10. Resistors 1k ohm
### The schematic
![Schematic_laser security system](https://user-images.githubusercontent.com/49360491/169693924-dbb8443f-4d83-4e35-9fcd-265c09d2c238.png)
For higher resolution check the hardware folder   
Upload the esp32_cam.ino to the board after updating all the dummy variables(wssid, wpass, server_url) and make the connections properly with the wifi.
## The Server
The server is ready to go. Just add some env variables, enable DisplayCaptcha and Allow less secure apps to your sender gmail account and you are good to deploy
### Deploy
Environmental variables
```
RECIPIENT_EMAIL receiver@gmail.com
SENDER_MAIL sender@gmail.com
SENDER_PASS anything1234
```
For heroku Buildpacks required
```
heroku/python
```
```
https://github.com/heroku/heroku-buildpack-apt
```
now just deploy.
# Warnings
Any soft damage can be fixed but not hard. This project contains hardware elements and any damage to any hardware or your assets due to any misconnections will not be any of our responsibility. Only recreate this if you know what you are doing. Also to be safe try testing the circuit with 3.3v before testing it with 5v.
