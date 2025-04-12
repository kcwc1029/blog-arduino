## 1. LCD 模組

LCD 顯示模組種類：分為 文字模式 與 圖形模式

-   **文字型**：顯示固定大小的文字、符號、數字（如 1602 LCD）
-   **圖形型**：可顯示圖片或自訂圖樣（如 OLED、128x64）
    常見顯示格式：**16x2**（共兩行、每行 16 字）
    常用控制晶片：**HD44780**
    無背光時會看不清楚 → 需加背光 LED 模組
    常搭配 Arduino 使用，內建字元資料表（支援英文字、符號等）

### 1.1. HD44780 相容的 LCD 控制原理

控制架構：LCD 內部包含

-   LCD 控制器
-   LCD 顯示記憶體（字元對應位址）
-   LCD 驅動器
-   LCD 面板（實際顯示畫面）

腳位功能（14 隻腳）

| 腳位  | 說明                            |
| ----- | ------------------------------- |
| VSS   | GND 接地                        |
| VDD   | +5V                             |
| V0    | 對比度調整（加電位器）          |
| RS    | 資料/指令切換（0=指令、1=資料） |
| RW    | 讀寫切換（多用寫）              |
| E     | Enable 啟用訊號                 |
| D0~D7 | 資料線（8 位元傳輸）            |

資料傳輸模式（顯示資料的方式）：支援兩種資料傳輸模式：

-   8 位元模式（需接 D0~D7）
-   4 位元模式（常用，只接 D4~D7）
    (為了省 IO 腳位，我們通常使用 **4-bit 模式**) =>需接線：D4~D7 + RS（暫存器選擇腳）+ E（致能腳）

## 2. I2C

![upgit_20250405_1743867269.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250405_1743867269.png)

### 2.1. Lab：使用`LiquidCrystal_I2C`驅動 LCD

Wire.h：Arduino 官方內建的函式庫，用來處理 I2C（雙線式）通訊協定。
I2C（Inter-Integrated Circuit）是一種常用的通訊方式，只用 2 條線：

-   SDA：資料線（Data）
-   SCL：時脈線（Clock）
    Wire.h 負責幫你：
-   發送資料給 I2C 裝置（如 LCD、溫濕度感測器、EEPROM）
-   接收來自 I2C 裝置的資料
-   設定 I2C 主機（Master）或從機（Slave）

![upgit_20250405_1743862520.png|593x410](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250405_1743862520.png)

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 初始化 LCD（預設位址通常是 0x27 或 0x3F）
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2 的 I2C LCD
const int potPin = A0;  // 可變電阻接 A0

void initialLCD(){
  lcd.init();               // 初始化 LCD
  lcd.backlight();          // 開啟背光
}

void setup() {
  initialLCD();
}

void loop() {
  int potValue = analogRead(potPin);  // 讀取旋鈕數值

  lcd.clear();  // 清除整個 LCD 畫面

  lcd.clear();                // 清除整個 LCD 畫面
  lcd.setCursor(0, 0);
  lcd.print("Hello I2C LCD!");
  lcd.setCursor(0, 1);
  lcd.print("Value: ");
  lcd.setCursor(7, 1);    // 光標移到數字位置
  lcd.print(potValue);    // 顯示新的值

  delay(1000);
}
```

### 2.2. Lab：掃描周邊裝置

根據 I2C，裝置給的地址編碼為 7bit=>共 128 個位址，因此要掃描裝置地址的話，就是跑 for(1-127)

![upgit_20250405_1743862520.png|593x410](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250405_1743862520.png)`

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("I2C Scanner"); // 顯示啟動訊息
}

void loop() {
  byte error, address; // 宣告變數 error 用來儲存傳輸錯誤碼，address 是目前要掃描的 I2C 位址
  Serial.println("Scanning...");

  // 掃描所有可能的 I2C 位址（從 1 到 126）
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);   // 啟動與該位址的 I2C 傳輸
    error = Wire.endTransmission();    // 結束傳輸，取得是否有錯誤發生

    if (error == 0) { // 如果沒有錯誤，表示該位址有裝置回應
      Serial.print("Found I2C device at 0x");
      Serial.println(address, HEX);          // 以十六進位格式顯示位址
    }
  }
  delay(3000);
}
```

### 2.3. Lab：掃描周邊裝置

利用 I1C 的特行，嘗試連接兩塊 LCD
![upgit_20250405_1743868096.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250405_1743868096.png)

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 宣告兩個 LCD（位址不同）
LiquidCrystal_I2C lcd1(0x27, 16, 2);  // 第一個 LCD
LiquidCrystal_I2C lcd2(0x3F, 16, 2);  // 第二個 LCD

void setup() {
  // 初始化 LCD1
  lcd1.init();
  lcd1.backlight();
  lcd1.setCursor(0, 0);
  lcd1.print("Hello from LCD1!");
  lcd1.setCursor(0, 1);
  lcd1.print("I'm on 0x27!");

  // 初始化 LCD2
  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(0, 0);
  lcd2.print("Hello from LCD2!");
  lcd2.setCursor(0, 1);
  lcd2.print("I'm on 0x3F!");
}

void loop() {
  // 空迴圈，不需重複更新內容
}
```
