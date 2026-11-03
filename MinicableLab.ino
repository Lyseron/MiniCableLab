/* MiniCableLab -> One or Two conductor Cable tester
  D2 -> D6 : First conductor
  D3 -> D7 : Second conductor

  The tester can check for:
    - Correct connections
    - Swapped connections
    - Missing continuity (open circuits)
    - Possible short circuits
    - Other unexpected wiring configurations
*/

#include <LiquidCrystal.h>

// Define the LCD control and data pins.
// LCD RS -> Arduino D8
// LCD E  -> Arduino D9
// LCD D4 -> Arduino A0
// LCD D5 -> Arduino A1
// LCD D6 -> Arduino A2
// LCD D7 -> Arduino A3
LiquidCrystal lcd(8, 9, A0, A1, A2, A3);

// Define the Arduino pins used to read the test result
const int D6_PIN = 6;
const int D2_PIN = 2;

// Define the Arduino pins used to generate test signals
const int D3_PIN = 3;
const int D7_PIN = 7;

// Define the pins connected to the status LEDs
const int GREEN_LED = 5;
const int RED_LED = 4;

// To start the testing process
const int TEST_BUTTON = 10;

// To change for 1 wire check to 2
const int MODE_BUTTON = 11;
bool modeOneWire = true;

void setup() {
  // Start serial communication at 9600 bits per second -> For diagnostic messages in the Serial Monitor
  Serial.begin(9600);

// Set the output level of D2 & D3 to LOW (0V)
  digitalWrite(D2_PIN, LOW);
  digitalWrite(D3_PIN, LOW);

// Configure D2 & D3 as INPUT initially, to keep them inactive until their test begin
  pinMode(D2_PIN, INPUT);
  pinMode(D3_PIN, INPUT);

// Configure D6 & D7 as INPUT_PULLUP -> (INPUT with an internal pull-up resistor to keep D6 & D7 HIGH when no LOW signal reaches them)
  pinMode(D6_PIN, INPUT_PULLUP);
  pinMode(D7_PIN, INPUT_PULLUP);

// Configurate the LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

// Configurate the buttons to INPUT_PULLUP
  pinMode(TEST_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);

// Initialize the LCD with 16 columns and 2 rows.
  lcd.begin(16, 2);

 // Display the project name on the first row.
  lcd.setCursor(0, 0);
  lcd.print("MiniCableLab");

}

void mode1Wire()
{
  const char* lcdMsg = "OTHER FAULT";
  // Activate D2 (OUTPUT)
  pinMode(D2_PIN, OUTPUT);
  // Wait 1 milisecond for the electrical signals to settle
  delay(1);
  // Read D6 & D7 when D2 is active (OUTPUT)
  int state_d2_d6 = digitalRead(D6_PIN);
  // Desactivate D2 (INPUT) to prevent it from interfering with the next test
  pinMode(D2_PIN, INPUT);

  bool allGoodOneWire = 
  (state_d2_d6 == LOW);

  // Clear the second LCD row before writing.
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);

  if (allGoodOneWire == true)
  {
    lcdMsg = "SINGLE: ALL GOOD ! ";
    Serial.println("Single: All good");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  else
  {
    lcdMsg = "SINGLE: NO CONTINUITY";
    Serial.println("Single: No continuity");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  // Print the msg on the screen on the second line
  lcd.setCursor(0, 1);
  lcd.print(lcdMsg);

  Serial.println("---------------------------------");
}

void mode2Wire()
{
  // TEST 1 : Activate D2 and read D6 & D7

  // Activate D2 (OUTPUT)
  pinMode(D2_PIN, OUTPUT);
  // Wait 1 milisecond for the electrical signals to settle
  delay(1);
  // Read D6 & D7 when D2 is active (OUTPUT)
  int state_d2_d6 = digitalRead(D6_PIN);
  int state_d2_d7 = digitalRead(D7_PIN);
  // Desactivate D2 (INPUT) to prevent it from interfering with the next test
  pinMode(D2_PIN, INPUT);

// TEST 2 : Activate D3 and read D6 & D7
  pinMode(D3_PIN, OUTPUT);
  delay(1);
  int state_d3_d6 = digitalRead(D6_PIN);
  int state_d3_d7 = digitalRead(D7_PIN);
  pinMode(D3_PIN, INPUT);

// Diagnosis possible:

  // 
  bool allGood = 
    (state_d2_d6 == LOW && state_d2_d7 == HIGH
    && state_d3_d6 == HIGH && state_d3_d7 == LOW);

  // Open circuit -> interrupted electrical path, possible cause:
    // - A broken wire inside the cable
    // - Disconnected connector
  bool thread_D2_D6Cut = 
    (state_d2_d6 == HIGH && state_d2_d7 == HIGH
    && state_d3_d7 == LOW && state_d3_d6 == HIGH);
  bool thread_D3_D7Cut = 
    (state_d3_d7 == HIGH && state_d3_d6 == HIGH
    && state_d2_d6 == LOW && state_d2_d7 == HIGH);
  bool bothCut = 
    (state_d2_d6 == HIGH && state_d2_d7 == HIGH 
    && state_d3_d7 == HIGH && state_d3_d6 == HIGH);

  // Inverted connections -> when the connections are swapped, possible cause:
    // - Incorrect connector wiring
    // - Incorrect pin placement
    // - Assembly mistakes
  bool invertedCable = 
    (state_d2_d6 == HIGH && state_d2_d7 == LOW
    && state_d3_d6 == LOW && state_d3_d7 == HIGH);

  // Short circuit -> when two points become electrically connected when they should be isolated, possible cause:
    // - Damage insulation
    // - Two exposed wires touching each other
    // - Solder bridging two connector contacts
    // - Incorrect wiring during assembly
    // Only a possibility !
  bool shortCircuit = 
    (state_d2_d6 == LOW && state_d2_d7 == LOW
    || state_d3_d6 == LOW && state_d3_d7 == LOW);

  const char* lcdMsg = "OTHER FAULT";
// Display the diagnosis:
  // - GREEN LED ON -> Everthing works as expected
  // - RED LED ON -> An error occurs and a message is display on the screen
  if (allGood == true)
  {
    lcdMsg = "ALL GOOD ! ";
    Serial.println("ALL GOOD");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  else if (invertedCable == true)
  {
    lcdMsg = "INVERTED CABLE";
    Serial.println("Inverted Cable");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else if (bothCut == true)
  {
    lcdMsg = "BOTH CABLES CUT";
    Serial.println("Both cables cut");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else if (thread_D2_D6Cut == true)
  {
    lcdMsg = "D2-D6 CUT";
    Serial.println("Cut cable between d2 et d6");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else if (thread_D3_D7Cut == true)
  {
    lcdMsg = "D3-D7 CUT";
    Serial.println("Cut cable between d3 et d7");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else if (shortCircuit == true)
  {
    lcdMsg = "SHORT CIRCUIT";
    Serial.println("Short circuit possible");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else
  {
    lcdMsg = "OTHER FAULT";
    Serial.println("Other fault detected");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
// Print the msg on the screen on the second line
  lcd.setCursor(0, 1);
  lcd.print(lcdMsg);

  Serial.println("---------------------------------");

}

void loop() {
  
  int modeButtonState = digitalRead(MODE_BUTTON);
  if (modeButtonState == LOW)
  {
    // Switch between the two mode
    modeOneWire = !modeOneWire;
  
    // Turn off the previous diagnostic LEDs
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    // Clear the second LCD row before writing.
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    if (modeOneWire == true)
      lcd.print("MODE: 1 WIRE");
    else
      lcd.print("MODE: 2 WIRES");
    // Wait until the button is release
    while (digitalRead(MODE_BUTTON) == LOW)
      delay(10);
    // Wait 500 miliseconds to avoid contact bouncing
    delay(30);
  }
  int testButtonState = digitalRead(TEST_BUTTON);
  if (testButtonState == LOW)
  {
    if (modeOneWire == true)
      mode1Wire();
    else
      mode2Wire();
     // Wait until the button is release
    while (digitalRead(TEST_BUTTON) == LOW)
      delay(10);
    // Wait 500 miliseconds to avoid contact bouncing
    delay(30);
  }
}





