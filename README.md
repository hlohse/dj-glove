### Compilation

#### Windows

Make sure the following software is installed:

    Visual Studio Express 2013
    Arduino IDE

To make use of virtual MIDI ports, use the following 3rd party software:

    Go to http://www.tobias-erichsen.de/
    Download and install loopMIDI
    Download the virtualMIDI SDK
    Copy the following virtualMIDI SDK C files to the ThirdParty directory:
        teVirtualMidi.h
        teVirtualMidi32.lib


#### Linux (Ubuntu 14.04)

Make sure the following packages are installed:

    g++ (>= 4.8)
    arduino
    libasound2-dev
    libbluetooth-dev


#### All Platforms

Pair your PC with the Arduino's Bluetooth module using the PIN 1234.

Adjust default Bluetooth address to your module's in PC/BluetoothDevice.cpp, BluetoothDevice::Arduino.

### About

Advanced project of Robotics Lab ST14, University of Heidelberg, Germany.

Authors: Jan-Lukas Tirpitz, Henning Lohse

Supervisors: Karsten Krispin, Thomas Kloepfer

[Demo Video](https://www.youtube.com/watch?v=biKKQlQEOuE)

[Project website (German)](http://joanna.iwr.uni-heidelberg.de/projects/2014SS_DJHANDSCHUH/hardware.html)

[Robotics Lab website](http://joanna.iwr.uni-heidelberg.de/rlab/en/about)

### Copyright

Copyright (c) 2014 Jan-Lukas Tirpitz, Henning Lohse

