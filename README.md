This code is developed using QUASAR framework: https://quasar.docs.cern.ch/quasar.html.
It creates a OPC-UA server which can then connect to a OPC-UA client such as WinCC-OA. 

This code establishes a connection with AMACv2 chip that is located on each silicon module. The code to establish the connection with the chip is in Device/src/ folder.

The code can't be run on its own, and needs to communicate with a software (proprietary code belonging to the ATLAS collaboration) specifically designed to convert the optical signals from AMACv2 chip to digital data that can be then interpreted by the above code.  
