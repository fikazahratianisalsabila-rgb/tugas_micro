// === Definisi pin ===
#define lampu1 2
#define lampu2 3
#define lampu3 4
#define lampu4 5
#define tombolStart 8
#define tombolStop 9
#define pot A0

// === Variabel sistem ===
bool sistemAktif = false;

// === Setup awal ===
void setup() {
  pinMode(lampu1, OUTPUT);
  pinMode(lampu2, OUTPUT);
  pinMode(lampu3, OUTPUT);
  pinMode(lampu4, OUTPUT);
  pinMode(tombolStart, INPUT_PULLUP);
  pinMode(tombolStop, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.println("Sistem Lampu Jalan Cerdas Siap...");
}

// === Loop utama ===
void loop() {
  bool startPressed = (digitalRead(tombolStart) == LOW);
  bool stopPressed = (digitalRead(tombolStop) == LOW);

  if (startPressed) {
    sistemAktif = true;
    Serial.println("Sistem AKTIF");
    delay(300);
  }

  if (stopPressed) {
    sistemAktif = false;
    matikanSemuaLampu();
    Serial.println("Sistem DIMATIKAN oleh tombol STOP");
    delay(300);
  }

  if (sistemAktif) {
    jalankanSistemLampu();
  }
}

// === Fungsi menjalankan pola lampu ===
void jalankanSistemLampu() {
  int potValue = analogRead(pot);
  int kecepatan = map(potValue, 0, 1023, 200, 1000); // semakin kecil, semakin cepat

  // Pola 1: Lampu 1 & 2 ON, Lampu 3 & 4 OFF
  digitalWrite(lampu1, HIGH);
  digitalWrite(lampu2, HIGH);
  digitalWrite(lampu3, LOW);
  digitalWrite(lampu4, LOW);
  Serial.println("Lampu 1 dan 2 NYALA | Lampu 3 dan 4 MATI");
  tungguDenganStopCheck(kecepatan);

  // Pola 2: Lampu 3 & 4 ON, Lampu 1 & 2 OFF
  digitalWrite(lampu1, LOW);
  digitalWrite(lampu2, LOW);
  digitalWrite(lampu3, HIGH);
  digitalWrite(lampu4, HIGH);
  Serial.println("Lampu 3 dan 4 NYALA | Lampu 1 dan 2 MATI");
  tungguDenganStopCheck(kecepatan);
}

// === Fungsi tunggu dengan pengecekan tombol STOP ===
void tungguDenganStopCheck(int durasi) {
  unsigned long start = millis();
  while (millis() - start < durasi) {
    if (digitalRead(tombolStop) == LOW) {
      sistemAktif = false;
      matikanSemuaLampu();
      Serial.println("Sistem DIMATIKAN saat berjalan");
      delay(300);
      return;
    }
  }
}

// === Fungsi mematikan semua lampu ===
void matikanSemuaLampu() {
  digitalWrite(lampu1, LOW);
  digitalWrite(lampu2, LOW);
  digitalWrite(lampu3, LOW);
  digitalWrite(lampu4, LOW);
}
