# LK12I-H
Harmonized PISO/SIPO version of the LK12I Protocol

# Description
The LK12I-H protocol is a modular, overclockable extension hub for the purposes of tinkering and programming. The main functionalities is the reading and writing of binary data from and to header pins on the Circuit Board.

# Context for new tinkerers

## Data, Bytes and Bits
Data refers to information. In the electrical scene, one of the types of data we can store is known as binary, or 1s and 0s, and ONs and OFFs. 1s are associated with HIGH signals (the signal coming from your power source) and 0s are associated with LOW signals (the signal going to ground). 1s and 0s are referred to as 'Bits', which just means a singular bit of binary data. A byte, on the other hand, is a collection of 8 bits in a specific order. Bytes allow us to organize our information in a standard format. When something outputs data at multiple sources, we list the sources with Qx, where x is a zero-based index. For example, the first output would be called Q0 while the second would be called Q1. When something takes in multiple data, we name the inputs in alphabetical order, such as ABC and D.

## Serial Data
Serial data refers to binary bits sent in sequence, one after each other, in order. A common example of serial data is data sent to and from your USB port or the music sent to your headphones from your phone. The major advantage is that it requires much less input/output ports, but the major disadvantage is that it is slower. A good analogy for serial data is to imagine adding up numbers you hear your friend saying.

## Parallel Data
Parallel data refers to binary bits sent simultaneously. A common example is the the individual data pins in an USB port. Be mindful that serial data can be transmitted in parallel. A good analogy for parallel data is to imagine adding numbers where you can see all of them on a sheet of paper.

## Clock
In electronics, a clock refers to a periodic cycle of HIGH and LOW signals. Many components use clocks to synchronize their procedures within the component or with other components. In most cases, components activate on a clock's rising edge, which is the exact point that the signal changes from LOW to HIGH.

## SIPO
Stands for Serial-In, Parallel-Out. It is essentially a translator that can take serial data as bytes (packages of data) to transmit each bit of that byte in parallel (simultaneously). Think of it as listening to your friend telling you which numbers they want you to write verbally while you write it down on paper. Your friend's voice is the serial data while the finished paper is the parallel data. 

## PISO
Stands for Parallel-In, Serial-Out. It is essentially the opposite of the SIPO. It takes in information from multiple sources and packages them as serial data. Imagine reading numbers on a piece of paper to your friend. The paper has all the information laid out to you in parallel, simultaneously. However, it is up to you to tell your friend the numbers in order, which represents serial data.

## Shift Registers
Shift registers, in essence, refer to components that can store data temporarily while we collect it. The shift registers used in this repo is the 74x595 SIPO shift register and the 74x165 PISO shift register.

### 74x595 SIPO Shift Register
The 74x595 is the component we will utilize to send our data to multiple recievers in this project. Please refer to the pinout.

<img width="832" height="950" alt="image" src="https://github.com/user-attachments/assets/7bb1465f-f723-4327-8050-ccc0bcb31bde" />

As stated before, Q0 to Q7 is our list of outputs. Pin 14, or SER, is the serial data pin we will use to send bytes that the 595 can translate and output. Pin 13 is an Active-LOW Output Enable pin. This means that if we tie it to ground, our outputs will be available.  RCLK and SCLK are two clock inputs that will be explained later. Pin 10 is another Active-LOW pin which resets the chip if it is tied to ground. To counteract that, we tie it to our source voltage. We have two different clocks made available to us, Register Clock and Shift Clock. When detecting a rising edge, it moves every Q output to the next Q output. That would mean that the data in our Q0 would go to Q1, and our Q1 data would already be at Q2 at that point. When the register clock outputs a high edge, the outputs are updated to the current value they store. Usually, the shift clock is pulsed 8 times before the register clock pulses to correctly map the bits in the right orientation. In order to imagine this, it may help to imagine 8 workers in a factory line. The first person in the line is an expert at operating red items, while the second person in the line is an expert at operating orange items. After they operate on their items, the shift clock is pulsed 8 times, which moves the items down 8 spaces. Shortly after the eighth item follows a cardboard box that keeps shifting down until it hits the eighth person. Finally, after everyone has a cardboard box, the register clock pulses, and the workers are allowed to open their box, revealing the item they are an expert at operating on.

###
The 74x165 is the component we will utilize to package our parallel data in a serial manner. Please refer to the pinout.

<img width="1024" height="549" alt="image" src="https://github.com/user-attachments/assets/21e79e64-f84f-4854-9659-ec97a475d187" />

The centre 4 pins on either side are the input pins. Thankfully, they are in a relatively sensible order. Think of the 165 as the inverse of the 595. The CLK INH, or Clock Inhibit, is tied to ground. The clock pin recieves the clock inputs as normal. The Load pin essentially allows us to update our inputs. It is the opposite of the register clock on the 595. As an analogy, the LOAD pin lets each pin collect carboard boxes when it is pulled LOW, and it gives it to the workers when it is pulled HIGH. The SER, or serial data pin, functions as the chip's output. It matches whatever the eighth worker has inside their box. As the clock pulses, the cardboard boxes shift one spot down the assembly line, which exposes new boxes to the SER pin. After all the boxes are collected by the serial data pin, we can load the belt with new boxes. It is important to understand that we need to read the clock pin for the 165 to synchronize it with when we see what is inside each box.


# Uses
- High speed data transfer
- Can be rewired into a standalone USB port

# Branches

## LK12I-H
The LK12I-H (lordking12 Interface, harmonized) is the most complete and stable version of the LK12I protocol. It features both the SIPO and PISO functionalities, packaged with working code to use them simultaneously. The code can be rewritten to a header and library for easier, cleaner access. The main drawback of the stability is lower serial communication rates and noticeably high-latency update speeds. However, the code is slightly blocking and is not ideal for high-speed data transfer such as for a high-speed Bluetooth port.

## LK12I-HvOC
The LK12I-HvOC(lordking12 Interface, Harmonized Overclock version, or HAVOC) is the fastest running, mostly stable release of the LK12I-H protocol. The serial port is ran at 250000 baud for the fastest, least blocking code.

## LK12I-HvOC-5x3 (Unreleased, Experimental)
The LK12I-HvOC-5x3 is an experimental prerelease that utilizes a 555/556 timer for overclocking on the client-side. This project is most ideal with the CMOS variants of the 4017, 74x165, 555, and 74x595.

## LK12I-SIPO
The LK12I protocol with only the SIPO functionalities.

## LK12I-PISO
The LK12I protocol with only the PISO functionalities.
