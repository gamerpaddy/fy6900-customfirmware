# FY6900 Custom Firmware

> ⚠️ **Project Status: Under Development** - This firmware is currently incomplete and non-functional. Contributions and testing are welcome!

A custom firmware replacement for the FY6900 dual-channel arbitrary waveform signal generator, built with Arduino framework and PlatformIO for STM32F103CB microcontroller.


Blame the AI for writing this readme if anything doesnt work. i hate working with git how it works and this was the only way of getting it to work




## 📋 Table of Contents

- [Overview](#overview)
- [Hardware](#hardware)
- [Current Status](#current-status)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Development](#development)
- [Contributing](#contributing)
- [Known Issues](#known-issues)
- [License](#license)

## 🎯 Overview

This project aims to create an open-source firmware replacement for the FY6900 signal generator, providing:

- Enhanced user interface and functionality
- Open-source codebase for community improvements
- Custom waveform support
- Advanced signal generation capabilities
- Improved system reliability and features

The firmware is designed to run on the STM32F103CB microcontroller that controls the FY6900's user interface, while communicating with the FPGA for signal generation.

## 🔧 Hardware

### Target Device
- **Model**: FY6900 Dual Channel Arbitrary Waveform Signal Generator
- **MCU**: STM32F103 (72MHz ARM Cortex-M3)
- **Flash**: 64K or 128K
- **FPGA**: Controls actual signal generation

### Key Components
- **Display**: TFT LCD with SPI interface
- **Input**: 4x4 matrix keypad + rotary encoder
- **Indicators**: Channel LEDs, Power LED
- **Audio**: Buzzer for feedback
- **Communication**: UART debug interface
- **Signal Generation**: FPGA with SPI communication

## 🚧 Current Status

**⚠️ WORK IN PROGRESS - NOT FUNCTIONAL**

- ✅ Basic project structure established
- ✅ Frontend Hardware initialization
- ✅ Keypad and encoder input handling
- ⚠️ Multi-page UI framework
- ⚠️ FPGA communication protocol
- ⚠️ FPGA initialization hangs (`FPGA_WaitReady()`)
- ❌ Signal generation not working
- ❌ Hardware-specific calibration needed
- ❌ Complete functionality testing required

## 🌟 Features

### Planned Functionality
- **Dual Channel Operation**: Independent control of 2 channels
- **Waveform Types**: Sine, Square, Triangle, Sawtooth, Custom waveforms
- **Frequency Range**: Up to 60MHz (hardware dependent)
- **Amplitude Control**: Adjustable output amplitude
- **Phase Control**: 0-360° phase adjustment
- **DC Offset**: Adjustable DC offset
- **Modulation**: AM, FM, PM modulation capabilities
- **Sweep Function**: Frequency sweep generation
- **Counter**: Frequency measurement
- **VCO Mode**: Voltage controlled oscillator
- **System Settings**: Backlight, beep, calibration

### User Interface
- **Multi-page Interface**: Wave, Mod, Sweep, Counter, VCO, System
- **Intuitive Navigation**: Function keys for quick page switching
- **Real-time Display**: Live parameter updates
- **Visual Feedback**: Color-coded channel indicators

## 💻 Hardware Requirements

### Development Tools
- **ST-Link V2**: For SWD programming and debugging
- **USB-Serial Adapter**: For UART debugging (COM14 in config)
- **PlatformIO**: Build system and IDE integration

### Hardware Connections
```
SWD Programming:
- SWDIO: PA13
- SWCLK: PA14
- GND: Ground
- VCC: 3.3V

UART Debug:
- TX: PA2
- RX: PA3
- Baud: 9600
```

## 🚀 Installation

### Prerequisites
1. [PlatformIO](https://platformio.org/) installed
2. [Visual Studio Code](https://code.visualstudio.com/) with PlatformIO extension
3. ST-Link V2 programmer
4. FY6900 device for testing

### Build Instructions
```bash
# Clone the repository
git clone https://github.com/yourusername/fy6900-firmware-myown.git
cd fy6900-firmware-myown

# Build the project
pio run

# Upload to device (requires ST-Link connected)
pio run --target upload

# Monitor serial output
pio device monitor --port COM14 --baud 9600
```

### Upload Configuration
- **Protocol**: ST-Link (SWD)
- **Target**: STM32F103
- **Flash Size**: 64KB (or 128KB.. depending on model)
- **RAM Size**: 20KB

## 📁 Project Structure

```
fy6900-firmware-myown/
├── src/
│   ├── fy6900-firmware-myown.ino    # Main application entry
│   ├── defines.h                    # Hardware pin definitions
│   ├── fpga.h                       # FPGA communication layer
│   ├── channel_manager.h            # Channel state management
│   ├── page_manager.h               # UI page navigation
│   ├── page_wave.h                  # Waveform control page
│   ├── page_mod.h                   # Modulation control page
│   ├── page_sweep.h                 # Sweep function page
│   ├── page_counter.h               # Frequency counter page
│   ├── page_vco.h                   # VCO control page
│   └── page_system.h                # System settings page
├── platformio.ini                   # PlatformIO configuration
├── .gitignore                       # Git ignore rules
└── README.md                        # This file
``` 

### Key Files

- **[`defines.h`](src/defines.h)**: All hardware pin assignments and constants
- **[`fpga.h`](src/fpga.h)**: FPGA register definitions and communication functions
- **[`channel_manager.h`](src/channel_manager.h)**: Dual channel state management
- **[`page_manager.h`](src/page_manager.h)**: UI navigation and page switching
- **[`page_*.h`](src/)**: Individual page implementations for each function

## 🎮 Usage

### Controls (When Functional)
```
Keypad Layout:
┌─────┬─────┬─────┬─────┐
│  A  │  W  │  M  │  S  │  <- Wave, Mod, Sweep
│ F1  │Wave │ Mod │Sweep│
├─────┼─────┼─────┼─────┤
│  B  │  T  │  V  │  Y  │  <- Counter, VCO, System
│ F2  │Count│ VCO │Sys  │
├─────┼─────┼─────┼─────┤
│  C  │  1  │  K  │  2  │  <- Ch1, OK, Ch2
│ F3  │ CH1 │ OK  │ CH2 │
├─────┼─────┼─────┼─────┤
│  D  │  E  │  <  │  >  │  <- Left, Right navigation
│ F4  │ F5  │Left │Right│
└─────┴─────┴─────┴─────┘

Rotary Encoder: Parameter adjustment
Power Button: System on/off
```

### Navigation
1. **Page Selection**: Use W/M/S/T/V/Y keys to switch between pages
2. **Channel Selection**: Use 1/2 keys to select CH1/CH2
3. **Parameter Editing**: Use A/B/C/D/E to select parameters
4. **Value Adjustment**: Use rotary encoder or +/- for fine tuning
5. **Range Selection**: Use </> to change adjustment increment

## 🛠️ Development

### Building
```bash
# Clean build
pio run --target clean

# Build with verbose output
pio run --verbose

# Build for specific environment
pio run --environment genericSTM32F103CB
```

### Debugging
```bash
# Start debugging session
pio debug

# Monitor serial output
pio device monitor --filter colorize
```

### Dependencies
- **Adafruit Keypad**: v1.2.4 - Matrix keypad handling
- **TFT_eSPI**: v2.5.43 - TFT display library
- **Button**: v1.0.0 - Button debouncing

## 🤝 Contributing

We welcome contributions! This project needs:

### High Priority
- [ ] Fix FPGA initialization hanging issue
- [ ] Complete FPGA register mapping
- [ ] Implement actual signal generation
- [ ] Hardware calibration routines
- [ ] Waveform upload functionality

### Medium Priority  
- [ ] Complete modulation implementation
- [ ] Sweep function implementation
- [ ] Counter/measurement functions
- [ ] Settings persistence (EEPROM/Flash)
- [ ] Custom waveform editor

### Low Priority
- [ ] UI improvements and animations
- [ ] Additional waveform types
- [ ] Remote control interface
- [ ] Automated testing framework

### How to Contribute
1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Make your changes
4. Test thoroughly (if possible)
5. Commit: `git commit -m 'Add amazing feature'`
6. Push: `git push origin feature/amazing-feature`
7. Open a Pull Request

## 🐛 Known Issues

### Critical Issues
- **FPGA Communication**: `FPGA_WaitReady()` function hangs, preventing proper initialization
- **Signal Generation**: No actual output generated yet
- **Hardware Timing**: May need timing adjustments for specific hardware revisions

### Minor Issues
- Display refresh optimization needed
- Encoder debouncing could be improved
- Memory usage optimization required

### Limitations
- No waveform generation currently working
- FPGA flash reading/writing untested
- Hardware-specific calibration not implemented

## 📄 License

This project is open source. Do whatever you want i dont care about licenses.

## 🙏 Acknowledgments

- Original FY6900 hardware design
- Arduino and PlatformIO communities
- STM32 development ecosystem
- All contributors and testers

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/fy6900-firmware-myown/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/fy6900-firmware-myown/discussions)
- **Documentation**: Check the code comments and header files

---

**⚠️ Disclaimer**: This firmware is experimental and may damage your device if it isnt allready damaged. Use at your own risk. Always have a backup of your original firmware before flashing.
