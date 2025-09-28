// 新文件：main/led_test_main.c
/*
 * 独立LED灯板测试程序
 * 功能：测试所有LED的不同状态和闪烁模式
 * 去除了网络通信、云服务、Modbus等复杂功能
 */

#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_task_wdt.h"
#include "esp_log.h"

#define TAG "LED_TEST"

// LED引脚定义
#define BAT_R_PIN        (13)
#define BAT_G_PIN        (12)
#define BAT_B_PIN        (14)

#define INVERTER_R_PIN   (5)
#define INVERTER_G_PIN   (17)
#define INVERTER_B_PIN   (16)

#define CHARGE_R_PIN     (23)
#define CHARGE_G_PIN     (22)
#define CHARGE_B_PIN     (19)

#define PV_G_PIN         (2)
#define GRID_G_PIN       (4)
#define EMS_G_PIN        (18)
#define RUN_G_PIN        (15)

// LED控制参数
#define LED_BRIGHTNESS_MAX  255
#define LED_BLINK_PERIOD    20000    // 50us * 20000
#define LED_BLINK_OPEN_TIME (LED_BLINK_PERIOD >> 2)

// LED状态枚举
typedef enum {
    CLOSE = 0,
    RED_STEADY_ON,
    RED_BLINK,
    YELLOW_STEADY_ON,
    YELLOW_BLINK,
    BLUE_STEADY_ON,
    BLUE_BLINK,
    GREEN_STEADY_ON,
    GREEN_BLINK,
    WHITE_STEADY_ON,
    WHITE_BLINK,
    ORANGE_STEADY_ON,
    ORANGE_BLINK
} LedState_enum;

// LED结构体
typedef struct {
    uint8_t mType;            // LED类型, 0 = 单色, 1 = RGB
    LedState_enum mState;     // LED状态
    
    uint8_t mRGBTimeCount;    // 颜色计数器
    uint32_t mLedTimeCount;   // 通用时间计数器，用于闪烁
    
    void (*setR)(int open_or_close);
    void (*setG)(int open_or_close);
    void (*setB)(int open_or_close);
} LED_struct;

// LED控制函数声明
void setR_BAT_LED(int s);
void setG_BAT_LED(int s);
void setB_BAT_LED(int s);
void setR_Inverter_LED(int s);
void setG_Inverter_LED(int s);
void setB_Inverter_LED(int s);
void setR_Charge_LED(int s);
void setG_Charge_LED(int s);
void setB_Charge_LED(int s);
void setG_GRID_LED(int s);
void setG_PV_LED(int s);
void setG_EMS_LED(int s);
void setG_RUN_LED(int s);

// LED对象实例
LED_struct gBatLED = {1, CLOSE, 0, 0, setR_BAT_LED, setG_BAT_LED, setB_BAT_LED};
LED_struct gInverterLED = {1, CLOSE, 0, 0, setR_Inverter_LED, setG_Inverter_LED, setB_Inverter_LED};
LED_struct gChargeLED = {1, CLOSE, 0, 0, setR_Charge_LED, setG_Charge_LED, setB_Charge_LED};
LED_struct gGridLED = {1, CLOSE, 0, 0, NULL, setG_GRID_LED, NULL};
LED_struct gPvLED = {1, CLOSE, 0, 0, NULL, setG_PV_LED, NULL};
LED_struct gNetLED = {1, CLOSE, 0, 0, NULL, setG_EMS_LED, NULL};
LED_struct gRunLED = {1, CLOSE, 0, 0, NULL, setG_RUN_LED, NULL};

// LED GPIO控制函数实现
void setR_BAT_LED(int s) { gpio_set_level(BAT_R_PIN, s); }
void setG_BAT_LED(int s) { gpio_set_level(BAT_G_PIN, s); }
void setB_BAT_LED(int s) { gpio_set_level(BAT_B_PIN, s); }

void setR_Inverter_LED(int s) { gpio_set_level(INVERTER_R_PIN, s); }
void setG_Inverter_LED(int s) { gpio_set_level(INVERTER_G_PIN, s); }
void setB_Inverter_LED(int s) { gpio_set_level(INVERTER_B_PIN, s); }

void setR_Charge_LED(int s) { gpio_set_level(CHARGE_R_PIN, s); }
void setG_Charge_LED(int s) { gpio_set_level(CHARGE_G_PIN, s); }
void setB_Charge_LED(int s) { gpio_set_level(CHARGE_B_PIN, s); }

void setG_GRID_LED(int s) { gpio_set_level(GRID_G_PIN, s); }
void setG_PV_LED(int s) { gpio_set_level(PV_G_PIN, s); }
void setG_EMS_LED(int s) { gpio_set_level(EMS_G_PIN, s); }
void setG_RUN_LED(int s) { gpio_set_level(RUN_G_PIN, s); }

// LED初始化函数
void ledBoardInit(void)
{
    // 初始化所有LED引脚
    gpio_reset_pin(BAT_R_PIN);
    gpio_set_direction(BAT_R_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(BAT_G_PIN);
    gpio_set_direction(BAT_G_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(BAT_B_PIN);
    gpio_set_direction(BAT_B_PIN, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(INVERTER_R_PIN);
    gpio_set_direction(INVERTER_R_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(INVERTER_G_PIN);
    gpio_set_direction(INVERTER_G_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(INVERTER_B_PIN);
    gpio_set_direction(INVERTER_B_PIN, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(CHARGE_R_PIN);
    gpio_set_direction(CHARGE_R_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CHARGE_G_PIN);
    gpio_set_direction(CHARGE_G_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CHARGE_B_PIN);
    gpio_set_direction(CHARGE_B_PIN, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(PV_G_PIN);
    gpio_set_direction(PV_G_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(GRID_G_PIN);
    gpio_set_direction(GRID_G_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(EMS_G_PIN);
    gpio_set_direction(EMS_G_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(RUN_G_PIN);
    gpio_set_direction(RUN_G_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG, "LED board initialized");
}

// RGB LED控制函数
void setRGB(LED_struct *led, uint8_t r, uint8_t g, uint8_t b)
{
    if (led->mRGBTimeCount < r) {
        if (led->setR != NULL)
            led->setR(0);
    } else {
        if (led->setR != NULL)
            led->setR(1);
    }
    
    if (led->mRGBTimeCount < g) {
        if (led->setG != NULL)
            led->setG(0);
    } else {
        if (led->setG != NULL)
            led->setG(1);
    }
    
    if (led->mRGBTimeCount < b) {
        if (led->setB != NULL)
            led->setB(0);
    } else {
        if (led->setB != NULL)
            led->setB(1);
    }
}

// LED状态设置函数
void setLed_State(LED_struct *led, LedState_enum state)
{
    led->mState = state;
    ESP_LOGI(TAG, "LED state set to: %d", state);
}

// RGB LED基础任务函数
void RGB_LedBaseTask(LED_struct *rgbLED)
{
    // 闪烁模式时
    if ((rgbLED->mState % 2) == 0) {
        rgbLED->mLedTimeCount++;
        if (rgbLED->mLedTimeCount >= LED_BLINK_PERIOD) {
            rgbLED->mLedTimeCount = 0;
        } else if (rgbLED->mLedTimeCount > LED_BLINK_OPEN_TIME) {
            setRGB(rgbLED, 0x00, 0x00, 0x00);
            return;
        }
    }
    
    rgbLED->mRGBTimeCount++;
    if (rgbLED->mRGBTimeCount >= LED_BRIGHTNESS_MAX) {
        rgbLED->mRGBTimeCount = 0;
    }
    
    switch (rgbLED->mState) {
        case CLOSE:
            setRGB(rgbLED, 0x00, 0x00, 0x00);
            break;
        case RED_STEADY_ON:
            setRGB(rgbLED, 0xFF, 0x00, 0x00);
            break;
        case RED_BLINK:
            setRGB(rgbLED, 0xFF, 0x00, 0x00);
            break;
        case YELLOW_STEADY_ON:
            setRGB(rgbLED, 0xFF, 0xFF, 0x00);
            break;
        case YELLOW_BLINK:
            setRGB(rgbLED, 0xFF, 0xFF, 0x00);
            break;
        case BLUE_STEADY_ON:
            setRGB(rgbLED, 0x00, 0x00, 0xFF);
            break;
        case BLUE_BLINK:
            setRGB(rgbLED, 0x00, 0x00, 0xFF);
            break;
        case GREEN_STEADY_ON:
            setRGB(rgbLED, 0x00, 0xFF, 0x00);
            break;
        case GREEN_BLINK:
            setRGB(rgbLED, 0x00, 0xFF, 0x00);
            break;
        case WHITE_STEADY_ON:
            setRGB(rgbLED, 0xFF, 0xFF, 0xFF);
            break;
        case WHITE_BLINK:
            setRGB(rgbLED, 0xFF, 0xFF, 0xFF);
            break;
        case ORANGE_STEADY_ON:
            setRGB(rgbLED, 0xFF, 0x80, 0x00);
            break;
        case ORANGE_BLINK:
            setRGB(rgbLED, 0xFF, 0x80, 0x00);
            break;
        default:
            break;
    }
}

// 定时器回调函数 - 50us精确控制LED
static bool timerCallback_50us(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    // 执行所有LED的基础任务
    RGB_LedBaseTask(&gBatLED);
    RGB_LedBaseTask(&gInverterLED);
    RGB_LedBaseTask(&gChargeLED);
    RGB_LedBaseTask(&gGridLED);
    RGB_LedBaseTask(&gPvLED);
    RGB_LedBaseTask(&gNetLED);
    RGB_LedBaseTask(&gRunLED);
    
    return 0;
}

// 定时器初始化
void timerInit(void)
{
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
    
    // 设置ISR
    gptimer_event_callbacks_t cbs = {
        .on_alarm = timerCallback_50us,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    
    // 启用定时器
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    
    // 设置事件
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 50, // period = 50us
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
    
    ESP_LOGI(TAG, "50us timer initialized");
}

// LED测试任务
void led_test_task(void *arg)
{
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    
    uint32_t test_counter = 0;
    const char* state_names[] = {
        "OFF", "RED ON", "YELLOW ON", "BLUE ON", "GREEN ON", "WHITE ON", "ORANGE ON"
    };
    
    ESP_LOGI(TAG, "Start LED test loop");
    
    while (1) {
        // 每10秒切换一次LED状态进行测试
        LedState_enum single_states[] = {
            CLOSE, RED_STEADY_ON, YELLOW_STEADY_ON, BLUE_STEADY_ON, GREEN_STEADY_ON, WHITE_STEADY_ON, ORANGE_STEADY_ON
        };
        int state_count = sizeof(single_states) / sizeof(single_states[0]);
        LedState_enum current_state = single_states[test_counter % state_count];
        ESP_LOGI(TAG, "=== LED test round %lu ===", test_counter / state_count + 1);
        ESP_LOGI(TAG, "Current test state: %s (%d)", state_names[test_counter % state_count], current_state);
        // 设置所有LED为相同状态进行测试
       setLed_State(&gBatLED,9);
        //setLed_State(&gInverterLED, current_state);
      //  setLed_State(&gChargeLED, current_state);
       // setLed_State(&gGridLED, current_state);
        //setLed_State(&gPvLED, current_state);
        //setLed_State(&gNetLED, current_state);
        //setLed_State(&gRunLED, current_state);
        // 显示系统状态
        if (test_counter % state_count == 0) {
            ESP_LOGI(TAG, "System running normally - Free heap: %d bytes", xPortGetFreeHeapSize());
        }
        test_counter++;
        // 10秒延迟期间分段喂狗，防止看门狗超时
        for (int i = 0; i < 10; i++) {
            esp_task_wdt_reset();
            vTaskDelay(1000 / portTICK_PERIOD_MS); // 每1秒喂一次狗
        }
    }
}

// 主程序入口
void app_main(void)
{
    ESP_LOGI(TAG, "=== LED test program started ===");
    ESP_LOGI(TAG, "Version: 1.0");
    ESP_LOGI(TAG, "Function: Test all LED states and blink modes");
    
    // 初始化看门狗
#if !CONFIG_ESP_TASK_WDT_INIT
    esp_task_wdt_config_t twdt_config = {
        .timeout_ms = 10000,
        .idle_core_mask = (1 << CONFIG_FREERTOS_NUMBER_OF_CORES) - 1,
        .trigger_panic = false,
    };
    ESP_ERROR_CHECK(esp_task_wdt_init(&twdt_config));
    ESP_LOGI(TAG, "Task watchdog timer initialized");
#endif

    // 初始化NVS（虽然不使用，但ESP32可能需要）
    nvs_flash_init();
    
    // 初始化LED板
    ledBoardInit();
    
    // 初始化精确定时器
    timerInit();
    
    // 创建LED测试任务
    xTaskCreate(led_test_task, "led_test_task", 4096, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "All initialization done, LED test running");
    
    // 删除主任务
    vTaskDelete(NULL);
}