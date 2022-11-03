# adcc-low-pass-filter-mode

* This demo illustrates the use of ADCC in low-pass filter mode.
* Enable the ADCC in low-pass filter mode and set Auto Conversion Trigger. 
* Enable Threshold Interrupt to trigger the threshold interrupt each time ADCC completes the set number of conversion repeats, regardless of the results of the threshold comparison.
* The calculated conversion time is approximately  36 microseconds, but there are instructions in the adcc that also take time to execute, so setting 1 ms as the auto-trigger should be sufficient for the adcc to complete the sampling.
* The cuttoff frequency is calculated using the formula shown below.

![image](https://user-images.githubusercontent.com/66494140/199657239-e0dd330b-d48d-434f-98e7-dc26cc6bc895.png)


 # Related Documentation
 * [PIC18F57Q43 Curiosity Nano Hardware User Guide](https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18F57Q43-Curiosity-Nano-HW-UserGuide-DS40002186B.pdf)
 * [ PIC16/PIC18 ADC² Technical Brief](http://ww1.microchip.com/downloads/en/AppNotes/PIC16-PIC18-ADC2-90003194A.pdf)
 * [ Using PIC18F26K42's 12-bit ADC² in Low-Pass Filter Mode](http://ww1.microchip.com/downloads/en/Appnotes/AN2749-Using-PIC18F26K42-ADC2-00002749A.pdf)
 
 # Hardware Used
* [PIC18F57Q43 Curiosity Nano Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM164150)
* [PIC18F47Q10 Curiosity Nano Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM164150)
* [Curiosity Nano Base for Click boards](https://www.microchip.com/en-us/development-tool/AC164162)

### Pin Connection Table

| Description | Direction| Microcontroller Pin|
| ------------- | ------------- |-------------         
| Analog Input  | IN | RA0

# Demo
 * After flashing the application to the PIC18F57Q43 Curiosity Nano, it can be observed that higher frequencies are attenuated. 
 
![image](https://user-images.githubusercontent.com/66494140/199658696-8ebaafc2-9a2f-4051-80fb-07a5200eab70.png)
