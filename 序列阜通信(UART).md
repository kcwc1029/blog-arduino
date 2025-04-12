## 1. 傳輸方式
- 並列（Parallel）通訊：一次傳送多個位元（例如 8 位元），每條線傳送一個位元。
- 序列（Serial）通訊：一次只傳送一個位元，用一條線依序傳送每一位元。

並列與序列的比較

|項目|並列（Parallel）|序列（Serial）|
|---|---|---|
|傳輸速度|快|較慢|
|傳輸距離|短（易受干擾）|長（抗干擾能力強）|
|成本與複雜度|高（需要多條傳輸線）|低（只需一條線）|
|常見應用|電腦內部傳輸（如 PCI 匯流排）|長距離裝置間傳輸（如 USB、SATA）|


## 2. USB 介面與 USB 序列通訊埠
USB 全名：Universal Serial Bus（通用序列匯流排）
目的是取代 RS-232 序列埠與 DB25 並列埠，統一連接標準。
USB 設備分為：
- 主控端（Host）：電腦、筆電
- 從端（Client Device）：滑鼠、鍵盤、隨身碟、手機等

UNO R3 控制板上的 USB 僅為「從端」，因此無法與其他 USB 裝置連接。

### 2.1. USB 接頭類型
![upgit_20250402_1743567658.png|571x229](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743567658.png)

| 接頭類型 | 常見用途 |
|----------|-----------|
| **Type A** | 主機端常見，例如電腦 |
| **Type B** | 印表機、早期硬碟外接盒 |
| **Type C** | 新一代雙面可插、支援高功率與高速資料傳輸 |
| **Micro-B** | 舊款手機、行動裝置常見 |

## 3. UART
### 3.1. RS-232序列
RS-232 是最早廣泛使用的 UART 序列埠標準。
發展於 1969 年，其中 RS 代表 Recommended Standard。
過去常見於桌上型電腦（例如：COM1、COM2...），每個 COM 埠只能連接一個裝置。

![upgit_20250402_1743567236.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743567236.png)

普通的IC高低電位變化，稱為TTL訊號，TTL訊號容易受到雜訊干擾。而RS-232為了提高抗雜訊能力，把訊號電為提升至正負3-正負15v

![upgit_20250402_1743567370.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743567370.png)


### 3.2. 序列資料傳輸協定（Serial Communication Protocol）

傳輸協定（Protocol）
- 是設備雙方通訊的規範與參數。
- 雙方設定需一致，否則會出現亂碼，無法溝通。
- 在 Windows 的裝置管理員中可查看 USB 序列埠裝置。

鮑率(baud rate)：序列副的傳輸速度(每秒為元速bit per second, bps)
- 兩個設備的baud rate需要一置
- 常用9600、115200bps

![upgit_20250402_1743568216.png|527x218](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743568216.png)

一個UART介面只能跟一個周邊通信

![upgit_20250402_1743567145.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743567145.png)

![upgit_20250402_1743568283.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743568283.png)

#### 3.2.1. macOS 與 Linux 的通訊埠

Windows 使用 COM 來代表序列通訊埠
macOS 與 Linux 則使用下面兩者來表示通訊埠。
- TTY（TeleTYpewriter, 電傳打字機）=> 被動接收數據（例如監聽輸出）
- CU（Call-Up，撥號器）=> CU 則用於 主動連接設備（例如傳送指令到 Arduino）

```
// 在 macOS 或 Linux 的終端機 (Terminal) 中，輸入以下指令來查看可用的 TTY 或 CU 設備
ls -l /dev/tty.* // 列出所有 TTY 設備
ls -l /dev/cu.* // 列出所有 CU 設備
```

### 3.3. UNO R3 的序列通訊方式

UNO R3 透過 USB 傳 TTL 訊號。
若要與真正的 RS-232 裝置通訊，需要加裝電壓轉換晶片（如 **MAX232**）。
UNO R3的第0腳(RX)與第1腳(TX)是UART街角，同時也連接到USB轉TTL訊號晶片(MEGA16U2微控器)。

![upgit_20250402_1743567781.png|654x402](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743567781.png)

### 3.4. 需要外接USB轉UART序列線的開發版
有些開發板（如 Arduino Pro Mini）本身沒有 USB 接頭，也不具備 USB 介面。
開發時需要額外接一個 USB 轉 UART 轉接板。
只有在以下兩種情況才需要接上 USB 轉 UART 板：
- 上傳程式碼
- 使用電腦的序列埠監控視窗（Serial Monitor）檢查訊息

![upgit_20250402_1743568637.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250402_1743568637.png)

## 4. 認識字元與字串資料類型
字元 (char)
- 用單引號 (' ') 包起來
	- 例如: char data = 'A';
- 字元本身是數字 (例如 'A' = 65)
ASCII 編碼
- 表示字元與數字之間的映射
- 系統使用 ASCII 來處理字元
控制字元 (無法顯示字元)
- 例如 CR(\r), LF(\n), Tab, Backspace...
- 據表格為根據 ASCII 編碼
字串
- 用雙引號 (") 包起來: char str[] = "Arduino";
- 字串最後會自動加 Null ('\0') 來表示結束

```cpp
// 字串定義方式
char str1[] = {'h','e','l','l','o','\0'};
char str2[6] = {'h','e','l','l','o',0};
```

使用 Serial 函式庫
- Serial.begin(9600)：設定序列通訊速度
- Serial.print() /Serial.println()：傳送資料(字串/數值)
- Serial.write()：傳送位元資料(不做格式化)


### 4.1. 認識 String 類別與方法
String 是類別，不只是字串
可彈性儲存與操作文字資料，長度可變
```cpp
// 宣告方式
String micro = "Arduino";

// 串接文字與數值
Serial.print(String("LED pin is: ") + ledPin);
```

常用方法

| 方法                | 功能            |
| ----------------- | ------------- |
| `.length()`       | 字串長度（不含 Null） |
| `.charAt(i)`      | 取得第 i 個字元     |
| `.substring(a,b)` | 取出索引 a~b 的字串  |
| `.equals("str")`  | 判斷字串是否相等      |
| `.toLowerCase()`  | 字串轉小寫         |


### 4.2. Lab：從序列埠監控窗觀察變數數值
```cpp
byte ledPin = 13;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Hello, ");
  Serial.print("\tLED pin is: ");
  Serial.print(ledPin);
  Serial.println("\nBYE!");
}

void loop() {}

// 輸出
// Hello, 
// LED pin is: 13
// BYE!
```

### 4.3. `Serial.print()` vs `Serial.write()` 的差別

共同點：兩者都可以把資料透過序列埠送出去。
差異 1：處理資料的方式不同
- Serial.print()會把 `"hello\n"` 當作一段「文字（字串）」送出。
- `Serial.write`會把 `"hello\n"` 當作「一組位元組陣列」來處理，送出的是每個字元的 ASCII 編碼位元資料。
差異 2：處理數值時更明顯！
- `Serial.print(65);` 會把 數字 65 轉成 「字串」"65"（就是字元 '6' 跟 '5'）
	- 所以送出兩個字元：6 和 5 的 ASCII
- `Serial.write(65);`  會把 65 當作一個 位元組（byte）送出
	- 就是送出 ASCII 編碼 65 對應的字元 → 'A'

### 4.4. 字串節省記憶體技巧（F() 巨集）
- Serial.print("字串") 預設會佔用 SRAM（主記憶體）
- 使用 F("字串") 可讓常數字串存在 Flash（程式記憶體）中，節省 SRAM
```cpp
Serial.println(F("Always do what you're afraid to do."));
```

### 4.5. 設定序列埠輸出的數字格式
預設
- Serial.print() 預設會輸出 10 進位數字。
* 浮點數預設會輸出 小數點後兩位。
 
```cpp
// 調整小數位數與進制
Serial.print(42, BIN);  // 輸出 "101010"
Serial.print(3.14159);        // 輸出 "3.14"（預設兩位小數）
Serial.print(3.14159, 0);     // 輸出 "3"
Serial.print(3.14159, 4);     // 輸出 "3.1415"
```
