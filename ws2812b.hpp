#ifndef WS2812B_HPP_
#define WS2812B_HPP_

#include "stdint.h"
#include "tim.h"
#include <vector>
#include <functional>
#include <map>

namespace ws2812b {

struct RGB_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct HSV_t {
    float hue; // 色相 (0~360)
    float sat; // 彩度 (0~100)
    float val; // 明度 (0~100)
};

const uint8_t gamma8[] = { // γ = 2.8くらい
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
  2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
  5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

class WS2812B {
private:
    TIM_HandleTypeDef *htim;
    uint32_t channel;

    uint16_t led_num; // LEDの個数

    uint16_t bit0;
    uint16_t bit1;

    std::vector<RGB_t> color_rgb;

    uint8_t datasentflag = 0;

public:
    WS2812B(TIM_HandleTypeDef *htim,
            uint32_t channel,
            uint16_t led_num);

    void init();

    void set_color_rgb(uint8_t led_id, uint8_t red, uint8_t green, uint8_t blue);
    void set_color_rgb(uint8_t led_id, RGB_t rgb);
    void set_color_rgb(std::vector<RGB_t> rgb);
    void set_color_hsv(uint8_t led_id, float hue, float saturation, float value);
    void set_color_hsv(uint8_t led_id, HSV_t hsv);
    void set_color_hsv(std::vector<HSV_t> hsv);
    void send(void);
    void run_pulse_finished(void);
};

} /* namespace ws2812b */

#endif /* WS2812B_HPP_ */
