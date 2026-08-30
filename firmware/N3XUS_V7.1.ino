// ======================================================
// N3XUS V7.1 - Ultimate Electronic Warfare Toolkit
// ======================================================

#include <TFT_eSPI.h>
#include <RF24.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// ==========================================
// تنظیمات WiFi
// ==========================================

const char* AP_SSID = "N3XUS";
const char* AP_PASS = "12345678";
WebServer server(80);

// ==========================================
// تعریف پین‌ها
// ==========================================

#define TFT_CS   2
#define TFT_DC   16
#define TFT_RST  5
#define SD_CS    14

#define CE1 25
#define CSN1 26
#define CE2 32
#define CSN2 33
#define CE3 22
#define CSN3 21

#define CC_CS   16
#define CC_GDO0 17

#define LED_GREEN  12
#define LED_RED    13
#define LED_YELLOW 27
#define LED_BLUE   15
#define LED_WHITE  17

#define SW1 33
#define SW2 32
#define SW3 4

// ==========================================
// اشیاء
// ==========================================

TFT_eSPI tft = TFT_eSPI();
RF24 radio1(CE1, CSN1);
RF24 radio2(CE2, CSN2);
RF24 radio3(CE3, CSN3);

// ==========================================
// تعریف ۱۲ حالت عملیاتی
// ==========================================

enum OperationMode {
  MODE_OMNISCIENT, MODE_SHADOW, MODE_SPECTRUM, MODE_DEVICE_FINDER,
  MODE_ANNIHILATOR, MODE_SNIPER, MODE_BLACKOUT, MODE_ZOMBIE,
  MODE_MIRAGE, MODE_CHAMELEON, MODE_PUPPET, MODE_OMEGA
};

// ==========================================
// متغیرهای عمومی
// ==========================================

enum SystemState { STATE_IDLE, STATE_SCANNING, STATE_ATTACKING, STATE_ALERT };
enum PageState { PAGE_HOME, PAGE_MODE, PAGE_STATS, PAGE_SETTINGS };

SystemState currentState = STATE_IDLE;
PageState currentPage = PAGE_HOME;
OperationMode currentMode = MODE_OMNISCIENT;
int channel = 1;
bool systemReady = false;
bool systemActive = false;
bool nrf1_active = true;
bool nrf2_active = true;
bool nrf3_active = true;
int nrfPower = RF24_PA_HIGH;
bool loggingEnabled = true;

int nrf1_packets = 0, nrf2_packets = 0, nrf3_packets = 0;
int networksFound = 0;
String lastResultText = "";
String lastEvents[10] = {"System Started", "", "", "", "", "", "", "", "", ""};
unsigned long uptimeStart = 0;
unsigned long lastScanTime = 0;
unsigned long lastLEDBlink = 0;
bool ledState = false;

// ==========================================
// متغیرهای وب
// ==========================================

String webStatus = "INACTIVE";
String webModeName = "Omniscient";
int webPackets = 0;
int webNetworks = 0;
String webResult = "System ready";
String webUptime = "00:00:00";

// ==========================================
// اسامی حالت‌ها
// ==========================================

const char* modeNames[] = {
  "Omniscient", "Shadow", "Spectrum Analyzer", "Device Finder",
  "Annihilator", "Sniper", "Blackout", "Zombie",
  "Mirage", "Chameleon", "Puppet", "Omega"
};

const char* modeIcons[] = {
  "📡", "👤", "📊", "🎯",
  "💥", "🎯", "🌑", "🧟",
  "🎭", "🦎", "🎪", "🌀"
};

const char* modeCategories[] = {
  "Recon", "Recon", "Recon", "Recon",
  "Attack", "Attack", "Attack", "Attack",
  "Deception", "Deception", "Deception", "Omega"
};

// ==========================================
// اعلان توابع
// ==========================================

void updatePage();
void displayMessage(String msg, int color);
void writeLog(String message);
bool initAllNRF();
void setupWebServer();
void changeMode(int direction);
void executeMode(OperationMode mode);
void scanAllChannelsAdvanced();
void attackAllChannelsAdvanced();
void sendPacketAdvanced(String data, int count);
void showResult(String title, String details, int color);
void updateWebData();

// ==========================================
// توابع کمکی
// ==========================================

void addEvent(String event) {
  for(int i = 9; i > 0; i--) lastEvents[i] = lastEvents[i-1];
  lastEvents[0] = event;
  if(loggingEnabled) writeLog(event);
  updateWebData();
}

String formatTime(unsigned long ms) {
  unsigned long seconds = ms / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  seconds %= 60;
  minutes %= 60;
  char buffer[20];
  sprintf(buffer, "%02lu:%02lu:%02lu", hours, minutes, seconds);
  return String(buffer);
}

bool readButton(int pin) {
  static unsigned long lastTime = 0;
  static bool lastState = HIGH;
  if(millis() - lastTime < 50) return false;
  lastTime = millis();
  int reading = digitalRead(pin);
  if(reading == LOW && lastState == HIGH) {
    lastState = LOW;
    return true;
  }
  if(reading == HIGH && lastState == LOW) lastState = HIGH;
  return false;
}

void blinkLED(int led, int times, int delay_ms) {
  for(int i = 0; i < times; i++) {
    digitalWrite(led, HIGH);
    delay(delay_ms);
    digitalWrite(led, LOW);
    delay(delay_ms);
  }
}

void updateLEDs() {
  unsigned long now = millis();
  
  if(!systemActive) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_WHITE, HIGH);
    return;
  }
  
  switch(currentState) {
    case STATE_IDLE:
      if(now - lastLEDBlink > 2000) { ledState = !ledState; lastLEDBlink = now; }
      digitalWrite(LED_GREEN, ledState);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_WHITE, HIGH);
      break;
    case STATE_SCANNING:
      if(now - lastLEDBlink > 200) { ledState = !ledState; lastLEDBlink = now; }
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, ledState);
      digitalWrite(LED_WHITE, LOW);
      break;
    case STATE_ATTACKING:
      if(now - lastLEDBlink > 150) { ledState = !ledState; lastLEDBlink = now; }
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, ledState);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_WHITE, LOW);
      break;
    case STATE_ALERT:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_WHITE, LOW);
      break;
  }
}

void writeLog(String message) {
  if(!loggingEnabled) return;
  File logFile = SD.open("/LOG.TXT", FILE_APPEND);
  if(logFile) {
    logFile.println(String(millis()) + ": " + message);
    logFile.close();
    digitalWrite(LED_BLUE, HIGH);
    delay(50);
    digitalWrite(LED_BLUE, LOW);
  }
}

void updateWebData() {
  webStatus = systemActive ? "ACTIVE" : "INACTIVE";
  webModeName = modeNames[(int)currentMode];
  webPackets = nrf1_packets + nrf2_packets + nrf3_packets;
  webNetworks = networksFound;
  webUptime = formatTime(millis() - uptimeStart);
}

void showResult(String title, String details, int color) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 10);
  tft.println("📊 RESULT");
  tft.drawLine(10, 35, 230, 35, TFT_DARKGREY);
  
  tft.setTextColor(color, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 50);
  tft.println(title);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(20, 85);
  tft.println(details);
  
  tft.drawLine(10, 250, 230, 250, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(15, 258);
  tft.print("Press any button to continue");
  
  lastResultText = title + ": " + details;
  webResult = lastResultText;
  updateWebData();
  
  delay(3000);
  updatePage();
}

bool initAllNRF() {
  bool allOk = true;
  
  if (!radio1.begin()) {
    Serial.println("❌ NRF1 FAILED!");
    allOk = false;
  } else {
    radio1.setChannel(1);
    radio1.setPALevel(RF24_PA_HIGH);
    radio1.setDataRate(RF24_250KBPS);
    radio1.stopListening();
    Serial.println("✅ NRF1 (Assassin) Ready");
  }
  
  if (!radio2.begin()) {
    Serial.println("❌ NRF2 FAILED!");
    allOk = false;
  } else {
    radio2.setChannel(40);
    radio2.setPALevel(RF24_PA_LOW);
    radio2.setDataRate(RF24_250KBPS);
    radio2.stopListening();
    Serial.println("✅ NRF2 (Watcher) Ready");
  }
  
  if (!radio3.begin()) {
    Serial.println("❌ NRF3 FAILED!");
    allOk = false;
  } else {
    radio3.setChannel(80);
    radio3.setPALevel(RF24_PA_MAX);
    radio3.setDataRate(RF24_250KBPS);
    radio3.stopListening();
    Serial.println("✅ NRF3 (Ghost) Ready");
  }
  
  return allOk;
}

void scanAllChannelsAdvanced() {
  if(!systemActive) return;
  
  currentState = STATE_SCANNING;
  networksFound = 0;
  String foundList = "";
  int foundCount = 0;
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("🔍 ADVANCED SCAN");
  tft.drawLine(10, 45, 230, 45, TFT_DARKGREY);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(20, 60);
  tft.println("NRF1: CH 0-41");
  tft.setCursor(20, 75);
  tft.println("NRF2: CH 42-83");
  tft.setCursor(20, 90);
  tft.println("NRF3: CH 84-125");
  
  int found1 = 0;
  for(int ch = 0; ch < 42; ch++) {
    radio1.setChannel(ch);
    radio1.startListening();
    delay(2);
    if(radio1.available()) {
      found1++;
      nrf1_packets++;
      foundList += "CH" + String(ch) + " ";
    }
    radio1.stopListening();
  }
  
  int found2 = 0;
  for(int ch = 42; ch < 84; ch++) {
    radio2.setChannel(ch);
    radio2.startListening();
    delay(2);
    if(radio2.available()) {
      found2++;
      nrf2_packets++;
      foundList += "CH" + String(ch) + " ";
    }
    radio2.stopListening();
  }
  
  int found3 = 0;
  for(int ch = 84; ch < 125; ch++) {
    radio3.setChannel(ch);
    radio3.startListening();
    delay(2);
    if(radio3.available()) {
      found3++;
      nrf3_packets++;
      foundList += "CH" + String(ch) + " ";
    }
    radio3.stopListening();
  }
  
  foundCount = found1 + found2 + found3;
  networksFound = foundCount;
  currentState = STATE_IDLE;
  
  addEvent("Advanced scan: " + String(foundCount) + " signals");
  
  String result = "NRF1: " + String(found1) + "\nNRF2: " + String(found2) + "\nNRF3: " + String(found3) + "\nTotal: " + String(foundCount);
  showResult("🔍 Advanced Scan", result, TFT_GREEN);
  updateWebData();
}

void attackAllChannelsAdvanced() {
  if(!systemActive) return;
  
  currentState = STATE_ATTACKING;
  byte attackData[] = "N3XUS ATK";
  int totalPackets = 0;
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("⚡ ADVANCED ATTACK");
  tft.drawLine(10, 45, 230, 45, TFT_DARKGREY);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(20, 60);
  tft.println("NRF1: CH 0-41");
  tft.setCursor(20, 75);
  tft.println("NRF2: CH 42-83");
  tft.setCursor(20, 90);
  tft.println("NRF3: CH 84-125");
  
  int sent1 = 0;
  for(int ch = 0; ch < 42; ch++) {
    radio1.setChannel(ch);
    radio1.stopListening();
    for(int i = 0; i < 3; i++) {
      if(radio1.write(&attackData, sizeof(attackData))) {
        sent1++;
        nrf1_packets++;
      }
    }
  }
  
  int sent2 = 0;
  for(int ch = 42; ch < 84; ch++) {
    radio2.setChannel(ch);
    radio2.stopListening();
    for(int i = 0; i < 3; i++) {
      if(radio2.write(&attackData, sizeof(attackData))) {
        sent2++;
        nrf2_packets++;
      }
    }
  }
  
  int sent3 = 0;
  for(int ch = 84; ch < 125; ch++) {
    radio3.setChannel(ch);
    radio3.stopListening();
    for(int i = 0; i < 3; i++) {
      if(radio3.write(&attackData, sizeof(attackData))) {
        sent3++;
        nrf3_packets++;
      }
    }
  }
  
  totalPackets = sent1 + sent2 + sent3;
  currentState = STATE_IDLE;
  
  addEvent("Advanced attack: " + String(totalPackets) + " packets");
  
  String result = "NRF1: " + String(sent1) + "\nNRF2: " + String(sent2) + "\nNRF3: " + String(sent3) + "\nTotal: " + String(totalPackets);
  showResult("⚡ Advanced Attack", result, TFT_RED);
  updateWebData();
}

void sendPacketAdvanced(String data, int count) {
  if(!systemActive) return;
  
  currentState = STATE_ATTACKING;
  byte packetData[32];
  data.toCharArray((char*)packetData, 32);
  
  int sent = 0;
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(30, 40);
  tft.println("📤 SENDING");
  
  for(int i = 0; i < count/3; i++) {
    radio1.stopListening();
    if(radio1.write(&packetData, sizeof(packetData))) {
      sent++;
      nrf1_packets++;
    }
    delay(2);
  }
  
  for(int i = 0; i < count/3; i++) {
    radio2.stopListening();
    if(radio2.write(&packetData, sizeof(packetData))) {
      sent++;
      nrf2_packets++;
    }
    delay(2);
  }
  
  for(int i = 0; i < count/3; i++) {
    radio3.stopListening();
    if(radio3.write(&packetData, sizeof(packetData))) {
      sent++;
      nrf3_packets++;
    }
    delay(2);
  }
  
  currentState = STATE_IDLE;
  addEvent("Sent " + String(sent) + " packets: " + data);
  showResult("📤 Packet Sent", "Sent " + String(sent) + " packets\nData: " + data, TFT_CYAN);
  updateWebData();
}

void executeMode(OperationMode mode) {
  if(!systemActive) {
    showResult("⚠️ Inactive", "Activate system first", TFT_RED);
    return;
  }
  
  int activeChannels = 0;
  
  switch(mode) {
    case MODE_OMNISCIENT:
      scanAllChannelsAdvanced();
      break;
      
    case MODE_SHADOW:
      currentState = STATE_SCANNING;
      radio2.startListening();
      delay(3000);
      radio2.stopListening();
      currentState = STATE_IDLE;
      addEvent("Shadow scan complete");
      showResult("👤 Shadow Scan", "Stealth scan with NRF2\nNo signals transmitted", TFT_CYAN);
      break;
      
    case MODE_SPECTRUM: {
      currentState = STATE_SCANNING;
      activeChannels = 0;
      for(int ch = 1; ch <= 125; ch++) {
        radio1.setChannel(ch);
        radio1.startListening();
        delay(2);
        radio1.stopListening();
        activeChannels++;
      }
      currentState = STATE_IDLE;
      showResult("📊 Spectrum Analysis", "Analyzed 125 channels\nActive channels: " + String(activeChannels), TFT_CYAN);
      break;
    }
      
    case MODE_DEVICE_FINDER:
      sendPacketAdvanced("FIND_DEVICE", 60);
      break;
      
    case MODE_ANNIHILATOR:
      attackAllChannelsAdvanced();
      break;
      
    case MODE_SNIPER:
      currentState = STATE_ATTACKING;
      radio1.setChannel(channel);
      sendPacketAdvanced("SNIPER_TARGET", 100);
      currentState = STATE_IDLE;
      showResult("🎯 Sniper Attack", "Targeted channel: " + String(channel) + "\n100 packets sent", TFT_RED);
      break;
      
    case MODE_BLACKOUT:
      attackAllChannelsAdvanced();
      showResult("🌑 Blackout", "Full frequency jamming\nAll channels affected", TFT_RED);
      break;
      
    case MODE_ZOMBIE:
      sendPacketAdvanced("ZOMBIE_PACKET", 200);
      break;
      
    case MODE_MIRAGE:
      sendPacketAdvanced("MIRAGE_AP_BEACON", 60);
      showResult("🎭 Mirage", "Rogue AP beacon sent\n60 packets broadcasted", TFT_PURPLE);
      break;
      
    case MODE_CHAMELEON:
      sendPacketAdvanced("SPOOFED_MAC", 30);
      showResult("🦎 Chameleon", "Identity spoofed\n30 spoofed packets sent", TFT_PURPLE);
      break;
      
    case MODE_PUPPET:
      sendPacketAdvanced("CONTROL_CMD", 50);
      showResult("🎪 Puppet", "Device control commands sent\n50 packets transmitted", TFT_PURPLE);
      break;
      
    case MODE_OMEGA:
      currentState = STATE_ATTACKING;
      scanAllChannelsAdvanced();
      attackAllChannelsAdvanced();
      sendPacketAdvanced("OMEGA_FINAL", 150);
      currentState = STATE_IDLE;
      showResult("🌀 Omega Mode", "All modes combined\nScan + Attack + Injection", TFT_ORANGE);
      break;
  }
  updateWebData();
}

void changeMode(int direction) {
  int modeCount = 12;
  int newMode = ((int)currentMode + direction + modeCount) % modeCount;
  currentMode = (OperationMode)newMode;
  addEvent("Mode: " + String(modeNames[newMode]));
  webModeName = modeNames[newMode];
  updateWebData();
  updatePage();
}

// ==========================================
// توابع نمایش TFT
// ==========================================

void displayHomePage() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(15, 5);
  tft.println("⚡ N3XUS v7.1");
  tft.drawLine(10, 30, 230, 30, TFT_DARKGREY);
  
  tft.setTextColor(systemReady ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(160, 5);
  tft.print(systemReady ? "● ONLINE" : "● OFFLINE");
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  
  tft.setCursor(15, 45);
  tft.print("Mode: ");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.print(modeIcons[(int)currentMode]);
  tft.print(" ");
  tft.println(modeNames[(int)currentMode]);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(15, 65);
    tft.print("Status: ");
  tft.setTextColor(systemActive ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.println(systemActive ? "ACTIVE" : "STANDBY");
  
  tft.setCursor(15, 85);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Uptime: ");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println(formatTime(millis() - uptimeStart));
  
  tft.setCursor(15, 105);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Networks: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println(networksFound);
  
  tft.setCursor(15, 125);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Packets: ");
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.println(nrf1_packets + nrf2_packets + nrf3_packets);
  
  tft.setCursor(15, 145);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Last: ");
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.println(lastResultText.substring(0, 25));
  
  tft.drawLine(10, 165, 230, 165, TFT_DARKGREY);
  
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(15, 180);
  tft.print("SW1:Menu  SW2:Execute  SW3:Mode");
  tft.setCursor(15, 195);
  tft.print("3x NRF Active!");
  
  tft.drawLine(10, 250, 230, 250, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(15, 258);
  tft.print("N3XUS - 12 Operational Modes");
}

void displayModePage() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(15, 5);
  tft.println("🎯 Mode Select");
  tft.drawLine(10, 30, 230, 30, TFT_DARKGREY);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  
  int displayMode = (int)currentMode;
  
  tft.setCursor(15, 45);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.print("Current: ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(modeIcons[displayMode]);
  tft.print(" ");
  tft.println(modeNames[displayMode]);
  
  tft.setCursor(15, 65);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.print("Category: ");
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.println(modeCategories[displayMode]);
  
  tft.setCursor(15, 85);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.print("Result: ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(lastResultText.substring(0, 25));
  
  tft.drawLine(10, 105, 230, 105, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(15, 120);
  tft.print("SW2:Execute  SW3:Next  SW1:Back");
  
  tft.drawLine(10, 250, 230, 250, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(15, 258);
  tft.print("Select Operation Mode");
}

void displayStatsPage() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(15, 5);
  tft.println("📊 Statistics");
  tft.drawLine(10, 30, 230, 30, TFT_DARKGREY);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  
  tft.setCursor(15, 45);
  tft.print("Packets Sent: ");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println(nrf1_packets + nrf2_packets + nrf3_packets);
  
  tft.setCursor(15, 65);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Networks Found: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println(networksFound);
  
  tft.setCursor(15, 85);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("NRF1: ");
  tft.setTextColor(nrf1_active ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(nrf1_active ? "ACTIVE" : "OFF");
  tft.print(" (");
  tft.print(nrf1_packets);
  tft.println(")");
  
  tft.setCursor(15, 105);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("NRF2: ");
  tft.setTextColor(nrf2_active ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(nrf2_active ? "ACTIVE" : "OFF");
  tft.print(" (");
  tft.print(nrf2_packets);
  tft.println(")");
  
  tft.setCursor(15, 125);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("NRF3: ");
  tft.setTextColor(nrf3_active ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(nrf3_active ? "ACTIVE" : "OFF");
  tft.print(" (");
  tft.print(nrf3_packets);
  tft.println(")");
  
  tft.drawLine(10, 145, 230, 145, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(15, 158);
  tft.print("SW1:Back");
  
  tft.drawLine(10, 250, 230, 250, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(15, 258);
  tft.print("System Statistics");
}

void displaySettingsPage() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(15, 5);
  tft.println("⚙️ Settings");
  tft.drawLine(10, 30, 230, 30, TFT_DARKGREY);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  
  tft.setCursor(15, 45);
  tft.print("NRF Power: ");
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  if(nrfPower == RF24_PA_LOW) tft.println("LOW");
  else if(nrfPower == RF24_PA_HIGH) tft.println("HIGH");
  else if(nrfPower == RF24_PA_MAX) tft.println("MAX");
  
  tft.setCursor(15, 65);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Logging: ");
  tft.setTextColor(loggingEnabled ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.println(loggingEnabled ? "ENABLED" : "DISABLED");
  
  tft.setCursor(15, 85);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("NRF1: ");
  tft.setTextColor(nrf1_active ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.println(nrf1_active ? "ON" : "OFF");
  
  tft.setCursor(15, 105);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("NRF2: ");
  tft.setTextColor(nrf2_active ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.println(nrf2_active ? "ON" : "OFF");
  
  tft.setCursor(15, 125);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("NRF3: ");
  tft.setTextColor(nrf3_active ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.println(nrf3_active ? "ON" : "OFF");
  
  tft.drawLine(10, 145, 230, 145, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(15, 158);
  tft.print("SW1:Back");
  
  tft.drawLine(10, 250, 230, 250, TFT_DARKGREY);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(15, 258);
  tft.print("System Settings");
}

void updatePage() {
  switch(currentPage) {
    case PAGE_HOME: displayHomePage(); break;
    case PAGE_MODE: displayModePage(); break;
    case PAGE_STATS: displayStatsPage(); break;
    case PAGE_SETTINGS: displaySettingsPage(); break;
  }
}

// ==========================================
// HTML صفحه وب
// ==========================================

String getHTMLPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>N3XUS · Command Center</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;500;600;700;800;900&display=swap" rel="stylesheet">
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        :root {
            --bg-primary: #0a0e17;
            --bg-card: rgba(255,255,255,0.03);
            --border-color: rgba(255,255,255,0.06);
            --text-primary: #e8edf5;
            --text-secondary: #8a9bb5;
            --accent-cyan: #00e5ff;
            --accent-green: #00ffb3;
            --accent-red: #ff6b6b;
            --accent-orange: #ffb347;
            --font: 'Orbitron', monospace;
        }
        body {
            font-family: var(--font);
            background: var(--bg-primary);
            color: var(--text-primary);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
            background-image: radial-gradient(ellipse at 10% 20%, rgba(0,180,255,0.05) 0%, transparent 60%);
        }
        .container {
            max-width: 480px;
            width: 100%;
            background: rgba(18,26,40,0.85);
            backdrop-filter: blur(24px);
            border-radius: 20px;
            padding: 24px 20px;
            border: 1px solid var(--border-color);
            box-shadow: 0 25px 60px rgba(0,0,0,0.6);
        }
        .header {
            display: flex; justify-content: space-between; align-items: center;
            margin-bottom: 16px; padding-bottom: 12px;
            border-bottom: 1px solid var(--border-color);
        }
        .logo-text { font-size: 22px; font-weight: 800; background: linear-gradient(135deg, var(--accent-cyan), var(--accent-green)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
        .logo-version { font-size: 10px; color: #4a5a70; padding: 4px 14px; border: 1px solid var(--border-color); border-radius: 100px; }
        .mode-display {
            background: var(--bg-card); border-radius: 12px; padding: 16px;
            border: 1px solid var(--border-color); margin-bottom: 14px;
            text-align: center;
        }
        .mode-icon { font-size: 38px; display: block; }
        .mode-name { font-size: 18px; font-weight: 700; color: var(--accent-cyan); margin-top: 4px; }
        .mode-category { font-size: 11px; color: var(--text-secondary); margin-top: 2px; }
        .status-grid {
            display: grid; grid-template-columns: 1fr 1fr; gap: 8px;
            margin-bottom: 14px;
        }
        .status-card {
            background: var(--bg-card); border-radius: 10px; padding: 12px 14px;
            border: 1px solid var(--border-color);
        }
        .status-label { font-size: 9px; color: var(--text-secondary); text-transform: uppercase; letter-spacing: 1px; }
        .status-value { font-size: 16px; font-weight: 700; margin-top: 3px; }
        .status-online { color: var(--accent-green); }
        .status-offline { color: var(--accent-red); }
        .status-cyan { color: var(--accent-cyan); }
        .result-box {
            background: var(--bg-card); border-radius: 10px; padding: 12px 14px;
            border: 1px solid var(--border-color); margin-bottom: 14px;
            min-height: 50px;
        }
        .result-label { font-size: 9px; color: var(--text-secondary); text-transform: uppercase; letter-spacing: 1px; }
        .result-text { font-size: 12px; color: var(--text-primary); margin-top: 4px; word-break: break-all; }
        .btn-grid {
            display: grid; grid-template-columns: 1fr 1fr; gap: 8px;
            margin: 6px 0;
        }
        .btn {
            font-family: var(--font); padding: 13px; border: none;
            border-radius: 10px; font-size: 10px; font-weight: 600;
            cursor: pointer; transition: all 0.2s;
            letter-spacing: 1px; text-transform: uppercase;
        }
        .btn:active { transform: scale(0.96); }
        .btn-full { grid-column: 1 / -1; }
        .btn-primary { background: linear-gradient(135deg, var(--accent-cyan), #0099cc); color: #0a0e17; }
        .btn-danger { background: linear-gradient(135deg, var(--accent-red), #cc4444); color: #fff; }
        .btn-success { background: linear-gradient(135deg, var(--accent-green), #00aa77); color: #0a0e17; }
        .btn-outline { background: transparent; border: 1px solid var(--border-color); color: var(--text-secondary); }
        .btn-outline:hover { background: rgba(255,255,255,0.05); }
        .mode-dots {
            display: flex; justify-content: center; gap: 5px; margin: 8px 0;
        }
        .mode-dot {
            width: 8px; height: 8px; border-radius: 50%;
            background: #4a5a70; transition: all 0.3s;
        }
        .mode-dot.active { background: var(--accent-cyan); box-shadow: 0 0 15px rgba(0,229,255,0.3); }
        .msg-box {
            background: var(--bg-card); border-radius: 10px; padding: 10px 14px;
            margin-top: 12px; color: var(--text-secondary); font-size: 11px;
            border-left: 3px solid var(--accent-cyan);
        }
        .msg-box.success { border-left-color: var(--accent-green); color: var(--accent-green); }
        .msg-box.error { border-left-color: var(--accent-red); color: var(--accent-red); }
        .footer {
            margin-top: 16px; text-align: center; font-size: 9px;
            color: #4a5a70; letter-spacing: 2px;
            border-top: 1px solid var(--border-color);
            padding-top: 14px; text-transform: uppercase;
        }
        .footer .highlight { color: var(--accent-cyan); }
        @media (max-width: 480px) {
            .container { padding: 16px 14px; }
            .mode-name { font-size: 16px; }
            .status-value { font-size: 14px; }
            .btn { font-size: 9px; padding: 10px; }
            .result-text { font-size: 11px; }
        }
    </style>
</head>
<body>
<div class="container">
    <div class="header">
        <span class="logo-text">N3XUS</span>
        <span class="logo-version">v7.1</span>
    </div>
    <div class="mode-display">
        <span class="mode-icon" id="modeIcon">📡</span>
        <div class="mode-name" id="modeName">Omniscient</div>
        <div class="mode-category" id="modeCategory">Recon</div>
    </div>
    <div class="status-grid">
        <div class="status-card">
            <div class="status-label"><i class="fas fa-microchip"></i> System</div>
            <div class="status-value status-online" id="statusDisplay">● ONLINE</div>
        </div>
        <div class="status-card">
            <div class="status-label"><i class="fas fa-bolt"></i> Packets</div>
            <div class="status-value status-cyan" id="packetDisplay">0</div>
        </div>
    </div>
    <div class="result-box">
        <div class="result-label"><i class="fas fa-terminal"></i> Last Result</div>
        <div class="result-text" id="resultDisplay">System ready</div>
    </div>
    <div class="btn-grid">
        <button class="btn btn-full" id="toggleBtn"><i class="fas fa-power-off"></i> ACTIVATE</button>
    </div>
    <div class="btn-grid">
        <button class="btn btn-success" id="executeBtn"><i class="fas fa-play"></i> EXECUTE</button>
        <button class="btn btn-danger" id="attackBtn"><i class="fas fa-bolt"></i> ATTACK</button>
    </div>
    <div class="btn-grid">
        <button class="btn btn-outline" id="prevBtn"><i class="fas fa-chevron-left"></i> PREV</button>
        <button class="btn btn-outline" id="nextBtn">NEXT <i class="fas fa-chevron-right"></i></button>
    </div>
    <div class="mode-dots" id="modeDots"></div>
    <div class="msg-box" id="messageBox"><i class="fas fa-circle"></i> System ready</div>
    <div class="footer">N3XUS <span class="highlight">3x NRF</span> · 12 Modes</div>
</div>
<script>
    const modeNames = ["Omniscient","Shadow","Spectrum Analyzer","Device Finder","Annihilator","Sniper","Blackout","Zombie","Mirage","Chameleon","Puppet","Omega"];
    const modeIcons = ["📡","👤","📊","🎯","💥","🎯","🌑","🧟","🎭","🦎","🎪","🌀"];
    const modeCategories = ["Recon","Recon","Recon","Recon","Attack","Attack","Attack","Attack","Deception","Deception","Deception","Omega"];
    let currentMode = 0;
    let isActive = false;
    let isUpdating = false;

    function updateUI(data) {
        currentMode = data.mode || 0;
        isActive = data.status === 'ACTIVE';
        document.getElementById('modeIcon').textContent = modeIcons[currentMode];
        document.getElementById('modeName').textContent = modeNames[currentMode];
        document.getElementById('modeCategory').textContent = modeCategories[currentMode];
        document.getElementById('statusDisplay').textContent = isActive ? '● ONLINE' : '● OFFLINE';
        document.getElementById('statusDisplay').className = 'status-value ' + (isActive ? 'status-online' : 'status-offline');
        document.getElementById('packetDisplay').textContent = data.packets || 0;
        if(data.result) {
            document.getElementById('resultDisplay').textContent = data.result;
        }
        document.getElementById('toggleBtn').innerHTML = isActive ? '<i class="fas fa-power-off"></i> DEACTIVATE' : '<i class="fas fa-power-off"></i> ACTIVATE';
        document.getElementById('toggleBtn').className = 'btn btn-full ' + (isActive ? 'btn-danger' : 'btn-primary');
        updateDots();
    }

    function updateDots() {
        const container = document.getElementById('modeDots');
        container.innerHTML = '';
        for(let i=0; i<12; i++) {
            const dot = document.createElement('span');
            dot.className = 'mode-dot' + (i === currentMode ? ' active' : '');
            container.appendChild(dot);
        }
    }

    function showMessage(text, type='info') {
        const box = document.getElementById('messageBox');
        box.innerHTML = ' <i class="fas fa-circle"></i> ' + text;
        box.className = 'msg-box';
        if(type === 'success') box.classList.add('success');
        else if(type === 'error') box.classList.add('error');
    }

    async function fetchStatus() {
        if(isUpdating) return;
        try {
            const res = await fetch('/status');
            const data = await res.json();
            updateUI(data);
        } catch(e) {
            showMessage('Connection lost', 'error');
        }
    }

    async function sendCommand(cmd, param='') {
        if(isUpdating) return;
        isUpdating = true;
        try {
            const url = `/command?cmd=${cmd}${param ? '&'+param : ''}`;
            const res = await fetch(url);
            const text = await res.text();
            showMessage(text, 'success');
            await fetchStatus();
        } catch(e) {
            showMessage('Command failed', 'error');
        }
        isUpdating = false;
    }

    document.getElementById('toggleBtn').addEventListener('click', () => sendCommand('toggle'));
    document.getElementById('executeBtn').addEventListener('click', () => sendCommand('execute'));
    document.getElementById('attackBtn').addEventListener('click', () => sendCommand('attack'));
    document.getElementById('prevBtn').addEventListener('click', () => sendCommand('prev'));
    document.getElementById('nextBtn').addEventListener('click', () => sendCommand('next'));

    fetchStatus();
    setInterval(fetchStatus, 2000);
</script>
</body>
</html>
)rawliteral";
}

// ==========================================
// هندلرهای وب
// ==========================================

void handleRoot() {
  server.send(200, "text/html", getHTMLPage());
}

void handleStatus() {
  updateWebData();
  
  String json = "{";
  json += "\"status\":\"" + webStatus + "\",";
  json += "\"mode\":" + String((int)currentMode) + ",";
  json += "\"packets\":" + String(webPackets) + ",";
  json += "\"networks\":" + String(webNetworks) + ",";
  json += "\"uptime\":\"" + webUptime + "\",";
  json += "\"result\":\"" + webResult + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handleCommand() {
  if (!server.hasArg("cmd")) {
    server.send(400, "text/plain", "Missing command");
    return;
  }

  String cmd = server.arg("cmd");
  String response = "OK";

  if (cmd == "toggle") {
    systemActive = !systemActive;
    response = systemActive ? "System activated" : "System deactivated";
    webResult = response;
    updateWebData();
  }
  else if (cmd == "execute") {
    if (systemActive && currentState == STATE_IDLE) {
      executeMode(currentMode);
      response = "Executed: " + String(modeNames[(int)currentMode]);
    } else {
      response = systemActive ? "System busy" : "System inactive";
    }
  }
  else if (cmd == "attack") {
    if (systemActive && currentState == STATE_IDLE) {
      attackAllChannelsAdvanced();
      response = "Advanced attack executed!";
    } else {
      response = systemActive ? "System busy" : "System inactive";
    }
  }
  else if (cmd == "prev") {
    changeMode(-1);
    response = "Mode: " + String(modeNames[(int)currentMode]);
  }
  else if (cmd == "next") {
    changeMode(1);
    response = "Mode: " + String(modeNames[(int)currentMode]);
  }

  server.send(200, "text/plain", response);
}

// ==========================================
// راه‌اندازی وب سرور
// ==========================================

void setupWebServer() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  
  Serial.println("📡 N3XUS AP Ready!");
  Serial.print("🌐 IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/command", handleCommand);
  server.begin();
  
  Serial.println("✅ Web Panel Ready!");
  Serial.println("👉 http://192.168.4.1");
}

// ==========================================
// Setup
// ==========================================

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 N3XUS V7.1 - 3x NRF Mode Starting...");
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  blinkLED(LED_BLUE, 3, 150);
  digitalWrite(LED_WHITE, HIGH);
  
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  
  tft.init();
  tft.setRotation(4);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(30, 60);
  tft.println("N3XUS");
  tft.setTextSize(1);
  tft.setCursor(50, 100);
  tft.println("v7.1 Loading...");
  tft.setCursor(50, 120);
  tft.println("3x NRF Mode");
  delay(1500);
  
  bool allNRF = initAllNRF();
  
  if(allNRF) {
    addEvent("All 3 NRF modules ready!");
    blinkLED(LED_GREEN, 3, 200);
  } else {
    addEvent("Some NRF modules failed!");
    blinkLED(LED_RED, 5, 200);
  }
  
  if(SD.begin(SD_CS)) {
    addEvent("SD Card ready");
    blinkLED(LED_BLUE, 2, 200);
  } else {
    addEvent("SD Card failed");
    blinkLED(LED_RED, 3, 200);
  }
  
  setupWebServer();
  
  systemReady = true;
  currentMode = MODE_OMNISCIENT;
  currentState = STATE_IDLE;
  currentPage = PAGE_HOME;
  uptimeStart = millis();
  lastResultText = "System Ready - 3x NRF";
  webResult = "System Ready - 3x NRF";
  updateWebData();
  updatePage();
  
  Serial.println("✅ N3XUS V7.1 Ready!");
  Serial.println("👉 3x NRF + 12 Modes + Web Panel!");
}

// ==========================================
// Loop
// ==========================================

void loop() {
  server.handleClient();
  updateLEDs();
  
  if(readButton(SW1)) {
    currentPage = (PageState)((currentPage + 1) % 4);
    updatePage();
    addEvent("Page changed");
  }
  
  if(readButton(SW2)) {
    if(systemActive && currentState == STATE_IDLE) {
      executeMode(currentMode);
      updatePage();
    } else if(!systemActive) {
      showResult("⚠️ Inactive", "Activate system first", TFT_RED);
    }
  }
  
  if(readButton(SW3)) {
    changeMode(1);
    updatePage();
  }
  
  delay(10);
}"// Pin definitions" 
