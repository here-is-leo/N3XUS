<div align="center">

# ⚡ N3XUS

### *The Ultimate Electronic Warfare Toolkit*

![GitHub license](https://img.shields.io/github/license/here-is-leo/N3XUS)
![GitHub stars](https://img.shields.io/github/stars/here-is-leo/N3XUS)
![GitHub forks](https://img.shields.io/github/forks/here-is-leo/N3XUS)
![GitHub issues](https://img.shields.io/github/issues/here-is-leo/N3XUS)
![GitHub release](https://img.shields.io/github/v/release/here-is-leo/N3XUS)

![N3XUS Banner](assets/images/banner.png)

**"Security is not a product, but a process."** — *Bruce Schneier*

---

## 🚨 IMPORTANT LEGAL NOTICE
╔══════════════════════════════════════════════════════════════════╗
║ THIS TOOL IS PROVIDED FOR EDUCATIONAL AND RESEARCH PURPOSES ║
║ ONLY. THE AUTHOR DOES NOT CONDONE OR SUPPORT ANY ILLEGAL ║
║ USE OF THIS TECHNOLOGY. USING THIS TOOL AGAINST NETWORKS OR ║
║ DEVICES WITHOUT EXPLICIT PERMISSION IS ILLEGAL AND MAY ║
║ RESULT IN SEVERE PENALTIES UNDER LOCAL AND INTERNATIONAL ║
║ LAWS. BY USING THIS TOOL, YOU AGREE TO TAKE FULL ║
║ RESPONSIBILITY FOR YOUR ACTIONS. ║
╚══════════════════════════════════════════════════════════════════╝

text

---

## 📡 What is N3XUS?

**N3XUS** is a state-of-the-art, portable **Electronic Warfare (EW) testing toolkit** built around the powerful ESP32 microcontroller. It integrates **three NRF24L01+ PA+LNA modules** for 2.4GHz operations and a **CC1101 transceiver** for sub-1GHz frequency bands, creating a comprehensive platform for:

- 🔍 **RF Reconnaissance** — Full-spectrum scanning and signal analysis
- ⚔️ **Attack Simulation** — Deauthentication attacks, packet injection, and frequency jamming
- 🎭 **Deception Techniques** — Rogue AP creation, MAC spoofing, and device manipulation
- 🛡️ **Network Security Testing** — WPA/WPA2 handshake capture and PMKID analysis
- 📊 **Real-time Visualization** — TFT display with live status and statistics
- 🌐 **Remote Control** — Web-based dashboard accessible via WiFi

---

## 🎯 Why N3XUS?

| Feature | N3XUS | Flipper Zero | HackRF | RTL-SDR |
|---------|-------|--------------|--------|---------|
| **Cost** | ~$50 | ~$200 | ~$400 | ~$30 |
| **Frequency Bands** | 2.4GHz + Sub-1GHz | Sub-1GHz + RFID | 1MHz-6GHz | 24MHz-1.7GHz |
| **Transmit Capability** | ✅ Yes | ✅ (Limited) | ✅ Yes | ❌ No |
| **3x NRF24 Modules** | ✅ Simultaneous | ❌ No | ❌ No | ❌ No |
| **Web Panel Control** | ✅ Built-in | ❌ No | ❌ No | ❌ No |
| **TFT Display** | ✅ 1.69" Color | ❌ No | ❌ No | ❌ No |
| **SD Card Logging** | ✅ Included | ✅ Included | ❌ No | ❌ No |
| **12 Operational Modes** | ✅ Comprehensive | ✅ Limited | ❌ No | ❌ No |
| **Open Source** | ✅ MIT License | ❌ Closed | ✅ Partially | ✅ GNU |
| **Educational Value** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ |

---

## 🚀 Quick Start

```bash
# 1. Clone the repository
git clone https://github.com/here-is-leo/N3XUS.git

# 2. Navigate to the project
cd N3XUS

# 3. Open the firmware in Arduino IDE
# File → Open → firmware/N3XUS_V7.1.ino

# 4. Install required libraries (see below)

# 5. Upload to ESP32
# Tools → Board → ESP32 Dev Module
# Tools → Port → Select your COM port
# Click Upload

# 6. Connect to the device
# WiFi: N3XUS (password: 12345678)
# Web Panel: http://192.168.4.1
📦 Hardware Requirements
Essential Components
Component	Quantity	Specifications	Estimated Price (USD)	Estimated Price (IRR)
ESP32 Development Board	1	38-pin, WROOM-32, 4MB Flash	$8-12	150,000-300,000
NRF24+PA+LNA	3	2.4GHz, with external antenna	$10-15 each	250,000-400,000
CC1101 Transceiver	1	Sub-1GHz, with antenna	$8-12	200,000-350,000
TFT Display 1.69"	1	ST7789 driver, 240x280	$12-18	300,000-500,000
OLED 0.96"	1	I2C, SSD1306 (optional)	$3-5	80,000-150,000
SD Card Reader	1	SPI interface	$2-4	50,000-100,000
Micro SD Card	1	4-32GB, FAT32 format	$5-10	100,000-200,000
AMS1117 3.3V Regulator	1	With heatsink	$2-3	50,000-80,000
Capacitors	Several	10µF, 100µF, 22µF	$2-5	50,000-100,000
LEDs (5 colors)	5	With 220Ω resistors	$3-5	80,000-150,000
Push Buttons	3	Tactile switches	$2-3	50,000-80,000
Breadboards	2+	Large + mini	$10-15	250,000-400,000
Jumper Wires	Set	Male-to-male, male-to-female	$5-10	100,000-200,000
5V 2A Power Supply	1	USB charger or power bank	$10-15	250,000-400,000
Optional Components
Component	Description	Estimated Price
18650 Li-ion Battery	For portable operation	$5-10
TP4056 Charger Module	For battery charging	$2-3
Boost Converter	3.7V to 5V step-up	$2-4
Enclosure/Case	3D printed or project box	$10-20
External Antennas	For improved range	$5-15 each
Total Estimated Cost: $80-120 (1,500,000 - 2,500,000 IRR)
🔌 Pinout Diagram
ESP32 30-Pin Pinout
text
┌─────────────────────────────────────────────────────────────────────┐
│                     ESP32-WROOM-32 (30-Pin)                       │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│   GND  ────┐                                                ┌─── 3.3V │
│   GPIO23 ──┼──── MOSI (SPI)                               ┼─── GPIO22 │
│   GPIO22 ──┤                                                ├─── GPIO21 │
│   GPIO1 ───┤                                                ├─── GPIO19 │─ MISO
│   GPIO3 ───┤                                                ├─── GPIO18 │─ SCK
│   GPIO21 ──┤                                                ├─── GPIO5  │─ RST (TFT)
│   GPIO19 ──┼──── MISO (SPI)                               ┼─── GPIO17 │─ GDO0 (CC1101)
│   GPIO18 ──┼──── SCK (SPI)                                ┼─── GPIO16 │─ DC (TFT) / CS (CC1101)
│   GPIO5 ───┼──── RST (TFT)                               ┼─── GPIO4  │─ SW3 (Button)
│   GPIO17 ──┼──── GDO0 (CC1101)                           ┼─── GPIO0  │─ (BOOT Button)
│   GPIO16 ──┼──── DC (TFT) / CS (CC1101)                 ┼─── GPIO2  │─ CS (TFT)
│   GPIO4 ───┼──── SW3 (Button)                            ┼─── GPIO15 │─ LED_WHITE
│   GPIO0 ───┼──── (BOOT Button)                           ┼─── GPIO13 │─ LED_RED
│   GPIO2 ───┼──── CS (TFT)                                ┼─── GPIO12 │─ LED_GREEN
│   GPIO15 ──┼──── LED_WHITE                               ┼─── GPIO14 │─ CS (SD Card)
│   GPIO13 ──┼──── LED_RED                                 ┼─── GPIO27 │─ LED_YELLOW
│   GPIO12 ──┼──── LED_GREEN                               ┼─── GPIO26 │─ CSN (NRF1)
│   GPIO14 ──┼──── CS (SD Card)                            ┼─── GND    │
│   GPIO27 ──┼──── LED_YELLOW                              ┼─── 3.3V   │
│   GPIO26 ──┼──── CSN (NRF1)                              ┼─── EN     │─ Reset
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
Component Connections Table
Component	Pin	ESP32	Description
SPI Bus	SCK	GPIO 18	Shared Clock
MOSI	GPIO 23	Shared Data Out
MISO	GPIO 19	Shared Data In
TFT	CS	GPIO 2	Chip Select
DC	GPIO 16	Data/Command
RST	GPIO 5	Reset
BLK	3.3V	Backlight
NRF1 (Assassin)	CE	GPIO 25	Chip Enable
CSN	GPIO 26	Chip Select
NRF2 (Watcher)	CE	GPIO 32	Chip Enable
CSN	GPIO 33	Chip Select
NRF3 (Ghost)	CE	GPIO 22	Chip Enable
CSN	GPIO 21	Chip Select
CC1101	CS	GPIO 16	Chip Select
GDO0	GPIO 17	GPIO/Interrupt
SD Card	CS	GPIO 14	Chip Select
LEDs	Green	GPIO 12	System Ready
Red	GPIO 13	Alert/Attack
Yellow	GPIO 27	Scanning
White	GPIO 15	Standby
Blue	GPIO 34	SD Activity
Buttons	SW1	GPIO 33	Menu/Back
SW2	GPIO 32	Up/Select
SW3	GPIO 4	Down/Back
🎮 12 Operational Modes
📡 Reconnaissance (Scanning & Discovery)
#	Mode	Icon	Description	Use Case
1	Omniscient	📡	Full 125-channel scan (2.4GHz + sub-1GHz)	Discover all devices in range
2	Shadow	👤	Stealth scan (receive-only, no transmission)	Undetectable reconnaissance
3	Spectrum Analyzer	📊	Real-time signal strength visualization	RF environment analysis
4	Device Finder	🎯	Targeted device discovery (cameras, drones)	Locate specific devices
⚔️ Attack (Offensive Operations)
#	Mode	Icon	Description	Use Case
5	Annihilator	💥	Multi-channel attack on all networks	Widespread disruption
6	Sniper	🎯	Targeted attack on a specific channel	Precision strikes
7	Blackout	🌑	Full frequency jamming across all channels	Complete signal blackout
8	Zombie	🧟	Packet injection into target networks	Confuse security systems
🎭 Deception (Social Engineering)
#	Mode	Icon	Description	Use Case
9	Mirage	🎭	Rogue AP creation	Phishing and man-in-the-middle
10	Chameleon	🦎	MAC address spoofing	Hide identity in networks
11	Puppet	🎪	IoT device control	Manipulate smart devices
🔄 Hybrid (Combined Operations)
#	Mode	Icon	Description	Use Case
12	Omega	🌀	Combined mode (Scan+Attack+Inject)	Advanced operations
🖥️ Web Panel Features
Dashboard Overview
text
┌─────────────────────────────────────────────────────────────────┐
│  ⚡ N3XUS v7.1                                    ● ONLINE    │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │  📡 Mode: Omniscient        🌐 Networks: 12             │ │
│  │  📊 Status: ACTIVE          📤 Packets: 1,247          │ │
│  │  ⏱️ Uptime: 02:34:17        📋 Last Result: Scan OK    │ │
│  └───────────────────────────────────────────────────────────┘ │
│                                                               │
│  [▶ ACTIVATE]  [🔍 SCAN]  [⚡ ATTACK]                       │
│                                                               │
│  [◀ PREV]  [NEXT ▶]                                         │
│                                                               │
│  Select Mode: [Omniscient ▾]                                 │
│                                                               │
│  ● System ready                                               │
│  ──────────────────────────────────────────────────────────── │
│  N3XUS v7.1 · 12 Modes · 3x NRF                             │
└─────────────────────────────────────────────────────────────────┘
Web Panel Capabilities
Feature	Description
Real-time status monitoring	Live system state (ONLINE/OFFLINE)
Mode selection and execution	Choose from 12 modes with one click
Live packet statistics	Track packets sent/received in real-time
Network discovery results	View detected networks and signal strength
Command execution interface	Run scans, attacks, and operations
Mobile-responsive design	Works on phones, tablets, and desktops
Auto-refresh every 2 seconds	Always up-to-date information
Dark theme	Eye-friendly design for extended use
📖 Step-by-Step Setup Guide
1️⃣ Install Arduino IDE
bash
# Windows/Mac/Linux
# Download from: https://www.arduino.cc/en/software
# Install version 1.8.19 or newer
# Follow installation wizard for your OS
Detailed Installation Steps:

Windows:

Download the Windows installer (.exe)

Run the installer

Follow the wizard, accept the license

Choose installation directory (default: C:\Program Files (x86)\Arduino)

Select "Install USB drivers" option

Complete installation

Mac:

Download the Mac version (.dmg)

Open the .dmg file

Drag Arduino.app to Applications folder

Open Arduino from Applications

Linux:

bash
# Ubuntu/Debian
sudo apt update
sudo apt install arduino

# Or download the Linux AppImage from the website
chmod +x arduino-*.AppImage
./arduino-*.AppImage
2️⃣ Add ESP32 Board Support
bash
# Open Arduino IDE → File → Preferences
# Add to "Additional Boards Manager URLs":
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Click OK to save
# Tools → Board → Boards Manager
# Search "ESP32"
# Install "esp32 by Espressif Systems"
# Wait for installation to complete
3️⃣ Install Required Libraries
bash
# Sketch → Include Library → Manage Libraries
# Search and install each library:
Essential Libraries:

Library	Author	Search Term	Purpose
TFT_eSPI	Bodmer	TFT_eSPI	TFT display driver
RF24	TMRh20	RF24	NRF24 communication
ELECHOUSE_CC1101	ELECHOUSE	ELECHOUSE_CC1101	CC1101 communication
SD	Built-in	SD (built-in)	SD card interface
SPI	Built-in	SPI (built-in)	SPI communication
WiFi	Built-in	WiFi (built-in)	WiFi connectivity
WebServer	Built-in	WebServer (built-in)	Web server
Installation Instructions:

Click on the library name

Click "Install" button

Wait for download and installation

Repeat for each library

4️⃣ Configure TFT_eSPI Library
bash
# Navigate to: libraries/TFT_eSPI/User_Setup.h
# Open in a text editor
# Enable/configure these lines:
Required Configuration:

cpp
// ==========================================
// DISPLAY DRIVER
// ==========================================
#define ST7789_DRIVER           // Enable ST7789 driver

// ==========================================
// DISPLAY RESOLUTION
// ==========================================
#define TFT_WIDTH  240          // Display width
#define TFT_HEIGHT 280          // Display height

// ==========================================
// PIN CONFIGURATION
// ==========================================
#define TFT_CS   2              // Chip select pin
#define TFT_DC   16             // Data/Command pin
#define TFT_RST  5              // Reset pin

#define TFT_SCLK 18             // SPI Clock
#define TFT_MOSI 23             // SPI Data Out
#define TFT_MISO 19             // SPI Data In

// ==========================================
// BACKLIGHT
// ==========================================
#define TFT_BACKLIGHT_ON HIGH   // Backlight active high

// ==========================================
// FONTS
// ==========================================
#define LOAD_GLCD               // Load GLCD font
#define LOAD_FONT2              // Load Font2
#define LOAD_FONT4              // Load Font4
#define LOAD_FONT6              // Load Font6
#define LOAD_FONT7              // Load Font7
#define LOAD_FONT8              // Load Font8
#define LOAD_GFXFF              // Load GFX font
#define SMOOTH_FONT             // Enable smooth fonts
5️⃣ Upload the Firmware
bash
# Step-by-step upload process:
Open the firmware:

File ← Open

Navigate to firmware/N3XUS_V7.1.ino

Click Open

Select the board:

Tools ← Board ← ESP32 Dev Module

Configure board settings:

Tools ← CPU Frequency: 240MHz

Tools ← Flash Size: 4MB

Tools ← Partition Scheme: Default

Tools ← Upload Speed: 115200

Select the port:

Tools ← Port ← Select your COM port

Windows: COM3, COM4, etc.

Mac: /dev/cu.usbserial-*

Linux: /dev/ttyUSB0, /dev/ttyACM0

Upload:

Click the Upload button (←)

Wait for compilation and upload

Look for "Done uploading" message

Troubleshooting Upload Issues:

Issue	Solution
"Failed to connect"	Hold BOOT button, press EN, release BOOT, try again
"Timed out"	Reduce Upload Speed to 115200
"Port not found"	Install USB drivers (CP2102 or CH340)
"Compilation error"	Verify all libraries are installed
6️⃣ Connect to the Device
bash
# After successful upload:
WiFi Connection:

Open WiFi settings on your device

Find network: N3XUS

Enter password: 12345678

Wait for connection

Web Panel Access:

Open web browser

Navigate to: http://192.168.4.1

The N3XUS web panel should appear

Serial Monitor:

Tools ← Serial Monitor

Baud rate: 115200

View system logs and debug messages

🛠️ Troubleshooting Guide
Common Issues and Solutions
Issue	Solution
TFT white screen	1. Check User_Setup.h → Enable ST7789_DRIVER
2. Verify CS (GPIO 2), DC (GPIO 16), RST (GPIO 5)
3. Check 3.3V power supply
4. BLK pin must be connected to 3.3V
NRF not found	1. Add 10µF capacitor between VCC and GND
2. Verify CE/CSN pins
3. Check 3.3V power with multimeter
4. Ensure PA+LNA version is used
CC1101 fails	1. Check CS pin (GPIO 16)
2. Install ELECHOUSE_CC1101 library
3. Verify SPI connections
4. Check GDO0 pin connection
SD Card not reading	1. Format SD Card as FAT32
2. Maximum 32GB capacity
3. Check CS pin (GPIO 14)
4. Try a different SD card
Web panel not loading	1. Connect to N3XUS WiFi
2. Open http://192.168.4.1
3. Disable firewall temporarily
4. Check WiFi connection
Upload fails	1. Remove all wires from ESP32
2. Hold BOOT → Press EN → Release BOOT
3. Select correct COM port
4. Try different USB cable
Constant reboots	1. Use 5V 2A power supply
2. Add 1000µF capacitor
3. Check power connections
4. Check for short circuits
Buttons not working	1. Use INPUT_PULLUP in code
2. Verify GPIO connections to GND
3. Check debounce delay
4. Test with multimeter
OLED not working	1. Check I2C address (0x3C or 0x3D)
2. Verify SDA/SCL pins
3. Check 3.3V power
4. Try different OLED library
WiFi not connecting	1. Verify AP_SSID and AP_PASS
2. Check ESP32 WiFi antenna
3. Reset device
4. Check for interference
Advanced Troubleshooting
Checking Power Supply:

bash
# Using multimeter:
# 1. Measure voltage between VCC and GND: should be 3.3V
# 2. Measure current draw: should be < 500mA idle
# 3. Check for voltage drops during operation
Checking SPI Communication:

bash
# In Serial Monitor:
# Look for:
# - "NRF1 OK" messages
# - "CC1101 OK" messages
# - SPI initialization success messages
# - Any error messages
Testing Individual Components:

cpp
// Test NRF24 only
#include <RF24.h>
RF24 radio(25, 26);
void setup() {
  Serial.begin(115200);
  if(radio.begin()) Serial.println("NRF OK");
}

// Test TFT only
#include <TFT_eSPI.h>
TFT_eSPI tft;
void setup() {
  tft.init();
  tft.fillScreen(TFT_RED);
}

// Test CC1101 only
#include <ELECHOUSE_CC1101.h>
void setup() {
  Serial.begin(115200);
  ELECHOUSE_cc1101.Init();
  Serial.println("CC1101 OK");
}
📚 Documentation Structure
text
N3XUS/
├── README.md                 ← You are here!
├── LICENSE                   ← MIT License
├── .gitignore               ← Git ignore rules
├── firmware/
│   ├── N3XUS_V7.1.ino       ← Main firmware
│   └── libraries/            ← Required libraries
├── hardware/
│   ├── schematic.png         ← Circuit schematic
│   ├── pinout.png            ← ESP32 pinout
│   └── wiring_diagram.png    ← Breadboard diagram
├── docs/
│   ├── setup_guide.md        ← Detailed setup instructions
│   ├── commands.md           ← Command reference
│   └── troubleshooting.md    ← Troubleshooting guide
├── assets/
│   ├── images/               ← Project images
│   └── videos/               ← Demo videos
└── web/
    └── panel.html            ← Web panel HTML
🔧 Advanced Configuration
Customizing WiFi Settings
cpp
// In firmware/N3XUS_V7.1.ino
// Find these lines at the top of the file:

const char* AP_SSID = "N3XUS";        // Change network name (max 32 chars)
const char* AP_PASS = "12345678";     // Change password (min 8 chars)
Adjusting NRF24 Power Levels
cpp
// In initNRF() function
// Change the third parameter:

// Power Level Options:
// RF24_PA_MIN   - ~-18dBm (lowest power)
// RF24_PA_LOW   - ~-12dBm (low power)
// RF24_PA_HIGH  - ~-6dBm  (medium power)
// RF24_PA_MAX   - ~0dBm   (maximum power)

radio.setPALevel(RF24_PA_LOW);    // For battery saving
radio.setPALevel(RF24_PA_HIGH);   // Balanced mode
radio.setPALevel(RF24_PA_MAX);    // Maximum range
Changing CC1101 Frequency
cpp
// In executeMode() function
// Use setMHZ() function:

ELECHOUSE_cc1101.setMHZ(433.92);   // 433 MHz band
ELECHOUSE_cc1101.setMHZ(868.0);    // 868 MHz band
ELECHOUSE_cc1101.setMHZ(915.0);    // 915 MHz band
ELECHOUSE_cc1101.setMHZ(434.0);    // Alternative 434 MHz
Modifying Button Functions
cpp
// In loop() function
// Change the button actions:

if(readButton(SW1)) {
  // SW1 action - currently page change
  currentPage = (PageState)((currentPage + 1) % 4);
  updatePage();
}

if(readButton(SW2)) {
  // SW2 action - currently execute mode
  if(systemActive && currentState == STATE_IDLE) {
    executeMode(currentMode);
  }
}

if(readButton(SW3)) {
  // SW3 action - currently change mode
  changeMode(1);
  updatePage();
}
Adding Custom Modes
cpp
// 1. Add to OperationMode enum:
enum OperationMode {
  MODE_OMNISCIENT,
  // ... existing modes ...
  MODE_CUSTOM,      // ← Add new mode
  MODE_OMEGA
};

// 2. Add name to modeNames array:
const char* modeNames[] = {
  "Omniscient",
  // ... existing names ...
  "Custom Mode",    // ← Add new name
  "Omega"
};

// 3. Add to executeMode() function:
case MODE_CUSTOM:
  // Your custom code here
  showResult("✨ Custom Mode", "Custom operation executed", TFT_CYAN);
  break;
🤝 Contributing
We welcome contributions! Here's how:

Fork the Repository
Navigate to github.com/here-is-leo/N3XUS

Click "Fork" button (top right)

Choose your account

Wait for fork to complete

Create a Feature Branch
bash
# Clone your fork
git clone https://github.com/your-username/N3XUS.git
cd N3XUS

# Create feature branch
git checkout -b feature/amazing-feature
Make Your Changes
bash
# Make your changes to the code
# Add new files if needed
# Update documentation
# Test thoroughly
Commit and Push
bash
# Stage changes
git add .

# Commit with clear message
git commit -m 'Add amazing feature'

# Push to your fork
git push origin feature/amazing-feature
Open a Pull Request
Go to your fork on GitHub

Click "Pull Request" button

Select branch: feature/amazing-feature

Fill in the PR template

Submit the pull request

Contribution Guidelines
Guideline	Description
Code Style	Follow existing code formatting
Comments	Add comments for complex logic
Testing	Test your changes thoroughly
Documentation	Update docs as needed
Commit Messages	Use clear, descriptive messages
Single Purpose	Each PR should address one issue
📜 License
This project is licensed under the MIT License — see the LICENSE file for details.

text
MIT License

Copyright (c) 2026 Ilya Farahani (Leo)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
License Summary
Permission	Commercial Use	Modification	Distribution	Private Use
✅ Yes	✅ Yes	✅ Yes	✅ Yes	✅ Yes
Conditions:

Include original copyright notice

Include license text in distribution

No warranty or liability

📬 Contact & Support
Developer Information
Field	Details
Name	Ilya Farahani (Leo)
Title	Senior Backend Developer & Security Researcher
Location	Tehran, Iran
Experience	4-5 years production systems
Specialization	Python, C#, Security, APIs
Contact Channels
Channel	Link
GitHub	github.com/here-is-leo
Blog	here-is-leo-blog.ilyafarahanii.workers.dev
Email	ilyafarahanii@gmail.com
Telegram	@here_is_leo
LinkedIn	linkedin.com/in/ilya-farahani
Support Options
Option	Description
⭐ Star the repository	Show appreciation on GitHub
🍵 Buy me a coffee	coffeebede.com/leo
🐛 Report issues	Submit bug reports on GitHub
📖 Contribute docs	Help improve documentation
🔧 Submit PRs	Contribute code improvements
🙏 Acknowledgments
Open Source Libraries
Library	Author	Purpose
TFT_eSPI	Bodmer	TFT display driver
RF24	TMRh20	NRF24 communication
ELECHOUSE_CC1101	ELECHOUSE	CC1101 communication
ArduinoJson	bblanchon	JSON parsing
Inspiration & References
Resource	Description
ESP32 Community	Hardware and software support
RF Security Research	Academic and practical papers
Open Source Security Tools	Community projects and tools
Cybersecurity Education	Learning platforms and courses
Special Thanks
All contributors and testers

Security researchers pushing ethical boundaries

Open-source community for accessible knowledge

ESP32 development team

RF24 and CC1101 library maintainers

📈 Project Status
Metric	Status
Version	v7.1
Release Date	August 2026
Stability	Stable
Documentation	Complete
Test Coverage	Extensive
Community	Growing
🚀 Roadmap
Upcoming Features
Feature	Status	Expected
BLE Attack Mode	🟡 In Development	Q3 2026
Mobile App Control	🔵 Planned	Q4 2026
GPS Integration	🔵 Planned	Q1 2027
AI-based Detection	🔵 Planned	Q2 2027
Automated Reporting	🟡 In Development	Q3 2026
Long-term Vision
□ Full SDR integration
□ Cloud-based analysis
□ Multi-device coordination
□ Advanced AI features
□ Commercial hardware version
<div align="center">
⭐ If you find this project useful, please consider giving it a star!

Report Bug · Request Feature · Read Docs

Made with ❤️ by Leo

"Security is not a product, but a process." — Bruce Schneier

</div> ```"## Installation" 
