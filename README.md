# MiniCableLab

MiniCableLab is a small cable tester I made with an **Arduino Uno**.

The idea is simple: before using a wire in another project, I want to check if it works and see what the problem is if it doesn't.

I started with a basic continuity test. Then I added a **16x2 LCD**, **two LEDs**, and **two buttons** so I can use it without opening the Arduino Serial Monitor every time.

**Important:** This tester is for disconnected, unpowered wires only. Never connect it to a powered circuit, a battery, or mains electricity.

## What it can do

There are two modes:

- **1 WIRE:** check if one wire has continuity.
- **2 WIRES:** check two connections and detect different wiring problems.

The **MODE** button changes the mode.

The **TEST** button starts a measurement. The result stays on the LCD until I change the mode or start another test.

A **green LED** means the test passed. A **red LED** means a problem was detected.

## Components

- Arduino Uno
- Breadboard
- LCD 1602 (16 columns, 2 rows)
- 2 push buttons (TEST and MODE)
- 1 green LED and 1 red LED
- 4 resistors of 1 kΩ: 2 for the LEDs and 2 to protect the test outputs
- A current-limiting resistor for the LCD backlight
- Dupont wires (male-male, male-female, female-female)

## Connections

### Arduino pins

| Pin | Used for |
| --- | --- |
| D2 | First test output, through a 1 kΩ resistor |
| D3 | Second test output, through a 1 kΩ resistor |
| D6 | First test input (`INPUT_PULLUP`) |
| D7 | Second test input (`INPUT_PULLUP`) |
| D5 | Green LED, with its own 1 kΩ resistor |
| D4 | Red LED, with its own 1 kΩ resistor |
| D10 | TEST button (other side connected to GND) |
| D11 | MODE button (other side connected to GND) |

### LCD 1602

| LCD pin | Arduino pin |
| --- | --- |
| VSS | GND |
| VDD | 5V |
| VO | GND (fixed contrast for now) |
| RS | D8 |
| RW | GND |
| E | D9 |
| D4 | A0 |
| D5 | A1 |
| D6 | A2 |
| D7 | A3 |

LCD pins D0 to D3 are not used. The LCD backlight uses `A` and `K`, with a suitable series resistor on `A`.

## How the cable test works

I use two Arduino pins to send a LOW signal (`D2` and `D3`) and two pins to read it (`D6` and `D7`).

The inputs are configured with `INPUT_PULLUP`:

- **HIGH (1):** no LOW signal was detected.
- **LOW (0):** a LOW signal reached the input through an electrical connection.

In **2 WIRES** mode, I test one output at a time. This is important because I need to know exactly which output is connected to each input.

Expected connections:

```text
D2 --- 1 kΩ --- [wire to test] --- D6
D3 --- 1 kΩ --- [wire to test] --- D7
```

The wires being tested are removable. There must not be another direct connection between the test points, or the test would be wrong.

### 1 WIRE mode

Only the connection between **D2 and D6** is checked.

| LCD message | Meaning |
| --- | --- |
| `SINGLE: ALL GOOD` | The wire has continuity |
| `SINGLE: OPEN` | No continuity detected |

An open circuit means the electrical path is interrupted. It could be a broken wire, a disconnected end, or a bad contact.

### 2 WIRES mode

The expected connections are **D2 → D6** and **D3 → D7**.

| LCD message | Meaning |
| --- | --- |
| `ALL GOOD !` | Both connections are correct |
| `D2-D6 CUT` | No continuity on the first connection |
| `D3-D7 CUT` | No continuity on the second connection |
| `BOTH CABLES CUT` | No continuity on either connection |
| `INVERTED CABLE` | The destinations are swapped: D2 → D7 and D3 → D6 |
| `SHORT CIRCUIT` | A possible short circuit or unexpected connection |
| `OTHER FAULT` | The readings do not match the cases above |

**What is a short circuit?**

A short circuit is an electrical connection between points that should normally be separated. For example, two damaged wires can touch because their insulation is broken.

In my test, if the LOW signal reaches **both D6 and D7 at the same time**, the program reports a possible short circuit.

This is not a perfect diagnosis: another wrong connection can sometimes produce the same readings. Also, `CUT` means *no continuity detected* — it does not prove that the wire is physically broken.

## How to use it

1. Disconnect the Arduino USB cable before changing the test wiring.
2. Connect the **unpowered** wire or wires I want to test.
3. Plug in the Arduino.
4. Press **MODE** to choose `1 WIRE` or `2 WIRES`.
5. Press **TEST** to start the check.
6. Read the LCD message and the status LED.

The TEST button does not turn the tester off. It just starts another measurement.

## Demo videos

I want to show every case with a short video: the wiring, the button press, the LCD message and the LED.

| Demo | What I will do | Expected LCD message | Video |
| --- | --- | --- | --- |
| 01 | Test one working wire | `SINGLE: ALL GOOD` | To record |
| 02 | Disconnect the single wire | `SINGLE: OPEN` | To record |
| 03 | Connect both wires correctly | `ALL GOOD !` | To record |
| 04 | Disconnect D2 → D6 | `D2-D6 CUT` | To record |
| 05 | Disconnect D3 → D7 | `D3-D7 CUT` | To record |
| 06 | Disconnect both wires | `BOTH CABLES CUT` | To record |
| 07 | Swap the destinations of the two wires | `INVERTED CABLE` | To record |
| 08 | Simulate a short between the two test conductors | `SHORT CIRCUIT` | To record |

I will add the actual links after filming and checking the results. The short-circuit simulation must be wired **with the USB disconnected**, and only on the unpowered test connections.

## Project files

```text
MiniCableLab/
├── README.md
├── MiniCableLab_v1.ino
├── images/
│   ├── prototype.jpg
│   └── wiring.jpg
└── videos/
    └── (demo videos will be added here)
```

## What I learned

This project helped me practice:

- Arduino digital inputs and outputs
- `INPUT_PULLUP` and `HIGH` / `LOW`
- Continuity tests, open circuits and short circuits
- Buttons and LCD control
- Reading measurements and finding wiring problems

## Next improvements

- Make the test connectors easier to use and label them properly
- Improve the LCD contrast
- Make the code easier to extend
- Test 3, 4 or more conductors later

**Status:** Working breadboard prototype. The two modes, buttons, LCD and LEDs are working. I still want to document every fault with a video and improve the physical connectors.
