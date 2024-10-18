### What is it
This repository contains example code to integrate Seeed Respeaker Lite Voice Kit (with XIOA ESP32-S3 on board) with ESPHome.
I'm thankful for continuous help of ESPHome team and Seeed developers, that let me put this thing together. 
It's using [brilliant job of ESPHome team](https://github.com/esphome/home-assistant-voice-pe), as well as firmware and guides from [Seeed Wiki](https://wiki.seeedstudio.com/xiao_respeaker/) and [their repository for Respeaker](https://github.com/respeaker/ReSpeaker_Lite/tree/master).

### *Disclaimer
#### Use this on your own risk. Expect breaking changes.
At the moment, all this is changing at very fast pace. Respeaker board considered alpha, PE (Voice Kit) software is under heavy development. I will try to update YAML and XMOS board firmware as soon as i can, but it can't be instantenuous...
If you encounter a problem, before creating ticket here you may go to PE and Seeed repos, linked above, and see if something changed there. I'll gladly accept pull-requests. :)

### Why?
I ditched all proprietary assistants for good about 6 months ago, and dove into Assist all-hands. 
With this board i finally can build device, that will satisfy me and (more important) my family as their trusted voice assistant.

### What to do with it?
1. Get Respeaker Lite with ESP32 soldered to it (you may solder it yourself, pins on the back can remain dry, they're not used)
2. [Solder USR to D2 and MUTE to D3 pins](https://wiki.seeedstudio.com/respeaker_button/)
3. [Flash I2S firmware to the XMOS board](https://wiki.seeedstudio.com/xiao_respeaker/#flash-the-i2s-firmware) (pay attention to USB port, you need the main board port, not ESP32 one)
4. Flash ESPHome firmware (YAML included, adjust to your environment) to ESP32 (use its port)
5. Add device to Home Assistant.
   

### Current state
- voice communication: works
- playing media: works (with exception of files, containing album art, playing from HA media picker - this is getting addressed by ESPHome guys (i can't thank them enough!!!)
- media ducking while voice communication ongoing: works
- interactions with HA timers: works
- LED: works (also can be exposed as light in HA)
- mute: hardware button works
- action button: works, multifunctional, based on PE logic

### ToDo
- software mute switch. Software triggering works, but state read doesn't work so far (needs custom ESPHome component for reading mute status with i2c)
- 48kHz bitrate. Right now the board works on 16kHz, but i convinced Seeed guys that we can try using 48kHz for better sound experience. They promised to deliver special firmware soon. Great guys!
- hardware volume controls. Would be nice to have, but there's no pins left on ESP32... Thinking...
- bigger LED strip. The problem is same as for hardware volume...
