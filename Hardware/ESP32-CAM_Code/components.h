#ifndef COMPONENTS_H
#define COMPONENTS_H

// Pin definitions for ESP32-CAM
#define CAMERA_MODEL_AI_THINKER

// Camera pins (already defined in main file, but kept for reference)
#if defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
#endif

// LED pin (built-in flash LED)
#define LED_PIN 4

// Button pin (for manual image capture)
#define BUTTON_PIN 12

// Buzzer pin (for audio alerts)
#define BUZZER_PIN 13

// Battery monitoring pin
#define BATTERY_PIN 14

// Function prototypes
void beep(int duration);
float readBatteryLevel();
void lowBatteryWarning();

// Buzzer functions
void beep(int duration)
{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
}

// Battery monitoring
float readBatteryLevel()
{
    int analogValue = analogRead(BATTERY_PIN);
    float voltage = (analogValue / 4095.0) * 3.3 * 2; // Voltage divider ratio
    return voltage;
}

void lowBatteryWarning()
{
    if (readBatteryLevel() < 3.5)
    {
        for (int i = 0; i < 3; i++)
        {
            beep(200);
            delay(200);
        }
    }
}

#endif