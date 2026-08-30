docs/setup_guide.md
markdown
# راهنمای گام‌به‌گام راه‌اندازی N3XUS

## 🛠️ مرحله ۱: آماده‌سازی قطعات

### لیست قطعات مورد نیاز:
- ESP32 (30 یا 38 پین)
- ۳ عدد NRF24+PA+LNA (با آنتن)
- ۱ عدد CC1101 (با آنتن)
- ۱ عدد TFT 1.69 اینچی (ST7789)
- ۱ عدد ماژول SD Card Reader
- ۵ عدد LED (سبز، قرمز، زرد، آبی، سفید) + مقاومت ۲۲۰Ω
- ۳ عدد دکمه فشاری
- ۱ عدد رگولاتور AMS1117 3.3V
- خازن‌های ۱۰µF، ۱۰۰µF، ۲۲µF
- بردبورد و سیم‌های جامپر

---

## 🔌 مرحله ۲: اتصالات پایه

### ۲-۱. تغذیه
شارژر ۵V ← AMS1117 (VIN)
AMS1117 (VOUT) ← ۳.۳V به ردیف VCC بردبورد
AMS1117 (GND) ← GND به ردیف GND بردبورد

text

### ۲-۲. اتصالات SPI مشترک
GPIO 18 (SCK) ← همه قطعات SPI
GPIO 23 (MOSI) ← همه قطعات SPI
GPIO 19 (MISO) ← همه قطعات SPI

text

---

## 📡 مرحله ۳: اتصال NRFها

| ماژول | CE | CSN |
|-------|-----|-----|
| NRF1 (Assassin) | GPIO 25 | GPIO 26 |
| NRF2 (Watcher) | GPIO 32 | GPIO 33 |
| NRF3 (Ghost) | GPIO 22 | GPIO 21 |

> ⚠️ برای هر NRF یک خازن ۱۰µF بین VCC و GND نصب کنید.

---

## 🖥️ مرحله ۴: اتصال TFT

| پین TFT | ESP32 |
|---------|-------|
| VCC | ۳.۳V |
| GND | GND |
| SCL | GPIO 18 |
| SDA | GPIO 23 |
| CS | GPIO 2 |
| DC | GPIO 16 |
| RES | GPIO 5 |
| BLK | ۳.۳V |

---

## 📻 مرحله ۵: اتصال CC1101

| پین CC1101 | ESP32 |
|------------|-------|
| VCC | ۳.۳V |
| GND | GND |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| CS | GPIO 16 |
| GDO0 | GPIO 17 |

---

## 💾 مرحله ۶: اتصال SD Card

| پین SD | ESP32 |
|--------|-------|
| VCC | ۳.۳V |
| GND | GND |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| CS | GPIO 14 |

---

## 💡 مرحله ۷: اتصال LEDها

| LED | ESP32 | مقاومت به GND |
|-----|-------|---------------|
| سبز 🟢 | GPIO 12 | ۲۲۰Ω |
| قرمز 🔴 | GPIO 13 | ۲۲۰Ω |
| زرد 🟡 | GPIO 27 | ۲۲۰Ω |
| سفید ⚪ | GPIO 15 | ۲۲۰Ω |
| آبی 🔵 | GPIO 34 | ۲۲۰Ω |

---

## 🔘 مرحله ۸: اتصال دکمه‌ها

| دکمه | ESP32 | سر دیگر به |
|------|-------|------------|
| SW1 (Menu) | GPIO 33 | GND |
| SW2 (Up) | GPIO 32 | GND |
| SW3 (Down) | GPIO 4 | GND |

---

## 💻 مرحله ۹: نصب نرم‌افزار

### ۹-۱. نصب کتابخانه‌ها
```bash
TFT_eSPI
RF24
ELECHOUSE_CC1101
SD
SPI
WiFi
WebServer
۹-۲. تنظیم TFT_eSPI
فایل User_Setup.h را ویرایش کنید:

cpp
#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 280
#define TFT_CS 2
#define TFT_DC 16
#define TFT_RST 5
#define TFT_SCLK 18
#define TFT_MOSI 23
۹-۳. آپلود کد
فایل N3XUS_V7.1.ino را باز کنید

برد: ESP32 Dev Module

پورت: COMx

کلیک Upload

🌐 مرحله ۱۰: استفاده از پنل وب
به شبکه N3XUS متصل شوید (رمز: 12345678)

مرورگر را باز کنید: http://192.168.4.1

سیستم را ACTIVATE کنید

حالت مورد نظر را انتخاب کنید

روی EXECUTE کلیک کنید"## Web Interface" 
