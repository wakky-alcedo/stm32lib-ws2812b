# WS2812B - 1.0.0

```c++
#include "ws2812b.hpp"
```

```c++
namespace ws2812b
```

---
## CubeMx
```yaml
PinOut: TIMn_CHm
TIMn:
    - Mode
        - Clock Scurce を Internal Clock に設定
        - Channelm を PWM Generation CHm に設定
    - Configuration
        - DMA Settings
            - add から TIMn を追加
            - Direction: Memory to Peripheral


```

## サンプルプログラム

### コンストラクタ
```c++
constexpr uint8_t max_led = 8;
WS2812B tape_led(&htim3, TIM_CHANNEL_1, max_led);
```

### 初期化処理
```c++
tape_led.init();
```

### 色を変更
```C++
for (int i=0; i<360; i++){
    for (uint8_t j = 0; j < max_led; j++) {
        // どれかのコメントアウトを解除する
//        tape_led.set_color_hsv(j, 360.0f*j/MAX_LED + i*8, 100, 50); // ゲーミング
//        tape_led.set_color_hsv(j, 360.0f*j/MAX_LED + i, 100, 100.0f*(MAX_LED-j)/MAX_LED); // ゲーミング＋固定明るさグラデーション
        tape_led.set_color_hsv(j, 360.0f*j/max_led + i, 100.0f*(max_led-j)/max_led, 50); // ゲーミング＋固定彩度グラデーション
    }
    tape_led.send();
    HAL_Delay (3);
}
```

### その他
以上で紹介したものの他にも、RGBで色設定する関数などもある。
詳細はプログラムを確認すること。
