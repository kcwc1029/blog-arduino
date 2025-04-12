## 1. LED七段顯示器

![upgit_20250402_1743571094.png|591x267](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743571094.png)


七段顯示器依照電源連接方式可分為兩種：
- 共陰極（Common Cathode）（常見）  
	- 所有 LED 的接地端共用（接 GND）  
	- 要讓某一段 LED 發光，需對另一端 輸入高電位（HIGH）  
	- 輸入高電位 → 發光
- 共陽極（Common Anode）
	- 所有 LED 的正極接一起，連接電源（VCC）  
	- 要讓某段發光，需對另一端 輸入低電位（LOW）  
	- 輸入低電位 → 發光
![upgit_20250402_1743571268.png|737x216](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743571268.png)


為了容易操弄，我們會將每組數字代號，用陣列的方式儲存

![upgit_20250402_1743571412.png|551x361](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743571412.png)
![upgit_20250402_1743571492.png|551x346](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743571492.png)

假設要點亮【1】，我們需要點亮b、c的LED，編碼數字又會因為共陰極/共陽極而有所不同。

### 1.1. 使用 PORT 寄存器控制腳位輸出（比 `digitalWrite()` 更快）
為了簡化同時對數個接腳的操作，可以設計成，把某一群接腳的模式都設為【輸出】，或者同時某一群接腳的模式都設為【高/低店為】
URO R3的微控器ATmaga328來說，把街角分為3個IO port

|PORT|控制腳位範圍|
|---|---|
|B|D8 ~ D13|
|C|A0 ~ A5（模擬腳）|
|D|D0 ~ D7|

而這3組port也有他對應的register來設定他們的模式與輸入/輸出值

- DDR（Data Direction Register）資料方向暫存器
	- 用來設定腳位是 輸入 還是 輸出
	- 就像你平常用 pinMode(10, OUTPUT); 一樣，但這裡是直接寫入二進位控制。
	- 有三種：DDRB、DDRC、DDRD，對應「哪一組腳位」
		- DDRB：第 8~13 腳（PORTB）
		- DDRC：A0~A5 腳（PORTC）
		- DDRD：第 0~7 腳（PORTD）
-  PORT 暫存器：
	- 用來設定腳位輸出的電壓是 HIGH（高電位） 還是 LOW（低電位）
	- 與 DDR 配合使用 → 腳位必須先設為輸出，PORT 才能控制輸出電壓
```cpp
DDRB = B00000100;   // 將 D10 設為輸出
PORTB = B00000100;  // 將 D10 輸出 HIGH（點亮 LED）
```
- PIN 暫存器：
	- 用來「讀取」腳位的輸入狀態（即外面是否有訊號進來）
	- 常用於按鈕輸入、感測器讀取等
```cpp
if (PINB & B00000100) {
  // D10 腳位為 HIGH，就執行這段
}
```
### 1.2. Lab：連接七段顯示器與開發板
(一開始是使用範例程式碼，但有錯，因此上網找)
(所以不會使用佔存器的方式去操作)

| Arduino的腳位 | 七段顯示器的腳位 |
| ---------- | -------- |
| 2          | 7(A)     |
| 3          | 6(B)     |
| 4          | 4(C)     |
| 5          | 2(D)     |
| 6          | 1(E)     |
| 7          | 9(F)     |
| 8          | 10(G)    |
| 9          | 5(DP)    |

![upgit_20250402_1743602436.png|151x261](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743602436.png)

![upgit_20250402_1743602612.png|914x472](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743602612.png)
```cpp
#define NUM 8
int pins[NUM] = {2, 3, 4, 5, 6, 7, 8, 9};

int data[10][NUM] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1, 0}, // 2
  {1, 1, 1, 1, 0, 0, 1, 0}, // 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // 4
  {1, 0, 1, 1, 0, 1, 1, 0}, // 5
  {1, 0, 1, 1, 1, 1, 1, 0}, // 6
  {1, 1, 1, 0, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1, 0}, // 8
  {1, 1, 1, 1, 0, 1, 1, 0}  // 9
};

void setup() {
  for (int i = 0; i < NUM; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void writeNumber(int n) {
  for (int i = 0; i < NUM; i++) {
    digitalWrite(pins[i], data[n][i] ? HIGH : LOW);
  }
}

void loop() {
  for (int n = 0; n <= 9; n++) {
    writeNumber(n);
    delay(1000);
  }
}
```

## 2. 積體電路（IC）：
- 將多個電子元件整合於一個小晶片上，完成特定電路功能。
- 優點：體積小、可靠性高、價格低廉，廣泛用於各種電子產品。

分類：
- **數位IC**：處理邏輯運算、計數、解碼、編碼等（二進位0與1）。
- **類比IC**：處理放大、調節電壓等連續訊號。

命名方式（例：HD74HC08P）：
- **廠牌代碼**：HD（Hitachi）
- **系列/功能型號**：74HC08
- **封裝型式**：P 表示 DIP（雙列直插封裝）

DIP封裝特徵：
- 有 **半月形缺口** 與 **圓點** 用來標示第1腳位。
- 腳距 2.54mm（0.1吋）。
- 並排型封裝（Dual In-line Package）。

![upgit_20250402_1743576444.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743576444.png)

![upgit_20250402_1743576556.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743576556.png)


### 2.1. 使用IC擴充arduino的輸出腳位
原生使用七段顯示器會有缺點：
- 使用到pin0與pin1
- 佔用太多腳位
因次，我們可以透過積體電路元件來擴充arduino的輸出腳位

減少auduino腳位的普遍解決方式，就是將原本「並聯」元件的接法，改為「串連」
常見的手法是採用74HC595的串入並出IC，充當arduino與七段顯示器之間的媒介

![upgit_20250402_1743576829.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743576829.png)



## 3. 74HC595 簡介
**74HC595** 是一個 **8位元移位暫存器（shift register）**。
可將 74HC595 想像成一條生產線：
- 資料從一個稱為 **SER**（serial）輸入端進入，類似將物品依序送入輸送帶。
- 每次時脈（clock）觸發，就像齒輪轉動一次，整條生產線上的物品就會**向左移動一格**。
- 這樣資料就可以從**串列輸入**轉為**並列輸出**。


![upgit_20250402_1743577972.png|491x781](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743577972.png)

![upgit_20250402_1743578037.png|486x332](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743578037.png)

![upgit_20250402_1743578055.png|682x345](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743578055.png)


### 3.1. 使用 `shiftOut()` 函數 傳輸序列資料給 74HC595
`shiftOut()` 是 Arduino 提供的函數，用來**序列資料位移輸出**。
可一次傳送 **8 位元（1 byte）** 資料給 **74HC595**。
只需設定資料與時脈腳位，不需處理移位細節。

```cpp
// 函數語法
shiftOut(資料輸出腳位, 時脈腳位, 位元順序, 資料值);

資料輸出腳位：資料輸入端 SER。
時脈腳位：控制移動節奏的 SHCP。
位元順序：
  - `MSBFIRST`：最左邊（高位元）先送
  - `LSBFIRST`：最右邊（低位元）先送
資料值：一個 `byte` 數值（8 位元）
```

位元順序的差異說明：
```cpp
假設：
byte ledData = 0b11100000;

LSBFIRST 傳輸：00000111（最低位元在前）
MSBFIRST 傳輸： 11100000（最高位元在前）
```


### 3.2. Lab：使用 74595 IC 連接七段顯示器，並在七段顯示器上每隔一秒顯示 0~9數字。


### 3.3. Lab：使用一個 74595 IC 控制多個七段顯示器















































































































































































































































































































































































































































































































































































































































































































































































































































