📝 Description

This code is developed using the QUASAR framework:
🔗 https://quasar.docs.cern.ch/quasar.html

It implements an OPC-UA server that can interface with an OPC-UA client such as WinCC-OA.

The purpose of this code is to establish communication with the AMACv2 chip, located on each silicon strip module of the ATLAS Inner Tracker upgrade.
The logic for interacting with the AMACv2 chip is implemented in the Device/src/ directory.

⚠️ Note:
This code cannot run as a standalone application.
It relies on a separate proprietary ATLAS software layer, which handles the conversion of optical signals from the AMACv2 chip into digital data.
Only after this conversion can the data be interpreted by the modules implemented in this repository.
