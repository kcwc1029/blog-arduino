在電源輸出端串聯一個電阻，即可降低電壓，避免大電流直接灌入:ED
驅動LED不需要太多電力，但如果是馬達或其他大電流的附載，則會浪費許多電力
電阻所消耗的電能也會轉換成熱能

## 1. PWM
定義：是一種透過開關頻率與開關時間比例（duty cycle）來控制電壓或功率輸出的技術。

PWM 訊號是一種數位訊號（Digital Signal）：
- 高電位（例如 5V） 與 低電位（例如 0V） 快速交替
- 用「開啟時間比例（Duty Cycle）」來控制平均輸出電壓
Duty Cycle（工作週期）：指「高電位所占時間」在一個週期中的比例

| Duty Cycle | 平均輸出電壓 (以 5V 為例) |
| ---------- | ---------------- |
| 100%       | 5V（完全導通）         |
| 50%        | 2.5V             |
| 10%        | 0.5V             |
| 0%         | 0V（完全關閉）         |

```
公式：輸出電壓 = Duty Cycle × 高電位值

例如：
輸出電壓 = 66% × 5V = 3.3V
```

### 1.1. PWM 的應用方式

改變頻率（切換速度）
- 頻率（Frequency）：1 秒內週期出現的次數，單位為 Hz
- 通常頻率需 **高於 30Hz**，人眼才無法察覺閃爍（例如 LED）
模擬類比電壓輸出
- 雖然是數位訊號，但由於頻率夠高，會讓設備「感覺」像是某種類比電壓（類似 DAC 效果）

### 1.2. Lab：randomPWM值操作LED

![upgit_20250410_1744268415.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744268415.png)

```c
const byte LED_PIN = 9;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  byte pwm_val = random(0, 255);
  analogWrite(LED_PIN, pwm_val);             // 寫入 PWM 值調整亮度
  delay(1000);
}

```


### 1.3. Lab：利用可變電阻調整PWM操作LED
![upgit_20250410_1744268052.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744268052.png)

```c
const byte LED_PIN = 9;
const byte POT_PIN = A0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
}

void loop() {
  int pot_val = 0;
  byte pwm_val = 0;

  pot_val = analogRead(POT_PIN);             // 讀取電位器輸出（0~1023）
  pwm_val = map(pot_val, 0, 1023, 0, 255);    // 將值轉成 PWM（0~255）
  analogWrite(LED_PIN, pwm_val);             // 寫入 PWM 值調整亮度
  delay(100);
}

```


## 2. 電晶體


基本認識：
- 微控制器（如 Arduino）輸出的電壓/電流太小，無法直接驅動馬達或大電流設備。
- 需要一種「放大元件」：電晶體（Transistor）來當作電力「開關」或「介面」使用。
![upgit_20250410_1744271910.png|501x172](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744271910.png)


電晶體的三個腳位：
- **B (Base)**：基極，控制腳位 
- **C (Collector)**：集電極，接電源正極
- **E (Emitter)**：射極，接電源負極（或地）
![upgit_20250410_1744272139.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744272139.png)


![upgit_20250410_1744272107.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744272107.png)

![upgit_20250410_1744272152.png](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744272152.png)


### 2.1. 電晶體類型：NPN vs PNP
根據製造結構，電晶體可以分為NPN與PNP兩種。

| 類型  | 電流方向         | 控制方式           | 用途               |
| --- | ------------ | -------------- | ---------------- |
| NPN | C → E（集電→射極） | B 接高電位（如電源）時導通 | 比較常見，用來當作「開關控制」。 |
| PNP | E → C（射極→集電） | B 接低電位（如接地）時導通 | 用於不同邏輯需求。        |

![upgit_20250410_1744280954.png|678x210](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744280954.png)



## 3. 馬達(有時間再補)
### 3.1. 直流馬達（DC Motor）
結構與控制
- 結構：由磁鐵、轉子與碳刷組成。
- 控制：只需將電源正負極與馬達相連即可控制正轉/反轉。
問題
- 會因碳刷與整流子接觸產生火花與雜訊干擾。
- 影響微處理器、無線遙控等裝置。
- 解法：通常會在馬達正負極之間並聯電容（如 0.01µF～0.1µF）來抑制雜訊。

![upgit_20250410_1744269760.png|661x192](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744269760.png)

![upgit_20250410_1744270030.png|639x162](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744270030.png)

![upgit_20250410_1744270183.png|627x137](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250410_1744270183.png)

### 3.2. 伺服馬達
![upgit_20250411_1744380362.png|463x185](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744380362.png)

![upgit_20250411_1744380519.png|433x224](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744380519.png)


伺服馬達（servo motor）是一種可以控制轉動角度的馬達，常用於精密定位的應用中，例如機械手臂、遙控模型、機器人、飛機舵面控制等。

#### 3.2.1. 組成：
馬達（通常為直流馬達）
減速齒輪
馬達控制電路（含回授）
位置感測元件（例如可變電阻）

![upgit_20250411_1744380532.png|415x222](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744380532.png)

#### 3.2.2. 運作原理
控制器送出 PWM 訊號（Pulse Width Modulation） → 控制轉角
馬達根據 PWM 的脈波寬度旋轉到指定角度
內部回授機制比較目標角度與目前角度，修正誤差


PWM 控制原理
- 每週期長度：20ms（頻率 50Hz）
- 脈波寬度決定轉角：
    - 0.5ms ➝ 0°
    - 1.5ms ➝ 中間位置（90°）
    - 2.5ms ➝ 180°

> 脈波寬度越大，轉角越大（SG90 例為 0°~180°）

![upgit_20250411_1744380567.png|414x175](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744380567.png)

![upgit_20250411_1744380579.png|624x196](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744380579.png)

### 3.3. Lab：操作伺服馬達
![upgit_20250411_1744380876.png|405x401](https://raw.githubusercontent.com/kcwc1029/obsidian-upgit-image/main/2025/04/upgit_20250411_1744380876.png)

```c
#include <Servo.h>  // 引用內建 Servo 函式庫

Servo myservo;      // 建立 Servo 物件

void setup() {
  myservo.attach(11);  // 將伺服馬達接在腳位 11
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 1) { // 從 0 轉到 180 度
    myservo.write(pos);                    
    delay(15);                             
  }
  for (int pos = 180; pos >= 0; pos -= 1) { // 從 180 轉回 0 度
    myservo.write(pos);                    
    delay(15);                             
  }
}
```


### 3.4. 步進馬達（Stepper Motor）
特色：
- 是能精確控制角度與位置的馬達，常用在需要控制精確定位的裝置，如：光碟機、印表機滑軌等。
- 每次轉動一個固定角度，例如：1.8° → 200 步 = 360°
- 控制需搭配驅動電路與微處理器產生脈衝訊號。
控制原理
- 透過微處理器 → 脈衝產生器 → 驅動電路 → 步進馬達。
- 每產生一組脈衝訊號，馬達軸就轉動一步。
優點
- 高精度定位控制。
- 不需要回授裝置就能定位。



