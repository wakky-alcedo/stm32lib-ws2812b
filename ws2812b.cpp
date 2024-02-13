#include <ws2812b.hpp>

namespace ws2812b {

/**
 * コンストラクタ
 * @param htim ws2812bを使用するタイマ
 * @param channel ws2812bを使用するタイマのチャンネル
 * @param led_num 使用するLEDの数
 */
WS2812B::WS2812B(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t led_num)
    : htim(htim),
      channel(channel),
      led_num(led_num)
{
    color_rgb.resize(led_num, RGB_t{0,0,0});
    pwm_data.resize((24*led_num)+res_frame);
}

/**
 * 初期化
 */
void WS2812B::init() {
    // make 1clk = 1.25µs -> 800kHz
    uint32_t timClkFreq = HAL_RCC_GetPCLK1Freq() * 2;  // timer freq is faster 2times!
    __HAL_TIM_SET_PRESCALER(htim, 0);
    uint16_t period = timClkFreq / 800000;
    __HAL_TIM_SET_AUTORELOAD(htim, period - 1);
    bit0 = period * 1 / 3.0f;
    bit1 = period * 2 / 3.0f;
}

/**
 * 色をRGBで設定
 * @param led_id LEDの番号(0 ~ led_num-1)
 * @param red 赤の明るさ(0 ~ 255)
 * @param green 緑の明るさ(0 ~ 255)
 * @param blue 青の明るさ(0 ~ 255)
 */
void WS2812B::set_color_rgb(uint8_t led_id, uint8_t red, uint8_t green, uint8_t blue){
    color_rgb[led_id].green = green;
    color_rgb[led_id].red   = red;
    color_rgb[led_id].blue  = blue;
}

/**
 * 色をRGBで設定
 * @param led_id LEDの番号(0 ~ led_num-1)
 * @param rgb 構造体RGB_t
 */
void WS2812B::set_color_rgb(uint8_t led_id, RGB_t rgb){
    color_rgb[led_id] = rgb;
}

/**
 * 色をRGBで設定
 * @param rgb 構造体RGB_tのvector配列
 */
void WS2812B::set_color_rgb(std::vector<RGB_t> rgb){
    color_rgb = rgb;
}

/**
 * 色をHSVで設定
 * @param led_num LEDの番号(0 ~ led_num-1)
 * @param hue 色相(0 ~ 360)
 * @param saturation 彩度(0 ~ 100)
 * @param value 明度(0 ~ 100)
 */
void WS2812B::set_color_hsv(uint8_t led_num, float hue, float saturation, float value)
{
    float r, g, b;

    hue        = hue / 360;
    saturation = saturation / 100;
    value      = value / 100;

    auto i = static_cast<int>(hue * 6);
    auto f = hue * 6 - i;
    auto p = value * (1 - saturation);
    auto q = value * (1 - f * saturation);
    auto t = value * (1 - (1 - f) * saturation);

    switch (i % 6)
    {
    case 0: r = value , g = t , b = p;
        break;
    case 1: r = q , g = value , b = p;
        break;
    case 2: r = p , g = value , b = t;
        break;
    case 3: r = p , g = q , b = value;
        break;
    case 4: r = t , g = p , b = value;
        break;
    case 5: r = value , g = p , b = q;
        break;
    }

    color_rgb[led_num].red   = static_cast<uint8_t>(r * 255);
    color_rgb[led_num].green = static_cast<uint8_t>(g * 255);
    color_rgb[led_num].blue  = static_cast<uint8_t>(b * 255);
}

/**
 * 色をHSVで設定
 * @param led_id LEDの番号(0 ~ led_num-1)
 * @param hsv 構造体HSV_t
 */
void WS2812B::set_color_hsv(uint8_t led_id, HSV_t hsv){
    set_color_hsv(led_id, hsv.hue, hsv.sat, hsv.val);
}

/**
 * 色をHSVで設定
 * @param hsv 構造体HSV_tのvector配列
 */
void WS2812B::set_color_hsv(std::vector<HSV_t> hsv){
    for (uint8_t i = 0; i < hsv.size(); i++) {
        set_color_hsv(i, hsv[i]);
    }
}

/**
 * PWMを出力
 */
void WS2812B::send(void){
    HAL_TIM_PWM_Stop_DMA(htim, channel);
    uint32_t indx = 0;
    uint32_t color;
    for (uint8_t i = 0; i < led_num; i++) {
//        color = ((color_rgb[i].green<<16) | (color_rgb[i].red<<8) | (color_rgb[i].blue)); // そのまま
        color = ((gamma8[color_rgb[i].green]<<16) | (gamma8[color_rgb[i].red]<<8) | (gamma8[color_rgb[i].blue])); // ガンマ補正あり
        for (int j = 23; j >= 0; j--) {
            if (color&(1<<j)) pwm_data[indx] = bit1;
            else pwm_data[indx] = bit0;
            indx++;
        }
    }

    for (uint8_t i = 0; i < res_frame; i++) {
        pwm_data[indx] = 0;
        indx++;
    }

    HAL_TIM_PWM_Start_DMA(htim, channel, (uint32_t*)pwm_data.data(), indx);
}

} /* namespace ws2812b */
