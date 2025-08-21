#include "components.h"
#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// --- AI Thinker ESP32-CAM pin map ---
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

// --- WiFi credentials ---
const char *WIFI_SSID = "Galaxy A34";
const char *WIFI_PASSWORD = "asdfghjkl";

WebServer server(80);

static const char INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>ESP32-CAM Capture</title>
    <style>
      body{font-family:system-ui,Segoe UI,Roboto,Arial;margin:24px}
      button{padding:10px 16px;font-size:16px}
      img{display:block;margin-top:16px;max-width:100%;height:auto}
      .ip{opacity:.7;font-size:14px}
    </style>
  </head>
  <body>
    <h2>ESP32-CAM Capture</h2>
    <div class="ip">Open <code>/capture</code> directly to download a JPEG.</div>
    <button onclick="capture()">Capture</button>
    <img id="photo" alt="Captured photo will appear here" />
    <script>
      let isCapturing = false;  // Prevent multiple captures

        async function capture(){
          if (isCapturing) return;  // Don't capture if already processing
    
        try{
          isCapturing = true;
          const r = await fetch('/capture',{cache:'no-store'});
          if(!r.ok){ alert('Capture failed: '+r.status); return; }
          const blob = await r.blob();
          document.getElementById('photo').src = URL.createObjectURL(blob);
        }catch(e){ alert('Capture error: '+e); }
        finally {
         isCapturing = false;
        }
      }
    </script>
  </body>
  </html>
)HTML";

void handleRoot()
{
  server.setContentLength(strlen(INDEX_HTML));
  server.send(200, "text/html", INDEX_HTML);
}

void handleCapture()
{
  Serial.println("Capture request received");

  // Add delay to control frame rate (lower FPS)
  delay(1000);

  camera_fb_t *frameBuffer = esp_camera_fb_get();
  if (!frameBuffer)
  {
    Serial.println("esp_camera_fb_get() failed - no frame buffer");
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }

  Serial.printf("Frame captured: %dx%d, %d bytes\n",
                frameBuffer->width, frameBuffer->height, frameBuffer->len);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.setContentLength(frameBuffer->len);
  server.send(200, "image/jpeg", "");

  WiFiClient client = server.client();
  client.setNoDelay(true); // send bytes ASAP
  int bytesWritten = client.write(frameBuffer->buf, frameBuffer->len);
  Serial.printf("Bytes written to client: %d\n", bytesWritten);

  esp_camera_fb_return(frameBuffer);
  Serial.println("Capture completed successfully");
}

void startCamera()
{
  delay(2000); // Wait 2 seconds for camera to stabilize

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000; // Changed from 20MHz to 10MHz
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())
  {
    config.frame_size = FRAMESIZE_QVGA; // Changed to QVGA (320x240)
    config.jpeg_quality = 50;           // Better quality
    config.fb_count = 1;
  }
  else
  {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 50;
    config.fb_count = 1;
  }

  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    Serial.println("Trying with even lower frequency...");

    // Try with 5MHz
    config.xclk_freq_hz = 5000000;
    err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
      Serial.printf("5MHz also failed: 0x%x\n", err);
      Serial.println("Check camera hardware connection!");
      for (;;)
      {
        delay(1000);
      }
    }
  }
  Serial.println("Camera initialized successfully!");
}
void startAccessPoint()
{
  const char *apSsid = "OPPO A74 5G"; // Changed from "ESP32-CAM"
  const char *apPass = "987654321";   // Changed from "12345678"
  bool ok = WiFi.softAP(apSsid, apPass, 6, 0, 4);
  if (ok)
  {
    IPAddress ip = WiFi.softAPIP();
    Serial.printf("Started AP '%s' (pass: %s)\nBrowse: http://%s/\n",
                  apSsid, apPass, ip.toString().c_str());
  }
  else
  {
    Serial.println("Failed to start SoftAP");
  }
}

void connectWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);                // disable power save to reduce latency
  WiFi.setTxPower(WIFI_POWER_19_5dBm); // max TX power for better link

  Serial.println("Scanning networks...");
  int n = WiFi.scanNetworks();
  if (n <= 0)
  {
    Serial.println("No networks found. Check 2.4GHz band and distance.");
  }
  else
  {
    for (int i = 0; i < n; ++i)
    {
      Serial.printf("%2d: %s (%ddBm) %s\n",
                    i + 1,
                    WiFi.SSID(i).c_str(),
                    WiFi.RSSI(i),
                    (WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "open" : "secured"));
    }
    bool targetFound = false;
    for (int i = 0; i < n; ++i)
    {
      if (WiFi.SSID(i) == WIFI_SSID)
      {
        targetFound = true;
        break;
      }
    }
    if (!targetFound)
    {
      Serial.println("Target SSID not found in scan (likely 5GHz or out of range). Ensure 2.4GHz hotspot.");
    }
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  unsigned long lastLog = 0;
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000)
  {
    Serial.print('.');
    delay(500);
    if (millis() - lastLog > 5000)
    {
      Serial.printf(" status=%d\n", WiFi.status());
      lastLog = millis();
    }
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("\nConnected. IP: http://%s\n", WiFi.localIP().toString().c_str());
  }
  else
  {
    Serial.println("\nWiFi join failed. Starting Access Point mode...");
    startAccessPoint();
  }
}

void setup()
{
  Serial.begin(115200);
  delay(200);

  startCamera();
  connectWiFi();

  server.setContentLength(0); // Disable content length for faster response
  server.enableCORS(true);    // Enable CORS for better browser compatibility

  server.on("/", HTTP_GET, handleRoot);
  server.on("/capture", HTTP_GET, handleCapture);
  server.begin();
  Serial.println("HTTP server started. Open / or /capture");
}

void loop()
{
  server.handleClient();
}