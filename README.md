# OPC_UA_AMAC

An OPC-UA server for interfacing with the **AMACv2** chip on the ATLAS Inner Tracker (ITk) silicon strip detector modules. Built on the [QUASAR](https://github.com/quasar-team/quasar) framework (CERN), it creates an interface to communicate with the AMACv2 hardware registers and let's it control the detector and make measurements via OPC-UA clients such as WinCC-OA.

Developed as part of the ATLAS High Luminosity LHC (HL-LHC) upgrade detector instrumentation effort.

> **Note:** Full operation requires a proprietary ATLAS software layer (`ITSDAQCommandline` / `endeavourTest`) that converts optical signals from the AMACv2 chip into usable digital data. 

---

## Hardware Context

The **AMACv2** (Autonomous Monitor and Control v2) is an ASIC used to monitor and control silicon strip detector modules. It provides:
- 16-channel analog monitoring (temperature, voltage, current)
- NTC thermistor-based temperature readout
- Configurable current measurement ranges via internal shunt resistors
- Register-level read/write access over a dedicated bus

The server models the physical hierarchy: **FibreA → Bus → AMAC**, where one fibre link carries multiple buses, each with multiple AMAC chips.

---

## Repository Structure

```
OPC_UA_AMAC/
├── Device/              ← Custom device logic (primary contribution)
│   ├── include/
│   │   ├── DAMAC.h      — AMACv2 device class
│   │   ├── DBus.h       — Bus device class
│   │   └── DFibreA.h    — Fibre link device class
│   └── src/
│       ├── DAMAC.cpp    — AMACv2 register I/O, ADC readout, calibration
│       ├── DBus.cpp     — Bus management and AMAC linkage
│       └── DFibreA.cpp  — Top-level fibre/bus coordination
├── AddressSpace/        ← OPC-UA address space definitions (QUASAR-generated)
├── CalculatedVariables/ ← Derived variable calculations
├── Configuration/       ← Runtime configuration files
├── Design/
│   └── Design.xml       — Device model specification (drives code generation)
├── FrameworkInternals/  ← QUASAR framework internals (upstream)
├── Server/              ← OPC-UA server entry point (upstream)
├── LogIt/               ← Logging library (upstream)
├── CMakeLists.txt
├── ProjectSettings.cmake
└── quasar.py            — QUASAR framework CLI
```

Files outside `Device/` are mainly taken from the upstream QUASAR codebase.

---

## Device Layer (`Device/`)

This folder contains all custom logic for communicating with AMACv2 hardware.

### `DAMAC` — AMACv2 Chip Interface

The core device class. Implements register-level communication with the AMACv2 chip and higher-level measurement routines.

**Communication backends** (selectable at build/runtime):
- `AMACInterface_ITSDAQCommandline` — Production backend; shells out to `endeavourTest` to talk to the FPGA over optical links
- `AMACInterface_Dummy` — Returns fixed dummy values; useful for development without hardware
- `AMACInterface_Powertools` — Stub for a future `endeavour`-based backend

**Key methods:**

| Method | Description |
|--------|-------------|
| `readReg(reg)` | Read a 32-bit value from an AMACv2 register |
| `writeReg(reg, val)` | Write a value to an AMACv2 register |
| `AMACv2_readAM()` | Read all 16 analog monitoring channels (registers 10-15), extracting 10-bit ADC fields with MUX configuration |
| `AMACv2_readAM_calibrated()` | Apply per-channel calibration factors to raw ADC readings |
| `AMACv2_zeroOffset()` | Establish ADC offset baselines by toggling hardware zero-cal mode |
| `AMACv2_readNTC()` | Auto-ranging NTC thermistor temperature readout using the Steinhart-Hart equation |
| `AMACv2_readIDET()` | Auto-ranging current measurement via switchable internal shunt resistors |
| `registers()` | Initialize OPC-UA variable nodes for all AMACv2 registers |
| `array_declaration()` | Declare array-type OPC-UA nodes |

### `DBus` — Bus Device

Manages a single hardware bus, which may carry multiple AMAC chips.

| Method | Description |
|--------|-------------|
| `linkAMACs()` | Retrieve the refresh period, pause, then call `registers()` on each attached AMAC |
| `busNumber()` | Return the integer bus index |
| `path()` | Return the OPC-UA address space path for this bus |
| `array_declaration()` | Forward array declarations to all attached AMACs |

### `DFibreA` — Fibre Link Device

Top-level device corresponding to a single optical fibre link. Coordinates all buses connected to that link.

| Method | Description |
|--------|-------------|
| `linkAMACs()` | Call `linkAMACs()` on all attached buses |
| `array_declarations()` | Forward array declarations to all attached buses |

---

## Building

QUASAR uses CMake. Standard build procedure:

```bash
# Generate code from Design.xml and build
python quasar.py build
```

Or manually:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

Requires:
- CMake ≥ 3.10
- C++14 compiler (GCC / Clang)
- open62541 (OPC-UA library, configured via `open62541_config.cmake`)
- Boost (threads, regex)
- Java (for XSLT code generation via Saxon)

---

## Running

```bash
./bin/OPC_UA_AMAC Configuration/config.xml
```

The configuration XML specifies the fibre/bus/AMAC hierarchy and connection parameters. Connect any OPC-UA client (WinCC-OA, UA Expert, etc.) to the server endpoint defined in `Configuration/`.

---

## Dependencies

| Dependency | Role |
|------------|------|
| [QUASAR](https://github.com/quasar-team/quasar) | OPC-UA server framework (CERN) |
| [open62541](https://open62541.org/) | Open-source OPC-UA library |
| Boost | Threading, utilities |
| `endeavourTest` / ITSDAQCommandline | ATLAS-internal tool for AMACv2 hardware access (not included) |

---

## License

Framework code (QUASAR, LogIt) is distributed under the **GNU Lesser General Public License v3**. Custom device code in `Device/` follows the same license.

---

## Related

- [QUASAR framework](https://github.com/quasar-team/quasar)
- [AMACv2 chip documentation](https://doi.org/10.25820/etd.006883)
- [ATLAS ITk Strip Detector - HL-LHC upgrade documentation](<https://cds.cern.ch/record/2257755>)
