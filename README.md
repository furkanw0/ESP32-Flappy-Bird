# ESP32-Flappy-Bird
ESP32 ve TFT LCD ile geliştirilmiş Retro Oyun Konsolu


ESP32 Flappy Bird - Gömülü Sistem Projesi
Bu proje, ESP32 mikrodenetleyici ve ILI9341 TFT LCD ekran kullanılarak geliştirilmiş, donanım seviyesinde optimize edilmiş bir Flappy Bird klonudur. Proje, kısıtlı donanım kaynaklarıyla akıcı grafikler ve gerçek zamanlı fizik hesaplamaları yapmayı amaçlamaktadır.

- Teknik Özellikler & Optimizasyonlar
Partial Screen Refresh: Ekran titremesini (flickering) önlemek için tüm ekranı yeniden boyamak yerine sadece değişen piksellerin güncellendiği bir algoritma kullanıldı.

High Speed SPI: Ekran ile haberleşme hızı 40MHz'e optimize edilerek yüksek FPS değerlerine ulaşıldı.

NVS (Non-Volatile Storage): Yüksek skor verisi, ESP32'nin Preferences kütüphanesi aracılığıyla kalıcı hafızaya kaydedilir; cihaz kapansa bile skor silinmez.

Physics Engine: Karakter hareketleri için yerçekimi ivmesi ve sıçrama kuvveti hesaplamaları içeren basit bir fizik motoru entegre edildi.

- Donanım Bileşenleri
Mikrodenetleyici: ESP32-WROOM

Ekran: 2.8" ILI9341 TFT LCD (320x240)

Ses: Piezo Buzzer (PWM kontrollü)

Giriş: Push Button (Internal Pull-up)

- Bağlantı Şeması (Wiring)

TFT LCD Pin,   ESP32 Pin,Açıklama
VCC,           3.3V,Güç 
GND,           GND,Toprak
CS,            GPIO 15
RESET (RST),   GPIO 4
DC (RS),       GPIO 2
SDI (MOSI),    GPIO 23
SCK (CLK),     GPIO 18
LED,           3.3V
SDO (MISO),    GPIO 19


Zıplama Butonu (A): GPIO 13 (Bir ucu GPIO 13, diğer ucu GND)

Restart Butonu (B): GPIO 12 (Bir ucu GPIO 12, diğer ucu GND)

Buzzer: GPIO 25 (Artı ucu GPIO 25, eksi ucu GND)


