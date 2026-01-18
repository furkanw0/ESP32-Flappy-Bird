#include <TFT_eSPI.h>
#include <Preferences.h>

TFT_eSPI tft = TFT_eSPI();
Preferences prefs;

// --- AYARLAR VE DEĞİŞKENLER ---
float kusY = 120;
float hiz = 0;
float yercekimi = 0.22;
float ziplama = -4.2;
const int kusX = 50;

int boruX = 320;
int boruBoslukY = 85;
int boruUstYukseklik = 60;
int boruGenislik = 40;
int oyunHizi = 3;

int puan = 0;
int rekor = 0;
bool oyunBitti = false;

const int ustPanelYukseklik = 30; 
const int ziplaButon = 13;
const int restartButon = 12;
const int buzzerPin = 25;

// --- SES FONKSİYONLARI ---
void sesZipla() { tone(buzzerPin, 1000, 30); }
void sesPuan() { tone(buzzerPin, 1800, 100); }
void sesYanma() { tone(buzzerPin, 150, 400); }

// --- KARAKTER ÇİZİMİ ---
void kusCiz(int x, int y) {
  tft.fillEllipse(x + 10, y + 8, 12, 10, TFT_YELLOW);
  tft.drawEllipse(x + 10, y + 8, 12, 10, TFT_BLACK);
  tft.fillEllipse(x + 5, y + 10, 6, 4, 0xFFE0); 
  tft.drawEllipse(x + 5, y + 10, 6, 4, TFT_BLACK);
  tft.fillCircle(x + 16, y + 5, 4, TFT_WHITE);
  tft.drawCircle(x + 16, y + 5, 4, TFT_BLACK);
  tft.fillCircle(x + 17, y + 5, 1, TFT_BLACK);
  tft.fillTriangle(x+19, y+7, x+19, y+13, x+26, y+10, TFT_ORANGE);
  tft.drawTriangle(x+19, y+7, x+19, y+13, x+26, y+10, TFT_BLACK);
}

// --- BORU ÇİZİMİ ---
void boruCiz(int x, int y, int g, int h, bool ustMu) {
  uint16_t koyuYesil = 0x03E0;
  uint16_t acikYesil = 0x07E0;
  if (ustMu) {
    tft.fillRect(x, ustPanelYukseklik + 1, g, h, koyuYesil);
    tft.fillRect(x + 4, ustPanelYukseklik + 1, 5, h, acikYesil);
    tft.drawRect(x, ustPanelYukseklik + 1, g, h, TFT_BLACK);
    tft.fillRect(x - 4, ustPanelYukseklik + h - 14, g + 8, 15, koyuYesil);
    tft.drawRect(x - 4, ustPanelYukseklik + h - 14, g + 8, 15, TFT_BLACK);
  } else {
    tft.fillRect(x, y, g, 240 - y, koyuYesil);
    tft.fillRect(x + 4, y, 5, 240 - y, acikYesil);
    tft.drawRect(x, y, g, 240 - y, TFT_BLACK);
    tft.fillRect(x - 4, y, g + 8, 15, koyuYesil);
    tft.drawRect(x - 4, y, g + 8, 15, TFT_BLACK);
  }
}

void setup() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_SKYBLUE);
  
  pinMode(ziplaButon, INPUT_PULLUP);
  pinMode(restartButon, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  prefs.begin("flappy", false);
  rekor = prefs.getInt("rekor", 0);
  
  randomSeed(analogRead(0));
}

void loop() {
  if (!oyunBitti) {
    // --- 1. SİLME (Partial Refresh) ---
    tft.fillRect(kusX - 2, (int)kusY - 4, 32, 25, TFT_SKYBLUE); 
    tft.fillRect(boruX + boruGenislik + 2, ustPanelYukseklik + 1, oyunHizi + 2, 210, TFT_SKYBLUE);

    // --- 2. HAREKET VE KONTROL ---
    if (digitalRead(ziplaButon) == LOW) {
      hiz = ziplama;
      sesZipla();
    }
    hiz += yercekimi;
    kusY += hiz;
    boruX -= oyunHizi;

    if (boruX < -boruGenislik - 10) {
      boruX = 320;
      boruUstYukseklik = random(30, 100);
      puan++;
      sesPuan();
    }

    // --- 3. ÇARPIŞMA KONTROLÜ ---
    if (kusY > 225 || kusY < ustPanelYukseklik) { 
      oyunBitti = true; 
      sesYanma(); 
    }
    if (boruX < (kusX + 22) && (boruX + boruGenislik) > kusX) {
      if (kusY < (ustPanelYukseklik + boruUstYukseklik) || (kusY + 16) > (ustPanelYukseklik + boruUstYukseklik + boruBoslukY)) {
        oyunBitti = true;
        sesYanma();
      }
    }

    // --- 4. ÇİZİM ---
    boruCiz(boruX, 0, boruGenislik, boruUstYukseklik, true);
    boruCiz(boruX, ustPanelYukseklik + boruUstYukseklik + boruBoslukY, boruGenislik, 0, false);
    kusCiz(kusX, (int)kusY);

    // ÜST SKOR PANELİ
    tft.fillRect(0, 0, 320, ustPanelYukseklik, 0x2104); 
    tft.drawFastHLine(0, ustPanelYukseklik, 320, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("Skor: "); tft.print(puan);
    tft.setCursor(180, 8);
    tft.print("Rekor: "); tft.print(rekor);

    delay(15); 
  } else {
    // --- OYUN BİTTİ EKRANI ---
    if (puan > rekor) {
      rekor = puan;
      prefs.putInt("rekor", rekor);
    }

    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(60, 70);
    tft.println("GAME OVER");
    tft.setTextSize(2);
    tft.setCursor(85, 120);
    tft.print("Skor: "); tft.println(puan);
    tft.setCursor(85, 150);
    tft.print("Rekor: "); tft.println(rekor);
    tft.setCursor(40, 200);
    tft.println("Restart: B Butonu");

    while(digitalRead(12) == HIGH) delay(10);
    while(digitalRead(12) == LOW) delay(10);

    // Reset İşlemi
    puan = 0; 
    kusY = 120; 
    hiz = 0; 
    boruX = 320;
    oyunBitti = false;
    tft.fillScreen(TFT_SKYBLUE);
  }
}