# LK12I-H
Harmonized PISO/SIPO version of the LK12I Protocol

# Description
The LK12I-H protocol is a modular, overclockable extension hub for the purposes of tinkering and programming. The main functionalities is the reading and writing of binary data from and to header pins on the Circuit Board.

# Context for new users
## Serial Data
Serial data refers to binary bits (1s and 0s, HIGHs and LOWs) sent in sequence, one after each other, in order. A common example of serial data is data sent to and from your USB port or the music sent to your headphones from your phone. The major advantage is that it requires much less input/output ports, but the major disadvantage is that it is slower.

## Parallel Data
Parallel data refers to binary bits (1s and 0s) sent simultaneously. A common example is the the individual data pins in an USB port. Be mindful that serial data can be transmitted in parallel.
## PISO
## SIPO


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
