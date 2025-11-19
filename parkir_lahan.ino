#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
Servo palang;

int ledMerah = 2;
int ledHijau = 3;
int tombolMasuk = 5;
int tombolKeluar = 4;

int sisaArea = 5;
int maxArea = 5;

bool tampilStatus = false;
unsigned long waktuTampil = 0;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();

  palang.attach(8);

  pinMode(ledMerah, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  pinMode(tombolMasuk, INPUT_PULLUP);
  pinMode(tombolKeluar, INPUT_PULLUP);

  // POSISI AWAL PALANG
  palang.write(0);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Sistem Lahan");
  lcd.setCursor(5, 1);
  lcd.print("Parkir");
  delay(4000);
  lcd.clear();

  // TAMPILAN AWAL
  lcd.print("Sisa Area: ");
  lcd.print(sisaArea);

  Serial.println("Sistem Parkir Aktif");
  Serial.print("Sisa Area Awal: ");
  Serial.println(sisaArea);
}

void bukaPalang() {
  Serial.println("Palang dibuka...");
  for (int i = 0; i <= 90; i++) {
    palang.write(i);
    delay(15);
  }
}

void tutupPalang() {
  Serial.println("Palang ditutup...");
  for (int i = 90; i >= 0; i--) {
    palang.write(i);
    delay(15);
  }
}

void loop() {

  bool masuk = digitalRead(tombolMasuk) == LOW;
  bool keluar = digitalRead(tombolKeluar) == LOW;

  // MOBIL MASUK
  if (masuk && !tampilStatus) {
    Serial.println("Tombol MASUK ditekan");

    if (sisaArea > 0) {
      sisaArea--;

      Serial.print("Mobil masuk → Sisa area: ");
      Serial.println(sisaArea);

      Serial.print("Sisa Area Sekarang: ");
      Serial.println(sisaArea);

      digitalWrite(ledMerah, HIGH);
      digitalWrite(ledHijau, LOW);

      bukaPalang();
      delay(1000);
      tutupPalang();

      lcd.clear();
      lcd.print("Mobil Masuk");
      lcd.setCursor(0, 1);
      lcd.print("Sisa Area: ");
      lcd.print(sisaArea);
    } 
    else {
      Serial.println("Parkir penuh!");
      lcd.clear();
      lcd.print("Parkir Penuh!");
      lcd.setCursor(0, 1);
      lcd.print("Tunggu Keluar");
    }

    tampilStatus = true;
    waktuTampil = millis();
  }

  // MOBIL KELUAR
  if (keluar && !tampilStatus) {
    Serial.println("Tombol KELUAR ditekan");

    if (sisaArea < maxArea) {
      sisaArea++;

      Serial.print("Mobil keluar → Sisa area: ");
      Serial.println(sisaArea);

      Serial.print("Sisa Area Sekarang: ");
      Serial.println(sisaArea);

      digitalWrite(ledMerah, LOW);
      digitalWrite(ledHijau, HIGH);

      bukaPalang();
      delay(1000);
      tutupPalang();

      lcd.clear();
      lcd.print("Mobil Keluar");
      lcd.setCursor(0, 1);
      lcd.print("Sisa Area: ");
      lcd.print(sisaArea);
    }

    tampilStatus = true;
    waktuTampil = millis();
  }

  // KEMBALI KE TAMPILAN DEFAULT
  if (tampilStatus && millis() - waktuTampil >= 3000) {

    lcd.clear();
    lcd.print("Sisa Area: ");
    lcd.print(sisaArea);

    Serial.print("Tampilan default → Sisa Area: ");
    Serial.println(sisaArea);

    tampilStatus = false;
  }

  // TAMPILAN SAAT DIAM
  if (!tampilStatus) {
    lcd.setCursor(0, 0);
    lcd.print("Sisa Area: ");
    lcd.print(sisaArea);
  }
}
