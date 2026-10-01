// ==========================================================
// SINGLE-PHASE FULL-BRIDGE INVERTER
// 50 Hz output
// 2 kHz SPWM
//
// PWM_A -> S1 and S4
// PWM_B -> S2 and S3
//
// Arduino UNO
// ==========================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD at standard address 0x27 (or change to 0x3F if your module uses that)
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define PWM_A 9
#define PWM_B 10
#define VOLT_PIN A0

const float F_OUT = 50.0;
const float F_CARRIER = 2000.0;

const float MOD_INDEX = 0.8;

// Number of carrier periods in one 50-Hz cycle
const int CYCLES = 40;

// Dead time in microseconds
const int DEAD_TIME = 10;

// Counter to periodically refresh the LCD without interrupting SPWM
int cycleCount = 0;

void setup()
{
  pinMode(PWM_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);

  digitalWrite(PWM_A, LOW);
  digitalWrite(PWM_B, LOW);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();
}

void loop()
{
  // One complete 50-Hz cycle
  for (int n = 0; n < CYCLES; n++)
  {
    // Angle for 50-Hz sine wave
    float theta = 2.0 * PI * n / CYCLES;

    float sineValue = sin(theta);

    // =====================================================
    // POSITIVE HALF CYCLE
    // S1 + S4
    // =====================================================

    if (sineValue >= 0)
    {
      // PWM_A controls S1 + S4
      // PWM_B remains OFF

      int duty = 128 + (int)(127.0 * MOD_INDEX * sineValue);

      analogWrite(PWM_B, 0);

      delayMicroseconds(DEAD_TIME);

      analogWrite(PWM_A, duty);
    }

    // =====================================================
    // NEGATIVE HALF CYCLE
    // S2 + S3
    // =====================================================

    else
    {
      // PWM_B controls S2 + S3
      // PWM_A remains OFF

      int duty = 128 + (int)(127.0 * MOD_INDEX * (-sineValue));

      analogWrite(PWM_A, 0);

      delayMicroseconds(DEAD_TIME);

      analogWrite(PWM_B, duty);
    }

    // One carrier period
    delayMicroseconds(500);
  }

  // =====================================================
  // VOLTAGE MEASUREMENT & LCD DISPLAY
  // Placed right after one full 50-Hz cycle completes
  // Updates every 50 cycles (~1 second interval)
  // =====================================================
  cycleCount++;
  if (cycleCount >= 50)
  {
    cycleCount = 0;

    int adcValue = analogRead(VOLT_PIN);
    // Standard 0-25V sensor uses a 5:1 divider (R1=30k, R2=7.5k)
    float voltage = (adcValue * 5.0 / 1024.0) * 5.0;

    lcd.setCursor(0, 0);
    lcd.print("Batt Volt:      ");
    lcd.setCursor(11, 0);
    lcd.print(voltage, 1);
    lcd.print("V");

    lcd.setCursor(0, 1);
    lcd.print("Inverter: ON 50Hz");
  }
}
