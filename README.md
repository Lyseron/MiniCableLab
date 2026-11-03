# MiniCableLab

MiniCableLab is a small cable tester I built with an **Arduino Uno**.

The goal is simple: before I use a wire in another project, I want to check if it works and quickly understand what the problem is if it does not.

I started with a basic continuity test, then I added:

- a **16x2 LCD**
- **2 LEDs**
- **2 push buttons**
- a **1-wire mode** and a **2-wire mode**

So now I can use it like a real small tool, not just a breadboard test.

> **Important:** MiniCableLab is only for **disconnected, unpowered wires**. Do **not** connect it to a powered circuit, a battery, or mains electricity.

---

## Project overview

At the beginning of the README, I want to show three real photos of the prototype:

<table>
  <tr>
    <td align="center"><img src="images/overview/lcd-button.jpg" alt="MiniCableLab LCD and buttons" width="300"><br><sub>LCD and buttons</sub></td>
    <td align="center"><img src="images/overview/full-prototype.jpg" alt="MiniCableLab prototype overview" width="300"><br><sub>Prototype overview</sub></td>
    <td align="center"><img src="images/overview/wires.jpg" alt="MiniCableLab test wires" width="300"><br><sub>Test wires / connections</sub></td>
    
  </tr>
</table>

**Note:** replace these three images with your own real photos using the exact same file names.

---

## What it can do

MiniCableLab has two modes:

- **1 WIRE**: check continuity between **D2** and **D6**
- **2 WIRES**: check two conductors and detect different wiring faults

The **MODE** button changes the mode.

The **TEST** button starts the measurement.

The result is shown on:

- the **LCD**
- the **green LED** if the test is OK
- the **red LED** if a fault is detected

---

## Main components

- Arduino Uno
- Breadboard
- LCD 1602 (16x2)
- 2 push buttons (**TEST** and **MODE**)
- 1 green LED
- 1 red LED
- 4 resistors of 1 kΩ
  - 2 for LED protection
  - 2 for the test outputs
- 1 resistor for LCD backlight
- Dupont wires

---

## Pin configuration

| Pin | Function |
| --- | --- |
| D2 | First test output |
| D3 | Second test output |
| D6 | First test input (`INPUT_PULLUP`) |
| D7 | Second test input (`INPUT_PULLUP`) |
| D5 | Green LED |
| D4 | Red LED |
| D10 | TEST button |
| D11 | MODE button |
| D8 | LCD RS |
| D9 | LCD E |
| A0 | LCD D4 |
| A1 | LCD D5 |
| A2 | LCD D6 |
| A3 | LCD D7 |

LCD power:

- `VSS` → GND
- `VDD` → 5V
- `VO` → GND (fixed contrast for now)
- `RW` → GND
- LCD backlight through a resistor

---

## How it works

MiniCableLab sends a **LOW** signal on one output at a time and checks whether that signal reaches the expected input.

The inputs use `INPUT_PULLUP`, so:

- **HIGH** means no LOW signal arrived
- **LOW** means the signal was detected through an electrical path

This lets the program detect:

- correct wiring
- open circuits
- swapped connections
- possible short circuits
- other unexpected cases

---

## Diagnostic visuals

The following visuals explain each test result.

**Important note:** the schematic visuals below were **generated with ChatGPT** based on my MiniCableLab project, to make the README cleaner and easier to read.

### 01 — Two Wires All Good

Both conductors are correctly connected.

<img src="images/diagrams/All_good.png" alt="Two wires all good" width="450">

### 02 — Single Wire OK

The wire has continuity between **D2** and **D6**.

<img src="images/diagrams/Single_ok.png" alt="Single connections ok" width="450">

### 03 — Single Wire Open

No continuity is detected between **D2** and **D6**.

<img src="images/diagrams/Single_open.png" alt="Single connections open" width="450">

### 04 — D2-D6 Cut

The connection between **D2** and **D6** is open.

<img src="images/diagrams/D2_D6_Cut.png" alt="D2 and D6 Cut" width="450">

### 05 — D3-D7 Cut

The connection between **D3** and **D7** is open.

<img src="images/diagrams/D3_D7_Cut.png" alt="D3 and D7 Cut" width="450">

### 06 — Both Cables Cut

No continuity is detected on either connection.

<img src="images/diagrams/Both_Cut.png" alt="Both connections cut" width="450">

### 07 — Inverted Connections

The two conductors are connected to the wrong inputs.

<img src="images/diagrams/Inverted.png" alt="Inverted connections" width="450">

### 08 — Short Circuit

An unexpected connection is detected between the two conductors.

<img src="images/diagrams/Short_circuit.png" alt="Short circuit" width="450">

### 09 — Other Fault

The readings do not match the expected cases.

<img src="images/diagrams/Other_fault.png" alt="Other fault" width="450">

---

## Real demo video

- the **MODE** button changing between **1 WIRE** and **2 WIRES**
- the **TEST** button starting a measurement
- the LCD updating the result
- the LEDs changing depending on the result

---

## What I learned

This project helped me practice:

- Arduino digital inputs and outputs
- `INPUT_PULLUP`
- continuity tests
- open circuits and possible short circuits
- LCD display control
- push buttons and mode selection
- turning a breadboard prototype into a usable tool

---

## Next improvements

- make the test connectors cleaner and easier to use
- improve the physical case / presentation
- add support for more conductors later
- improve the LCD contrast if needed
- maybe make a more compact version in the future

---

## Project status

**Current status:** working breadboard prototype.

MiniCableLab can already:

- test **one wire**
- test **two wires**
- display the result on the LCD
- use a **MODE** button and a **TEST** button
- indicate the result with green and red LEDs
