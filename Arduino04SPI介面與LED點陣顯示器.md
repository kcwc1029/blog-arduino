## 1. LED 點陣（LED Matrix）？
是一種將多顆 LED 排列封裝成方形元件的顯示單元。
可用來顯示文字、符號、圖案等。
常見尺寸有：
- 5×7 點陣(LTP-1557AKR)
- 8×8 點陣(JLDM-1088DHG)
![upgit_20250403_1743692341.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250403_1743692341.png)



控制方式與結構：
- 分為 共陽極（共陽極是正接） 與 共陰極（共陰極是負接） 兩種。
- 每一個 LED 點是由行與列的交叉控制。
- 若點亮某一顆 LED，需同時將對應行接地、列接正電（或反之，依模組類型而定）。

---

接腳對應圖（以 8×8 為例）：
有 16 根接腳，對應 8 行 × 8 列。
控制方式：掃描行列訊號，快速切換點亮達到完整顯示效果。
![upgit_20250403_1743692448.png|434x323](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250403_1743692448.png)

接腳數太多怎麼辦：使用「串列傳輸」方式可減少 I/O 腳位消耗：

### 1.1. 專門驅動七段顯示器與LED點陣的IC：MAX7219


可同時驅動 8x8 LED 點陣模組或 8 位七段顯示器。
可多片串接，形成更大 LED 顯示牆。
使用 SPI 介面，只需佔用一組 SPI 接口( 3 條訊號線)即可驅動整組 LED：
- DIN（資料輸入）
- CLK（時脈）
- LOAD / CS（片選）

## 2. SPI序列介面

以下是你提供的「7-4 認識 SPI 序列介面與 MAX7219」章節內容的筆記整理：

---

## 3. 📘 7-4 認識 SPI 序列介面與 MAX7219

SPI（Serial Peripheral Interface）是一種高速、同步的四線式串列通訊介面
常用於：顯示器（如 LED、LCD）、感測器、記憶卡、控制晶片（如 MAX7219）

![upgit_20250403_1743695684.png|1142x998](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250403_1743695684.png)

![upgit_20250403_1743695697.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250403_1743695697.png)

### 3.1. Lab：使用 MAX7219 控制的 8x32 LED 點陣模組
接法

| LED 模組腳位 | 功能   | UNO 腳位 |
| -------- | ---- | ------ |
| VCC      | 電源   | 5V     |
| GND      | 地線   | GND    |
| DIN      | 資料輸入 | D11    |
| CS       | 晶片選擇 | D10    |
| CLK      | 時脈   | D13    |
|          |      |        |


## 4. MAX7219 暫存器對照表

| 暫存器名稱                   | 說明                                | 位址（Hex）       |
| ----------------------- | --------------------------------- | ------------- |
| 不運作 (No-Op)             | 不進行操作（常用於多個 MAX7219 串接時）          | `0x00`        |
| 資料 0 ~ 資料 7 (Digit 0~7) | 每個暫存器對應 LED 點陣的第 1 ~ 第 8 行        | `0x01 ~ 0x08` |
| 解碼模式 (Decode Mode)      | 是否開啟七段顯示器 BCD 解碼（點陣需設為 0x00）      | `0x09`        |
| 顯示強度 (Intensity)        | 控制整體亮度（0x00 最暗 ~ 0x0F 最亮）         | `0x0A`        |
| 掃描限制 (Scan Limit)       | 設定顯示幾行（0x00~0x07），點陣應設為 0x07      | `0x0B`        |
| 停機 (Shutdown)           | 0x00 = 關閉顯示，0x01 = 啟動顯示           | `0x0C`        |
| 顯示器檢測 (Display Test)    | 0x01 = 所有 LED 全亮（測試用），0x00 = 正常模式 | `0x0`         |

![upgit_20250404_1743777005.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250404_1743777005.png)


假設要給D0佔存器資料
- arduino傳送數據時，會先送高位元8bit，再送高位元8bit
- D11-D08傳送的是佔存器位置，D0佔存器位置為0x1
- D08-D01傳送的是資料

| MSB |     |     |     |     |     |     |     |     |     |     |     |     |     |     | LSB |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| D15 | D14 | D13 | D12 | D11 | D10 | D09 | D08 | D07 | D06 | D05 | D04 | D03 | D02 | D01 | D00 |
| X   | X   | X   | X   | 0   | 0   | 0   | 1   | 0   | 1   | 1   | 1   | 0   | 0   | 0   | 1   |

![upgit_20250404_1743777575.png|586x130](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250404_1743777575.png)

#### 4.1.1. 基礎定義
```cpp
#define DATA 11   // DIN pin
#define CLK  13   // CLK pin
#define CS   10   // LOAD/CS pin

// =============================
// MAX7219 Register Address Constants
// =============================

const byte NOOP        = 0x00;  // No Operation（不執行任何操作，常用於多模組串接）
const byte DIGIT0      = 0x01;  // 資料暫存器：第 1 行
const byte DIGIT1      = 0x02;  // 資料暫存器：第 2 行
const byte DIGIT2      = 0x03;  // 資料暫存器：第 3 行
const byte DIGIT3      = 0x04;  // 資料暫存器：第 4 行
const byte DIGIT4      = 0x05;  // 資料暫存器：第 5 行
const byte DIGIT5      = 0x06;  // 資料暫存器：第 6 行
const byte DIGIT6      = 0x07;  // 資料暫存器：第 7 行
const byte DIGIT7      = 0x08;  // 資料暫存器：第 8 行

const byte DECODEMODE  = 0x09;  // 解碼模式暫存器（用於七段顯示器，點陣顯示需關閉）
const byte INTENSITY   = 0x0A;  // 顯示強度暫存器（亮度控制，0x00 最暗 ~ 0x0F 最亮）
const byte SCANLIMIT   = 0x0B;  // 掃描限制暫存器（控制顯示的行數，點陣需設為 0x07）
const byte SHUTDOWN    = 0x0C;  // 停機暫存器（0x00 關閉顯示，0x01 啟用顯示）
const byte DISPLAYTEST = 0x0F;  // 顯示測試暫存器（0x01 所有 LED 亮起，0x00 正常模式）


// =============================
// Bit-banging SPI 傳送資料（使用 shiftOut）
// =============================
void sendCommand(byte address, byte data) {
  digitalWrite(CS, LOW);
  shiftOut(DATA, CLK, MSBFIRST, address);  // 傳送暫存器位址
  shiftOut(DATA, CLK, MSBFIRST, data);     // 傳送對應資料
  digitalWrite(CS, HIGH);
}

void SPIInitial(){
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);

  // 初始化 MAX7219
  sendCommand(SHUTDOWN, 0x01);      // 開啟顯示
  sendCommand(DECODEMODE, 0x00);    // 關閉解碼模式
  sendCommand(SCANLIMIT, 0x07);     // 顯示所有行
  sendCommand(INTENSITY, 0x08);     // 設定中等亮度
  sendCommand(DISPLAYTEST, 0x00);   // 關閉測試模式
}
```

#### 4.1.2. 亮兩個LED燈
基於上述程式碼，在第 1 行資料亮燈

![upgit_20250404_1743780004.png|468x348](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250404_1743780004.png)


```cpp
void setup() {
  SPIInitial();

  // 顯示第 1 行資料（例：點亮兩端）
  sendCommand(DIGIT0, 0b10000001);
  delay(1000);
}

void loop() {
}
```

### 4.2. Lab：畫出左上 → 右下 的對角線

![upgit_20250404_1743780155.png|734x565](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250404_1743780155.png)

基於【基礎定義】程式碼初始化，再加入下列程式碼
```cpp
// 陣列版本（一次寫入整條線）
void drawDiagonal() {
  for (byte i = 0; i < 8; i++) {
    sendCommand(DIGIT0 + i, 1 << i);  // 第 i 列，第 i 行亮
  }
}

// 清除畫面函式
void clearMatrix() {
  for (byte i = 0; i < 8; i++) {
    sendCommand(DIGIT0 + i, 0x00);  // 所有列清空
  }
}

void setup() {
  SPIInitial();
}

void loop() {
  drawDiagonal(); // 畫動畫
  delay(2000);
  clearMatrix();          // 清除畫面
  delay(500);
}
```

### 4.3. Lab：顯示字母A
![upgit_20250404_1743780897.png|477x363](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250404_1743780897.png)
基於【基礎定義】程式碼初始化，再加入下列程式碼
```cpp
// 以字母 A 為例（用二進位畫出形狀）
const byte font_A[8] = {
  0b00011000,
  0b00100100,
  0b01000010,
  0b01000010,
  0b01111110,
  0b01000010,
  0b01000010,
  0b00000000
};

void drawSymbol(const byte symbol[8]) {
  for (byte row = 0; row < 8; row++) {
    sendCommand(DIGIT0 + row, symbol[row]);
  }
}

void setup() {
  SPIInitial();
}

void loop() {
  drawSymbol(font_A);  // 顯示 A
}
```


更好用的模組：LedControl 函式庫