# 🌡️ IoT Automated Fan Control System

A temperature-responsive fan control system built with Arduino that dynamically adjusts fan speed based on ambient temperature, featuring hysteresis logic to prevent rapid cycling and optimize energy efficiency.

![Project Demo](media/image12.jpeg)

## 📋 Table of Contents
- [Overview](##overview)
- [Key Features](##key-features)
- [Technical Stack](##technical-stack)
- [Hardware Components](##hardware-components)
- [System Architecture](##system-architecture)
- [Implementation Highlights](##implementation-highlights)
- [Performance Results](##performance-results)
- [Future Enhancements](##future-enhancements)
- [Setup & Installation](##setup--installation)
- [References](##references)

## 🎯 Overview

This project addresses Singapore's hot and humid climate (averaging 30°C) by creating an intelligent fan control system that responds dynamically to temperature fluctuations. The system automatically adjusts fan speed based on real-time temperature readings, minimizing energy consumption and noise while maintaining optimal cooling.

**Project Goals:**
- Dynamic temperature-based fan speed control
- Energy-efficient operation with hysteresis logic
- Real-time monitoring via LCD display
- Simple, modular codebase for easy expansion

## ✨ Key Features

- **🔄 Dynamic Speed Control**: Maps temperature to fan speed (25-255 PWM) for smooth operation
- **⚡ Smart Hysteresis**: 0.5°C buffer zone prevents rapid on/off cycling, extending motor lifespan
- **📊 Real-time Monitoring**: 16×2 LCD display shows current temperature and fan status
- **🎛️ Configurable Thresholds**: Easily adjustable temperature limits (default: 28.5°C OFF, 31.5°C MAX)
- **💡 Lightweight Code**: Only 30% of Arduino Uno's memory usage, leaving room for future features
- **🔌 Modular Design**: Clean, well-documented code structure for easy maintenance and expansion

## 🛠️ Technical Stack

**Programming:** C/C++ (Arduino IDE)

**Microcontroller:** Arduino Uno R3

**Key Libraries:**
- `DHT11.h` - Temperature sensor interface
- `LiquidCrystal_I2C.h` - LCD display control
- PWM (Pulse Width Modulation) for motor speed control

**Communication Protocols:** I2C (LCD), Digital I/O (DHT-11)

## 🔌 Hardware Components

| Component | Purpose | Cost (SGD) |
|-----------|---------|------------|
| **Arduino Uno R3** | Main microcontroller and logic processor | $34.00 |
| **DHT-11 Sensor** | Temperature & humidity monitoring | $5.00 |
| **L9110 Motor Driver + Fan** | Variable-speed DC motor control | $6.00 |
| **16×2 LCD Display (I2C)** | Real-time status visualization | $10.00 |
| **ESP8266 ESP-01** | Wireless connectivity (planned) | $11.00 |
| **Breadboard & Jumper Wires** | Prototyping and connections | $17.50 |
| **Total** | | **$83.50** |

### Circuit Diagram
![Circuit Schematic](media/image11.png)

**Pin Configuration:**
- DHT-11: Pin 2 (Digital Input), 5V, GND
- L9110 Motor: Pin 9 (INA/PWM), Pin 10 (INB), 5V, GND
- LCD I2C: A4 (SDA), A5 (SCL), 5V, GND

## 🏗️ System Architecture

### Control Flow
```
┌─────────────────┐
│  DHT-11 Sensor  │ ──► Read Temperature (5s intervals)
└────────┬────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│        Temperature Evaluation           │
│  • T > 31.5°C → Max Speed (PWM: 255)   │
│  • T < 28.5°C → Fan OFF (PWM: 0)       │
│  • 28.5°C - 31°C → Dynamic (PWM: 25-255)│
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────┐        ┌──────────────┐
│  L9110 Motor    │        │  LCD Display │
│  (Fan Control)  │        │  (Feedback)  │
└─────────────────┘        └──────────────┘
```

### Hysteresis Logic
```
Temperature Scale:
28.5°C ──────── 29.5°C ──────── 31.0°C ──────── 31.5°C
  │                │                │                │
  └─ Fan OFF       └─ Dynamic      └─ Dynamic      └─ Max Speed
                    Speed Range      Continues      (Hysteresis On)
```

**Why Hysteresis?**
- Prevents rapid fan cycling near thresholds
- Reduces mechanical wear on motor components
- Provides smoother user experience
- Extends hardware lifespan

## 💻 Implementation Highlights

### Core Temperature Mapping Algorithm
```cpp
// Maps temperature to fan speed with smooth transitions
if (temperature > FAN_ON_TEMP_HYS) {
    analogWrite(INA, 255);  // Maximum speed
} 
else if (temperature < FAN_OFF_TEMP) {
    digitalWrite(INA, LOW);  // Fan off
} 
else {
    // Dynamic speed calculation
    int fanSpeed = map(temperature, FAN_OFF_TEMP, FAN_ON_TEMP, 
                       MIN_FAN_SPEED, 255);
    analogWrite(INA, fanSpeed);
}
```

### Hysteresis Implementation
```cpp
const float FAN_ON_TEMP = 31.0;        // Base ON threshold
const float FAN_OFF_TEMP = 28.5;       // OFF threshold
const float HYS = 0.5;                 // Buffer zone
const float FAN_ON_TEMP_HYS = FAN_ON_TEMP + HYS;  // 31.5°C
```

### Key Technical Decisions
1. **5-second polling interval** - Balances responsiveness with system stability
2. **PWM range 25-255** - Minimum speed ensures fan starts reliably
3. **Separate ON/OFF thresholds** - Creates 2.5°C operational range for smooth transitions
4. **I2C for LCD** - Reduces pin usage from 6 to 2 pins

## 📊 Performance Results

### System Efficiency
- **Memory Usage**: ~30% of available program storage
- **Response Time**: 5-second temperature update cycle
- **Smooth Operation**: No rapid cycling observed with 0.5°C hysteresis
- **Power Consumption**: Variable based on fan speed (25-255 PWM)

### Testing Observations
✅ **Successful**: Dynamic speed adjustment prevents constant max-speed operation  
✅ **Successful**: Hysteresis buffer eliminates on/off oscillation  
✅ **Successful**: LCD provides clear real-time feedback  
⚠️ **Challenge**: ESP8266 integration encountered library compatibility issues

### Real-World Testing
- **Temperature Range Tested**: 27°C - 33°C
- **Fan Response**: Smooth transitions with no chattering
- **Accuracy**: DHT-11 sufficient for fan control (±2°C acceptable)

## 🚀 Future Enhancements

### Planned Features

**1. IoT Integration** (In Progress)
- Remote monitoring via Blynk cloud platform
- Mobile app control and data visualization
- Historical temperature logging and analytics

**2. Advanced Scheduling**
```cpp
// Proposed time-based control
if (currentTime >= 18:00 && currentTime <= 06:00) {
    systemEnabled = false;  // Power save mode
}
```

**3. Sensor Upgrade**
- Replace DHT-11 with DHT-22 for ±0.5°C accuracy
- Alternative: LM35/TMP36 for faster response time

**4. Additional Automation**
- PIR motion sensor for occupancy detection
- Light sensor for day/night mode
- Multi-zone control with multiple fans

**5. Machine Learning Integration**
- Temperature pattern analysis
- Predictive fan speed optimization
- Energy consumption tracking

## 🔧 Setup & Installation

### Prerequisites
```bash
# Arduino IDE Libraries (install via Library Manager)
- DHT sensor library by Adafruit
- LiquidCrystal I2C by Frank de Brabander
```

### Hardware Setup
1. Connect components according to circuit diagram above
2. Ensure proper 5V power supply (Arduino USB or external)
3. Verify I2C LCD address (default: 0x27, use I2C scanner if needed)

### Software Setup
```bash
# 1. Clone this repository
git clone https://github.com/yourusername/iot-fan-control.git

# 2. Open AutomatedFanControl.ino in Arduino IDE

# 3. Install required libraries:
#    Sketch → Include Library → Manage Libraries
#    Search and install: "DHT sensor library" and "LiquidCrystal I2C"

# 4. Select board: Tools → Board → Arduino Uno

# 5. Upload to Arduino
```

### Configuration
Adjust temperature thresholds in code:
```cpp
const float FAN_ON_TEMP = 31.0;   // Modify as needed
const float FAN_OFF_TEMP = 28.5;  // Modify as needed
const float HYS = 0.5;            // Adjust buffer zone
```

## 📈 Project Metrics

**Development Time**: 4 weeks (including testing and documentation)  
**Code Quality**: Well-commented, modular structure  
**Scalability**: 70% memory available for future features  
**Cost Efficiency**: <$100 total component cost

## 🎓 Learning Outcomes

- Implemented PWM for analog motor control
- Applied hysteresis logic to prevent system oscillation
- Integrated multiple I2C devices on single bus
- Optimized Arduino memory usage for embedded systems
- Troubleshot hardware-software integration challenges
- Created professional technical documentation

## 📚 References

1. [Keyestudio L9110 Fan Module](https://wiki.keyestudio.com/Ks0168_keyestudio_L9110_fan_control_module)
2. [LiquidCrystal I2C Library](https://reference.arduino.cc/reference/en/libraries/liquidcrystal-i2c/)
3. [DHT Sensor Library](https://www.arduino.cc/reference/en/libraries/dht-sensor-library/)
4. [Arduino Forum - Hysteresis Implementation](https://forum.arduino.cc/t/hysteresis/506190)
5. [Tinkercad Circuit Simulator](https://www.tinkercad.com)

## 📝 License

This project is open source and available under the MIT License.

Project Link: [https://github.com/renatotan96/IoT-Automated-Fan-Control-System]((https://github.com/renatotan96/IoT-Automated-Fan-Control-System))

---

⭐ If you found this project interesting, please consider giving it a star!
