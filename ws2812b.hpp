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
