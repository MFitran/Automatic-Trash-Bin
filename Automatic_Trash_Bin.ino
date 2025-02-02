#include <Servo.h>

#define RGB_RED A5         // Register DDRC digunakan untuk menetapkan pin ini sebagai output
#define RGB_GREEN A4       // Register DDRC digunakan untuk menetapkan pin ini sebagai output
#define TRIG1_PIN 9        // Register DDRB digunakan untuk menetapkan pin ini sebagai output
#define ECHO1_PIN 10       // Register DDRB digunakan untuk menetapkan pin ini sebagai input
#define TRIG2_PIN 13       // Register DDRB digunakan untuk menetapkan pin ini sebagai output
#define ECHO2_PIN 12       // Register DDRB digunakan untuk menetapkan pin ini sebagai input
#define SERVO_PIN 11       // Register Timer1 (TCCR1A, TCCR1B) digunakan untuk mengontrol PWM
#define SWITCH_PIN A3      // Register DDRC digunakan untuk menetapkan pin ini sebagai input

Servo myServo;

bool isOpen = false; // Status untuk melacak apakah servo pada tutup sampah terbuka atau tertutup

void setup() {
  // Monitor Serial
  Serial.begin(9600);

  // Switch on/off
  pinMode(SWITCH_PIN, INPUT); // DDRC digunakan untuk menetapkan SWITCH_PIN sebagai input

  // LED RGB
  pinMode(RGB_RED, OUTPUT);   // DDRC digunakan untuk menetapkan RGB_RED sebagai output
  pinMode(RGB_GREEN, OUTPUT); // DDRC digunakan untuk menetapkan RGB_GREEN sebagai output

  // Sensor luar
  pinMode(TRIG1_PIN, OUTPUT); // DDRB digunakan untuk menetapkan TRIG1_PIN sebagai output
  pinMode(ECHO1_PIN, INPUT);  // DDRB digunakan untuk menetapkan ECHO1_PIN sebagai input

  // Sensor dalam
  pinMode(TRIG2_PIN, OUTPUT); // DDRB digunakan untuk menetapkan TRIG2_PIN sebagai output
  pinMode(ECHO2_PIN, INPUT);  // DDRB digunakan untuk menetapkan ECHO2_PIN sebagai input

  // Servo
  myServo.attach(SERVO_PIN);  // TCCR1A dan TCCR1B digunakan untuk menghasilkan sinyal PWM
  myServo.write(180);         // Mengatur register OCR1A untuk menggerakkan servo ke posisi tertutup

  digitalWrite(RGB_RED, LOW);  // PORTC digunakan untuk mematikan LED merah
  digitalWrite(RGB_GREEN, LOW); // PORTC digunakan untuk mematikan LED hijau
}

int calculateDistance(int trigPin, int echoPin) {
  long duration;
  int distance;

  // Reset pin trigger
  digitalWrite(trigPin, LOW); // PORTx digunakan untuk mengatur pin trigger ke LOW
  delayMicroseconds(2);       // Delay berbasis Timer0 (TCCR0A, TCCR0B)

  // Mengaktifkan pin trigger HIGH selama 10 mikrodetik
  digitalWrite(trigPin, HIGH); // PORTx digunakan untuk mengatur pin trigger ke HIGH
  delayMicroseconds(10);       // Delay berbasis Timer0
  digitalWrite(trigPin, LOW);  // PORTx digunakan untuk mengatur pin trigger ke LOW

  // Mengukur durasi sinyal pantulan dari pin echo
  duration = pulseIn(echoPin, HIGH); // Timer1 digunakan untuk menghitung durasi

  // Menghitung jarak sensor HCSR-04 (cm)
  distance = duration * 0.034 / 2;

  return distance;
}

// Fungsi untuk mendeteksi keberadaan tangan
bool detectHand() {
  const int numReadings = 5; // Jumlah pembacaan untuk rata-rata
  int total = 0;

  for (int i = 0; i < numReadings; i++) {
    total += calculateDistance(TRIG1_PIN, ECHO1_PIN);
    delay(5); // Delay berbasis Timer0 (TCCR0A, TCCR0B)
  }

  int averageDistance = total / numReadings;

  // Tampilkan jarak tangan yang terdeteksi di Monitor Serial
  Serial.print("Jarak Deteksi Tangan: ");
  Serial.print(averageDistance);
  Serial.println(" cm");

  return averageDistance < 30; // Tangan terdeteksi jika jarak rata-rata kurang dari 30 cm
}

// Fungsi untuk mengecek apakah tempat sampah penuh
bool isTrashBinFull() {
  const int numReadings = 5; // Jumlah pembacaan untuk rata-rata
  int total = 0;

  for (int i = 0; i < numReadings; i++) {
    total += calculateDistance(TRIG2_PIN, ECHO2_PIN);
    delay(5); // Delay berbasis Timer0 (TCCR0A, TCCR0B)
  }

  int averageDistance = total / numReadings;

  // Tampilkan jarak tempat sampah penuh di Monitor Serial
  Serial.print("Jarak Sampah Penuh: ");
  Serial.print(averageDistance);
  Serial.println(" cm");

  return averageDistance < 10; // Tempat sampah penuh jika jarak rata-rata kurang dari 10 cm
}

void loop() {
  if(analogRead(SWITCH_PIN) < 620) { // PINC digunakan untuk membaca nilai analog dari SWITCH_PIN
    Serial.print(analogRead(SWITCH_PIN));
    Serial.println(" System offline");
    delay(1000); // Delay berbasis Timer0

    if (isOpen) { // Hanya tutup jika sedang terbuka
      myServo.write(180);  // Menutup servo, memengaruhi register OCR1A
      isOpen = false;      // Memperbarui status
      delay(1000);         // Delay berbasis Timer0
    }

    return;
  }

  if (isTrashBinFull()) {
    // Tunjukkan bahwa tempat sampah penuh dengan LED merah
    if (detectHand()) {
      digitalWrite(RGB_RED, HIGH); // PORTC digunakan untuk menghidupkan LED merah
      delay(1000);                 // Delay berbasis Timer0
      digitalWrite(RGB_RED, LOW);  // PORTC digunakan untuk mematikan LED merah
    }

    // Pastikan servo tertutup
    if (isOpen) {
      myServo.write(180); // Tutup servo, memengaruhi register OCR1A
      isOpen = false;
    }
  } else {
    // Cek deteksi tangan
    if (detectHand()) {
      if (!isOpen) { // Hanya akan terbuka jika servo belum terbuka
        myServo.write(120);        // Membuka servo, memengaruhi register OCR1A
        digitalWrite(RGB_GREEN, HIGH); // PORTC digunakan untuk menghidupkan LED hijau
        isOpen = true;             // Memperbarui status
        delay(1000);               // Delay berbasis Timer0
        digitalWrite(RGB_GREEN, LOW);  // PORTC digunakan untuk mematikan LED hijau
      }
    } else {
      if (isOpen) { // Hanya tutup jika sedang terbuka
        myServo.write(180);  // Menutup servo, memengaruhi register OCR1A
        isOpen = false;      // Memperbarui status
        delay(1000);         // Delay berbasis Timer0
      }
    }
  }
}
