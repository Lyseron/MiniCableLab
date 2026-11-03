# MiniCableLab

MiniCableLab is a small cable tester I made with an **Arduino Uno**.

The idea is simple: before using a wire in another project, I want to check if it works and see what the problem is if it doesn't.

I started with a basic continuity test. Then I added a **16x2 LCD**, **two LEDs**, and **two buttons** so I can use it without opening the Arduino Serial Monitor every time.

> **Important:** This tester is for **disconnected, unpowered wires only**. Never connect it to a powered circuit, a battery, or mains electricity.

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

I want to show how MiniCableLab reacts to each wiring problem. Each short video will show the test connections, the button press, the LCD result, and the LED.

**How to add a video:** On GitHub, open `README.md` and click the pencil icon to edit it. Find the matching demo below. Drag your `.mp4` video into the editor, then replace the `<!-- VIDEO ... -->` comment with the **GitHub-generated video URL**, on its own line. Do not wrap the URL in `[Watch video](...)`. GitHub should display it as a video player in the README. Videos are not uploaded yet.

### 01. Single wire - All good

**Test:** Connect one working wire between D2 and D6, select `1 WIRE`, and press TEST.  
**Expected LCD:** `SINGLE: ALL GOOD`  
**LED:** Green.

<!-- VIDEO 01: Replace this comment with the GitHub-generated video URL. -->

### 02. Single wire - Open circuit

**Test:** Disconnect the wire between D2 and D6, select `1 WIRE`, and press TEST.  
**Expected LCD:** `SINGLE: OPEN`  
**LED:** Red.

<!-- VIDEO 02: Replace this comment with the GitHub-generated video URL. -->

### 03. Two wires - All good

**Test:** Connect D2 to D6 and D3 to D7, select `2 WIRES`, and press TEST.  
**Expected LCD:** `ALL GOOD !`  
**LED:** Green.

<!-- VIDEO 03: Replace this comment with the GitHub-generated video URL. -->

### 04. D2 to D6 - Open circuit

**Test:** Keep D3 to D7 connected, but disconnect D2 to D6. Press TEST in `2 WIRES` mode.  
**Expected LCD:** `D2-D6 CUT`  
**LED:** Red.

<!-- VIDEO 04: Replace this comment with the GitHub-generated video URL. -->

### 05. D3 to D7 - Open circuit

**Test:** Keep D2 to D6 connected, but disconnect D3 to D7. Press TEST in `2 WIRES` mode.  
**Expected LCD:** `D3-D7 CUT`  
**LED:** Red.

<!-- VIDEO 05: Replace this comment with the GitHub-generated video URL. -->

### 06. Both wires - Open circuit

**Test:** Disconnect both test wires. Press TEST in `2 WIRES` mode.  
**Expected LCD:** `BOTH CABLES CUT`  
**LED:** Red.

<!-- VIDEO 06: Replace this comment with the GitHub-generated video URL. -->

### 07. Inverted connections

**Test:** Cross the destinations: D2 to D7 and D3 to D6. Press TEST in `2 WIRES` mode.  
**Expected LCD:** `INVERTED CABLE`  
**LED:** Red.

<!-- VIDEO 07: Replace this comment with the GitHub-generated video URL. -->

### 08. Possible short circuit

**Test:** With the Arduino unplugged, connect D2 to D6 and D3 to D7, then add a temporary wire between the D6 and D7 test points to simulate an unwanted connection. Reconnect power and press TEST in `2 WIRES` mode.  
**Expected LCD:** `SHORT CIRCUIT`  
**LED:** Red.

<!-- VIDEO 08: Replace this comment with the GitHub-generated video URL. -->

### 09. Other fault (optional)

**Test:** Reproduce an unexpected connection that does not match the other diagnoses, if I find one that is reliable and safe to demonstrate.  
**Expected LCD:** `OTHER FAULT`  
**LED:** Red.

<!-- VIDEO 09: Replace this comment with the GitHub-generated video URL, or remove this section if not needed. -->

> **Safety:** I always unplug the Arduino USB cable before changing test connections. I only test passive, unpowered cables. I never short the 5V and GND power pins.
>
> **Limitations:** `CUT` means missing continuity, not proof of a physically broken conductor. `SHORT CIRCUIT` means a possible short or unexpected electrical path; different wiring faults can sometimes produce the same readings.

## Project files

```text
MiniCableLab/
├── README.md
├── MiniCableLab_v1.ino
└── images/
    ├── prototype.jpg
    └── wiring.jpg
```

The pictures have not been added yet. Demo videos will be uploaded as GitHub attachments and embedded in this README using their generated URLs, so a `videos/` folder is not required.

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
- Add photos, wiring diagrams, and embedded demo videos
- Make the code easier to extend
- Test 3, 4 or more conductors later

**Status:** Working breadboard prototype. The two modes, buttons, LCD and LEDs are working. I still want to document every fault with a video and improve the physical connectors.

