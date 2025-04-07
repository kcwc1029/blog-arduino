## 1. Arduino接腳

![upgit_20250401_1743500023.png|523x413](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743500023.png)

![upgit_20250404_1743696917.png|1113x788](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250404_1743696917.png)

 數位腳位（Digital I/O）：
 - D0~D13：數位輸入/輸出
- D3、5、6、9、10、11：支援 PWM 輸出
類比輸入腳位（Analog Input）
-  A0~A5：支援讀取類比訊號
- A4 = SDA、A5 = SCL（支援 I2C 通訊）
SPI 腳位（靠近 ICSP 區塊）
- MOSI：主機資料輸出
- MISO：主機資料輸入
- SCK：時鐘訊號
電源區域
- 3.3V / 5V：供電輸出
- GND：接地
- VIN：可接電池或變壓器輸入（供板子用電）
其他
- RESET：重置按鈕，讓板子重新執行程式
- ICSP：可用來燒錄 bootloader
- IOREF：給擴充板（Shield）使用，供應電壓參考
- USB 插座：可供電與上傳程式

## 2. CPU、MPU、MCU、SoC

![upgit_20250223_1740298054.png|716x250](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/02/upgit_20250223_1740298054.png)

### 2.1. CPU（中央處理器，Central Processing Unit）
- 是負責指令執行與邏輯運算的核心單元。
- 在嵌入式領域中，有時稱為 MPU（Microprocessor Unit，微處理器單元）
### 2.2. MCU（Microcontroller Unit，微控制器）
MCU 是嵌入式系統中的主角，它是一種可以獨立運作的單晶片系統，內部整合了：
特點：  
- 小型、低功耗、成本低  
- 例：Arduino UNO、ESP32、STM32 等開發板皆採用 MCU  

| 元件 | 功能說明 |
|------|----------|
| CPU | 控制與運算核心 |
| Flash | 儲存程式與常駐資料（例如韌體） |
| SRAM | 程式執行時的變數與資料 |
| I/O 介面 | 與外部設備連接，如 GPIO、ADC、UART 等 |

### 2.3. SoC（System on a Chip，系統單晶片）
在手機或電腦等複雜系統中，會將：
- CPU
- GPU
- RAM 控制器
- I/O 控制器  
…等整合在一顆晶片內，稱為 **SoC**。  

是功能更強大、整合度更高的「超進化版 MCU」。

### 2.4. MCU 記憶體簡介（以 Arduino & ESP32 為例）
![upgit_20250223_1740298582.png|718x389](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/02/upgit_20250223_1740298582.png)

| 記憶體類型 | 功能 | 斷電是否保留？ | 說明 |
|------------|------|----------------|------|
| **SRAM** | 暫存變數與資料 | ❌ 否 | 速度最快，容量小（Arduino UNO：2KB / ESP32：520KB） |
| **Flash** | 儲存程式與重要資料 | ✅ 是 | 存放韌體或設定檔（UNO：32KB / ESP32：4MB ~ 16MB） |
| **EEPROM** | 儲存少量永久設定資料 | ✅ 是 | 可逐字節操作（UNO：1KB / ESP32 可用 Flash 模擬） |
### 2.5. Flash 中的程式就是「韌體（Firmware）」
- MCU 通電後，會先執行內部的 Bootloader（開機程式）
- 然後載入並執行儲存在 Flash 中的程式
- 這個儲存在 MCU 內、每次上電就會執行的程式，就叫做 **Firmware（韌體）**
## 3. Arduino 資料型態與記憶體使用

### 3.1. 基本資料型態與範圍（以 UNO 為例）
UNO R3 是 8 位元處理器，所以 `double` 實際上和 `float` 一樣，都是 4 Bytes！

| 類型       | 中文名稱   | 占用記憶體                         | 數值範圍（約略）          |
| -------- | ------ | ----------------------------- | ----------------- |
| `bool`   | 布林     | 1 Byte                        | true / false      |
| `byte`   | 位元組    | 1 Byte                        | 0 ~ 255           |
| `char`   | 字元     | 1 Byte                        | -128 ~ 127        |
| `int`    | 整數     | 2 Bytes                       | -32768 ~ 32767    |
| `long`   | 長整數    | 4 Bytes                       | 約 ±21 億（32 位元）    |
| `float`  | 浮點數    | 4 Bytes                       | 約 ±3.4E±38（32 位元） |
| `double` | 高精度浮點數 | 4 Bytes（UNO）或 8 Bytes（UNO R4） | ±1.7E±308（64 位元）  |
|          |        |                               |                   |

### 3.2. signed / unsigned 差異

| 類型           | 中文名稱     | 數值範圍              |
|----------------|--------------|------------------------|
| `unsigned char`| 正字元       | 0 ~ 255                |
| `unsigned int` | 正整數       | 0 ~ 65535（16位元）或 0~4294967295（32位元） |
| `unsigned long`| 正長整數     | 0 ~ 4294967295         |

```c
// 超過範圍的數值會被**轉為補數方式儲存**
byte x = -1;  // x 值變成 255
byte y = -2;  // y 值變成 254
```

### 3.3. 🧬 Arduino 使用標準整數名稱（C99 標準）

| 類型       | 對應舊型別       |
|------------|------------------|
| `int8_t`   | `char`           |
| `uint8_t`  | `byte`           |
| `int16_t`  | `int`            |
| `uint16_t` | `unsigned int`   |
| `int32_t`  | `long`           |
| `uint32_t` | `unsigned long`  |

範例：
```c
byte pin = 13;         // 傳統寫法
uint8_t pin = 13;      // 等效用法（更推薦）
```

## 4. 開關
開關依照尺寸與外型，可以分為：
![upgit_20250401_1743501453.png|733x390](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743501453.png)

另外可以分為：
* 常開(normal open, NO)：接點平常是不相連的，按下相連
* 常關(normal close, NC)：接點平常是相連的，按下不連

![upgit_20250401_1743501593.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743501593.png)

![upgit_20250401_1743501601.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743501601.png)

## 5. 開關電路
若開關沒有接地，也沒有接高電位，就會形成浮動訊號。
![upgit_20250401_1743501675.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743501675.png)

### 5.1. 上拉電阻
![upgit_20250401_1743501811.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743501811.png)

### 5.2. 下拉電阻

![upgit_20250401_1743501818.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743501818.png)

### 5.3. Lab：LED與開關，按著才亮
![upgit_20250401_1743502434.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743502434.png)

```cpp
void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, HIGH);
}
```

### 5.4. Lab：LED與開關toggle
![upgit_20250401_1743502885.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743502885.png)


```cpp
void setup() {
  pinMode(13, OUTPUT);  // LED
  pinMode(2, INPUT);   // 按鈕接在 D3
}

void loop() {
  int btn = digitalRead(2);  // 讀取按鈕狀態
  if (btn == HIGH) {
    digitalWrite(13, HIGH);   // 按下按鈕 → LED亮
  } else {
    digitalWrite(13, LOW);    // 沒按按鈕 → LED滅
  }
}
```

### 5.5. 開關彈跳問題
開關在從高信號到低信號時，會有短暫的彈跳現象。

![upgit_20250401_1743503115.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250401_1743503115.png)

- 解法01：使用delay()
```cpp
void setup() {
  pinMode(13, OUTPUT);  // LED
  pinMode(2, INPUT);   // 按鈕接在 D3
}

void loop() {
  int btn = digitalRead(2);  // 讀取按鈕狀態
  if (btn == HIGH) {
    delay(50); // 加一點時間過濾彈跳（通常 20~50ms 就夠）
    digitalWrite(13, HIGH);   // 按下按鈕 → LED亮
  } else {
    digitalWrite(13, LOW);    // 沒按按鈕 → LED滅
  }
}
```

### 5.6. Lab：LED跑馬燈

![[1743504021061.gif]]

```cpp
const byte led01 = 8;
const byte led02 = 9;
const byte led03 = 10;
const byte led04 = 11;
const byte led05 = 12;

void setup() {
  pinMode(led01, OUTPUT);  // LED
  pinMode(led02, OUTPUT);  // LED
  pinMode(led03, OUTPUT);  // LED
  pinMode(led04, OUTPUT);  // LED
  pinMode(led05, OUTPUT);  // LED
}

void loop() {
  digitalWrite(led01, HIGH);   // 按下按鈕 → LED亮
  digitalWrite(led02, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led03, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led04, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led05, LOW);   // 按下按鈕 → LED亮
  delay(1000);
  digitalWrite(led01, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led02, HIGH);   // 按下按鈕 → LED亮
  digitalWrite(led03, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led04, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led05, LOW);   // 按下按鈕 → LED亮
  delay(1000);
  digitalWrite(led01, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led02, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led03, HIGH);   // 按下按鈕 → LED亮
  digitalWrite(led04, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led05, LOW);   // 按下按鈕 → LED亮
  delay(1000);
  digitalWrite(led01, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led02, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led03, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led04, HIGH);   // 按下按鈕 → LED亮
  digitalWrite(led05, LOW);   // 按下按鈕 → LED亮
  delay(1000);
  digitalWrite(led01, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led02, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led03, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led04, LOW);   // 按下按鈕 → LED亮
  digitalWrite(led05, HIGH);   // 按下按鈕 → LED亮
  delay(1000);
}
```


### 5.7. Lab：LED跑馬燈 使用迴圈優化

```cpp
const byte leds[] = {8, 9, 10, 11, 12};  // 正確陣列寫法
byte len = sizeof(leds) / sizeof(leds[0]);  // 正確計算長度

void setup() {
  // 初始化
  for (int i = 0; i < len; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  // 往右跑馬燈 (0 到 4)
  for (int i = 0; i < len; i++) {
    digitalWrite(leds[i], HIGH);
    delay(1000);
    digitalWrite(leds[i], LOW);
  }

  // 往左跑馬燈 (4 到 0)
  for (int i = len - 2; i >= 0; i--) {
    digitalWrite(leds[i], HIGH);
    delay(1000);
    digitalWrite(leds[i], LOW);
  }
}
```

