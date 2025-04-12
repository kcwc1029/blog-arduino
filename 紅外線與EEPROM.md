## 1. 紅外線
近紅外線（Near IR）：接近可見光，可穿透霧、煙，常用於 短距離測距、紅外線遙控。
遠紅外線（Far IR）：來自熱能輻射，人類體溫下約釋放波長為 10μm（微米）的遠紅外線。

![upgit_20250412_1744460818.png|699x320](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250412_1744460818.png)

## 2. 紅外線遙控與編碼原理
生活周遭物品都會散發不一程度的紅外線，為了避免受到其他紅外線的干擾，內建在電視，音響等家電的紅外線遙控器。都只會對特定的【頻率信號(正確名稱為載波，通常為36KHz或38KHz)】與【通關密語】有反應。

載波相當於訊號的載體，使用載波傳送訊號的原因：
- 增加傳輸距離：高頻率載波更容易在空間中傳播
- 提高傳輸效率：多個訊息可以加在一個載波上，有效利用頻譜資源
- 提升抗訊干擾能力



### 2.1. 二、遙控器紅外線資料傳送機制（以 RC-5 為例）

每按一次鍵，遙控器會每 0.1 秒傳送一段紅外線訊號。
一段訊號長度固定（如：114ms），由多個「0」「1」脈波組成。
每段資料含：
- 起始位元 + 控制位元（如「按下」或「持續按住」）
- 裝置位址（5位元）：代表受控設備，如 1 = 電視機、20 = CD 機。
- 命令資料（6位元）：代表按鍵編碼，如 16 = 音量加、17 = 音量減。
        
![upgit_20250412_1744461194.png|643x313](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250412_1744461194.png)


## 3. 紅外線遙控接收元件
紅外線遙控接收元件，內部包含紅外線接收元件與訊號處裡IC，常見型號為TSOP4836與TSOP4838(最後兩位數字代表仔波頻率)。紅外線接收元件負責感應紅外線波長範圍
![upgit_20250412_1744461429.png|687x142](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250412_1744461429.png)


### 3.1. Lab：使用arduino操作紅外線感測器

![upgit_20250412_1744464444.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250412_1744464444.png)

```c
#include <IRremote.h>

const int RECV_PIN = 9;  // 接到 OUT 腳的腳位
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);    // 開啟序列監視器
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); // 啟用接收器
  Serial.println("準備接收紅外線信號...");
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("接收到紅外線碼：");
    // 完整 Raw Data(16進未表示)
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); 

    // 解析後的資訊
    // Protocol
    // Address=0x0 => 控制的裝置地址（幾乎都會是 0x0，表示「廣播」或單一裝置）
    // Command=0x30	你按下的按鍵對應的指令碼（這是你要拿來判斷用的）
    // Raw-Data=0xCF30FF00 => row data
    IrReceiver.printIRResultShort(&Serial);
    
    // 如何發送指令
    IrReceiver.printIRSendUsage(&Serial);   
    irrecv.resume();  // 接收下一筆訊號
  }
}
```


## EEPROM
EEPROM（Electrically Erasable Programmable Read-Only Memory） 是一種非揮發性記憶體，斷電後資料仍會保存。
Arduino UNO 使用的 ATmega328 微控制器內含 EEPROM：
- **UNO R3**：1KB（共 1024 位元組） => 可以寫的地址由0-1023
- **UNO R4（RA4M1）**：8KB（8192 位元組）  
- 可使用 EEPROM 儲存設定、使用者資料、記錄值等。
    

### 1. 基本操作
需引入 `<EEPROM.h>`

|函式|功能|
|---|---|
|`EEPROM.read(地址)`|從指定位址讀取**1 位元組**|
|`EEPROM.write(地址, 值)`|將**1 位元組**寫入指定位址（會強制覆蓋）|
|`EEPROM.update(地址, 值)`|只有當寫入值與目前不同時才會寫入，**延長 EEPROM 壽命**|
|`EEPROM.put(地址, 資料)`|儲存任意資料型態（如 `int`, `float`, `struct`）|
|`EEPROM.get(地址, 變數)`|讀出任意型別資料，並存入變數|
|`EEPROM.length()`|回傳 EEPROM 的大小（UNO R3 是 1024）|




## 2. EEPROM 的進階觀念
位址與位元組
- EEPROM 是以「**位元組（Byte）為單位**」儲存資料
- 像 `int`（2 Bytes）、`float`（4 Bytes）需佔用多個位址

```c
多位元資料儲存邏輯（以 int 為例）
舉例：儲存 int num = 1234

// Step01：將數字轉成二進位
1234（十進位） = 00000100 11010010（二進位）
              ↑ 高位元組(4) ↑ 低位元組(210)


// Step02：位元運算
int num = 1234;
byte high = num >> 8;      // 把右邊 8 bits 去掉 ⇒ 00000100 ⇒ 4
byte low  = num & 0xFF;    // 把左邊的 bits 遮掉 ⇒ 11010010 ⇒ 210

// Step03：分別寫入 EEPROM 兩個連續位址
EEPROM.write(0, high);   // 儲存高位元組到位址 0
EEPROM.write(1, low);    // 儲存低位元組到位址 1

// Step04：讀出後重組
byte high = EEPROM.read(0);
byte low  = EEPROM.read(1);
int num = (high << 8) | low;  // 左移 8 位元 + 合併 = 原始值
```


注意事項
- EEPROM **有寫入壽命限制（約 10 萬次）**，盡量使用 `update()` 來延長壽命。
- 多位元資料必須寫入連續位址，讀取時也要注意對齊。
- 可搭配 `EEPROM.length()` 檢查記憶體容量，避免超出範圍。


## 3. 使用範例


### 3.1. Lab：取得 EEPROM 大小
![upgit_20250411_1744382935.png|380x246](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744382935.png)

```c
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);  // 開啟序列通訊（必要）
  int  value = EEPROM.length();
  Serial.println(value);            // 顯示：5
}

void loop(){}
```
### 3.2. Lab：儲存單一位元組

```cpp
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);  // 開啟序列通訊

  byte data = 5;
  EEPROM.write(27, data);         // 將資料寫入位址 27
  int value = EEPROM.read(27);   // 從位址 27 讀出資料
  Serial.println(value);          // 顯示：5
}

void loop() {}
```

### 3.3. Lab：儲存與讀取整數型態（int）
![upgit_20250411_1744383190.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744383190.png)

```cpp
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);  // 開啟序列通訊
  
  int num = 1234;
  EEPROM.put(0, num);     // 儲存 int，從位址 0 開始（佔用 2 個位元組）
  int getNum = 0;
  EEPROM.get(0, getNum);  // 讀回 int
  Serial.println(getNum); // 顯示：1234
}

void loop() {}
```

### 3.4. Lab：儲存與讀取浮點數（float）


### 3.5. Lab：儲存複合型態（如 int、float）

### 3.6. Lab： 儲存與讀取字串（字元陣列）

### 3.7. Lab： 儲存與讀取結構體（struct）








