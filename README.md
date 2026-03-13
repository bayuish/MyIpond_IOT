# MyIpond_IOT

Project **ESP32** untuk monitoring kualitas air kolam ikan secara **real-time**.
Sistem membaca **pH, kekeruhan (NTU), dan suhu air**, lalu mengirim data ke server untuk diproses oleh **model machine learning**.

---

# 📋 Daftar Isi

* Fitur
* Struktur Folder
* Penjelasan File
* Cara Install
* Konfigurasi
* Penggunaan
* Format Data
* Dependensi
* Kustomisasi
* Catatan Penting
* Kontribusi
* Lisensi

---

# ✨ Fitur

* 📊 Monitoring **3 parameter air**

  * pH
  * Kekeruhan (NTU)
  * Suhu

* 🌐 **Koneksi WiFi otomatis**

  * Auto reconnect jika WiFi terputus

* ⏰ **Sinkronisasi waktu**

  * Mengambil waktu real-time dari **NTP Server Indonesia**

* 📤 **Kirim data ke server**

  * Menggunakan **HTTP POST**
  * Format **JSON**

* 🧩 **Kode modular**

  * Setiap sensor memiliki file `.h` dan `.cpp` sendiri

* 🔧 **Mudah dikustomisasi**

  * Semua konfigurasi ada di `config.h`

---

# 📁 Struktur Folder

```text
MyIpond_IOT/
│
├── include/                         # Header dan modul utama
│   ├── config.h                      # Konfigurasi WiFi, server, pin sensor
│   ├── data_structures.h             # Struct untuk data sensor
│   ├── network_utils.h               # Header fungsi jaringan
│   ├── network_utils.cpp             # Implementasi WiFi, HTTP, NTP
│   │
│   ├── sensors/                      # Modul masing-masing sensor
│   │   ├── ph_sensor.h
│   │   ├── ph_sensor.cpp             # Sensor pH
│   │   ├── turbidity_sensor.h
│   │   ├── turbidity_sensor.cpp      # Sensor kekeruhan
│   │   ├── temperature_sensor.h
│   │   └── temperature_sensor.cpp    # Sensor suhu DS18B20
│   │
│   └── README.md                     # Dokumentasi project
│
├── src/
│   ├── main.cpp                      # Program utama
│   └── main.cpp.backup               # Backup kode lama
│
├── lib/                              # Library tambahan
├── test/                             # Unit testing
└── platformio.ini                    # Konfigurasi PlatformIO
```

---

# 📄 Penjelasan File

## 🔧 Konfigurasi (`config.h`)

File ini berisi semua **pengaturan utama sistem**.

Yang bisa diubah di sini:

* SSID dan password WiFi
* URL server tujuan
* Pin sensor
* Konstanta kalibrasi sensor
* Konfigurasi waktu NTP

Jika ingin mengganti konfigurasi sistem, cukup ubah file ini.

---

## 📊 Data Structure (`data_structures.h`)

File ini berisi **struct untuk menyimpan data sensor**.

```cpp
struct PHData {
    float value;
    int adc;
    float voltage;
    String status;
};

struct TurbidityData {
    float ntu;
    int adc;
    float voltage;
    String status;
};

struct TemperatureData {
    float celsius;
    float fahrenheit;
    String status;
};

struct AllSensorData {
    PHData ph;
    TurbidityData turbidity;
    TemperatureData temperature;
    String timestamp;
    String esp_id;
};
```

Struct ini digunakan untuk mengelompokkan data sebelum dikirim ke server.

---

# 🌐 Network Manager (`network_utils.h / network_utils.cpp`)

Class **NetworkManager** bertanggung jawab untuk:

* Menghubungkan ESP32 ke **WiFi**
* **Auto reconnect** jika WiFi terputus
* Mengambil **waktu dari NTP server**
* Mengirim data ke server menggunakan **HTTP POST**
* Membuat **JSON payload**

Semua fungsi jaringan dipisahkan di modul ini agar kode utama tetap bersih.

---

# 🧪 Sensor Modules

Setiap sensor dibuat sebagai modul terpisah agar mudah dikembangkan.

| File                                          | Fungsi                                       |
| --------------------------------------------- | -------------------------------------------- |
| ph_sensor.h / ph_sensor.cpp                   | Membaca sensor pH, konversi ADC ke nilai pH  |
| turbidity_sensor.h / turbidity_sensor.cpp     | Membaca sensor kekeruhan dan konversi ke NTU |
| temperature_sensor.h / temperature_sensor.cpp | Membaca suhu dari sensor **DS18B20**         |

Setiap modul juga menentukan **status kualitas air** berdasarkan nilai sensor.

Contoh status:

* pH → Asam / Netral / Basa
* Turbidity → Jernih / Keruh / Sangat Keruh
* Temperature → Dingin / Normal / Panas

---

# 🎯 Main Program (`src/main.cpp`)

File ini adalah **entry point firmware**.

Tugas utama program:

1. Inisialisasi sensor
2. Menghubungkan ESP32 ke WiFi
3. Mengambil waktu dari NTP
4. Loop utama setiap **5 detik**:

* Membaca semua sensor
* Menampilkan data ke **Serial Monitor**
* Mengirim data ke server

Jika WiFi terputus, sistem akan mencoba **reconnect otomatis**.

---

# 🚀 Cara Install

## Prasyarat

Perangkat yang dibutuhkan:

* **VS Code**
* **PlatformIO Extension**
* **ESP32 Development Board**

Sensor yang digunakan:

* Sensor **pH (analog)**
* Sensor **Turbidity (analog)**
* Sensor suhu **DS18B20**

---

## Langkah Instalasi

### 1. Clone Repository

```bash
git clone https://github.com/username/MyIpond_IOT.git
cd MyIpond_IOT
```

---

### 2. Buka Project di VS Code

```bash
code .
```

---

### 3. Ubah Konfigurasi

Edit file:

```
include/config.h
```

Contoh konfigurasi:

```cpp
#define WIFI_SSID "MBC 2.4GHz"
#define WIFI_PASSWORD "gogombc123"
#define SERVER_URL "http://server-anda.com/predict"
```

---

### 4. Build Project

```bash
pio run
```

---

### 5. Upload Firmware ke ESP32

```bash
pio run --target upload
```

---

### 6. Monitor Serial

```bash
pio device monitor
```

---

# ⚙️ Konfigurasi

## Pin Sensor (Default)

| Sensor           | Pin     |
| ---------------- | ------- |
| pH Sensor        | GPIO 32 |
| Turbidity Sensor | GPIO 33 |
| DS18B20          | GPIO 27 |

---

## WiFi & Server

```cpp
#define WIFI_SSID "MBC 2.4GHz"
#define WIFI_PASSWORD "gogombc123"
#define SERVER_URL "http://mechine-learning-ecis.mbclaboratory.com/predict"
```

---

## Kalibrasi Sensor

```cpp
// pH: (adc - PH_OFFSET) / PH_SCALE
#define PH_OFFSET 123.33
#define PH_SCALE 175.06

// Turbidity: (TURBIDITY_OFFSET - adc) / TURBIDITY_SCALE
#define TURBIDITY_OFFSET 2464.3571
#define TURBIDITY_SCALE 0.8452
```

---

# 📤 Format Data ke Server

Data dikirim setiap **5 detik** dalam format JSON:

```json
{
  "ESP_ID": "ESP_Kolam1",
  "pH": 7.2,
  "pH_Status": "Netral",
  "NTU": 15.5,
  "Turbidity_Status": "Sedikit Keruh",
  "Temperature": 28.3,
  "Temperature_Status": "Normal",
  "ADC_PH": 2048,
  "Voltage_PH": 1.65,
  "ADC_Turbidity": 1800,
  "Voltage_Turbidity": 1.45,
  "Timestamp": "14:30:25"
}
```

Data ini kemudian diproses oleh **server machine learning** untuk prediksi kondisi kolam.

---

# 📦 Dependensi

Library yang digunakan:

```ini
lib_deps = 
    milesburton/DallasTemperature@^3.9.0
    paulstoffregen/OneWire@^2.3.7
    arduino-libraries/NTPClient@^3.2.1
```

PlatformIO akan otomatis menginstall library tersebut saat build.

---

# 🔧 Kustomisasi

### Menambahkan Sensor Baru

1. Buat file baru di:

```
include/sensors/
```

contoh:

```
oxygen_sensor.h
oxygen_sensor.cpp
```

2. Tambahkan struct baru di:

```
data_structures.h
```

3. Gunakan modul tersebut di:

```
main.cpp
```

---

### Mengganti Server

Edit di `config.h`:

```cpp
#define SERVER_URL "http://server-baru.com/api"
```

---

### Mengubah Interval Pengiriman Data

Ubah nilai delay pada `loop()`:

```cpp
delay(5000); // 5 detik
```

---

# 📝 Catatan Penting

* `main.cpp.backup` adalah **backup kode lama** jika ingin kembali ke versi sebelumnya.

* Port upload dapat diatur di `platformio.ini`.

Contoh:

Windows

```
upload_port = COM3
```

Linux

```
upload_port = /dev/ttyUSB0
```

Mac

```
upload_port = /dev/cu.usbserial-xxxx
```

---

# 🤝 Kontribusi

Kontribusi sangat terbuka.

Silakan:

* membuat **Pull Request**
* membuka **Issue**
* memberi saran perbaikan

---

# 📄 Lisensi

MIT License

Bebas digunakan, dimodifikasi, dan dikembangkan sesuai kebutuhan.

---

Dibuat dengan ❤️ untuk monitoring kolam ikan
Data dikirim ke server untuk analisis dan prediksi machine learning.
