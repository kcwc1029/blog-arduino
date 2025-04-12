## 1. RFID 基礎知識

定義：RFID 是一種**無線射頻辨識技術**，透過**無接觸方式讀取或寫入資訊**。

| 條碼                     | RFID                   |
| ------------------------ | ---------------------- |
| 需對準、掃描器與標籤接觸 | 無接觸、可穿透物體辨識 |
| 一次只能掃一個           | 可同時辨識多個         |
| 資料僅能讀取             | 可讀可寫               |

三大組成：

1. 電子標籤（Tag）：儲存資料的晶片與天線
2. 讀卡機（Reader）：發射無線波與接收標籤回傳資料
3. 主控端（Host）：如 Arduino 或電腦，負責解碼分析資料

![upgit_20250412_1744467147.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250412_1744467147.png)

RFID 應用實例

-   學生證、門禁卡
-   電子票證（悠遊卡）
-   倉儲物流追蹤
-   智慧購物（商品自動結帳）

### 1.1. RFID 類型分類

按**外型與用途**分類：

-   🧾 智慧標籤（Smart Label）：如紙張般可貼附於商品上
-   🔑 鑰匙圈型（Key Fob）：常見門禁卡
-   💊 玻璃管型（Glass Tube）：動物晶片，植入動物體內

按**電源方式**分類：

-   被動式（Passive Tag）：無內建電源、靠讀取器供電，便宜但讀取距離較短（如悠遊卡）
-   主動式（Active Tag）：內建電池、可主動傳送訊號，成本高但可遠距追蹤（如物流貨櫃追蹤）

按**資料寫入方式**分類：

-   唯讀型（Read Only）：資料寫入後無法改變
-   WORM（Write Once, Read Many）：寫入一次、可重複讀
-   可讀可寫型：可多次修改資料內容（如門禁卡）

### 1.2. RFID 頻率

| 頻率                    | 代表範圍 | 距離                               | 特性 |
| ----------------------- | -------- | ---------------------------------- | ---- |
| LF（低頻）125kHz        | < 0.5m   | 穿透力強、抗干擾、傳輸速度慢       |      |
| HF（高頻）13.56MHz      | < 1m     | 成本低、常見於學生證               |      |
| UHF（超高頻）860-960MHz | ~3m      | 快速掃描、大量應用（如倉儲物流）   |      |
| 微波 2.45GHz            | > 4m     | 傳輸速度最快、受干擾多、穿透力較弱 |      |

### 1.3. 補充：NFC（近場通訊）

定義：NFC 是 RFID 的一種衍生應用，稱作「近場通訊」，支援**雙向資料交換**。
特點：

-   距離需 < 10 公分
-   常見於：手機支付、感應門卡、電子票證、自動登錄

### 1.4. Lab：讀取 RFID 卡的 UID 碼

### 1.5. Lab：門禁卡實驗

## 2. Mifare Classic 1KB 記憶體結構

Mifare Classic 類型德 RFID 卡內部都會有 1KB 的 EEPROM 記憶體
sector0 的 block0 包含卡片唯一的 UID

![upgit_20250412_1744467891.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250412_1744467891.png)

### 2.1. 每個 sector 的組成

記憶體被劃分為 16 個區段(sector)，每個 sector 會有 4 個 block
每個 dector 的 block3 為 Trailer Block

-   控制區 (Trailer Block) 包含兩組密碼（Key A / Key B）及存取控制位元（Access Bits）

| 區塊編號  | Block 0        | Block 1 | Block 2 | Block 3（最後）         |
| --------- | -------------- | ------- | ------- | ----------------------- |
| 區段 0    | UID + 其他資料 | 資料區  | 資料區  | 控制區（Trailer Block） |
| 區段 1~15 | 資料區         | 資料區  | 資料區  | 控制區（Trailer Block） |

![upgit_20250412_1744468048.png|764x246](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250412_1744468048.png)

Key B 為公開
Key A 則因安全考量，在掃描時，會全部顯示 00
存取控制位元用於控制 block0-block3 是否能存取操作。
0XFF0780 為 block3 的出場預設值，代表：

-   Key A 不可見
-   若通過 Key A 或 Key B 驗證，即可讀取/操作 sector 裡面的 block0-block2。
-   若通過 Key A 驗證，可讀取/操作該 sector 裡面的 Key A、Key B 與存取控制位元。

### 2.2. Lab：儲值卡實驗
