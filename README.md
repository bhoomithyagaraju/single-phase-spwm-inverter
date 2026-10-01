# Single-Phase SPWM Full-Bridge Inverter (Arduino UNO)

Arduino UNO sketch that generates a 50 Hz sine-weighted PWM drive for a
single-phase full-bridge (H-bridge) inverter, and shows the battery voltage
on a 16x2 I2C LCD.

## Features
- 50 Hz output, 40 samples per cycle (500 µs per step)
- Half-cycle drive: `PWM_A` drives S1 + S4, `PWM_B` drives S2 + S3
- Modulation index set in code (`MOD_INDEX = 0.8`)
- Battery voltage read on `A0` (0–25 V sensor module, 5:1 divider)
- 16x2 I2C LCD status display, refreshed about once per second

## Hardware
| Part | Notes |
|------|-------|
| Arduino UNO | |
| 4 x MOSFET + gate drivers (e.g. IR2110 / IR2104) | S1–S4 full bridge |
| 0–25 V voltage sensor module | to `A0` |
| 16x2 LCD with I2C backpack | address `0x27` (change to `0x3F` if needed) |

## Pin map
| Signal | Arduino pin | Drives |
|--------|-------------|--------|
| `PWM_A` | D9 | S1 + S4 |
| `PWM_B` | D10 | S2 + S3 |
| `VOLT_PIN` | A0 | battery voltage sensor |
| LCD SDA / SCL | A4 / A5 | I2C |

## Library
Install **LiquidCrystal I2C** from the Arduino Library Manager.

## Build & upload
1. Open `single_phase_inverter/single_phase_inverter.ino` in the Arduino IDE.
2. Select **Arduino Uno** and the correct port.
3. Upload.

## Known limitations (read before connecting a real load)
- `analogWrite()` on pins 9/10 runs at about 490 Hz by default, not 2 kHz.
  `F_CARRIER` is informational only. For a true 2 kHz carrier, configure
  Timer1 registers directly.
- The duty is updated every 500 µs in software, so this is a stepped
  approximation of SPWM rather than a hardware-synchronized one.
- `DEAD_TIME` only delays the software switch-over between legs. It is not
  hardware dead time. Use gate drivers with built-in dead time and test with a
  current-limited supply first.
- The duty starts at 50 % (128) at each zero crossing, so the output is not a
  clean zero-based sine.
- The LCD refresh (I2C) takes several milliseconds and slightly stretches
  one 50 Hz cycle every ~50 cycles.
- The second LCD line, `"Inverter: ON 50Hz"`, is 17 characters and
  overflows a 16-column display by one.
- **Mains-level voltages are dangerous.** Test at low voltage first.

## License
MIT
