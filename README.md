# LED测试项目

这是一个ESP32 LED测试项目，用于测试GPIO控制LED灯的基本功能。

## 项目结构

```
LED_TEST/
├── CMakeLists.txt          # 主项目CMake文件
├── main/
│   ├── CMakeLists.txt      # main组件CMake文件
│   └── main.c              # 主程序源码
├── build.bat               # 编译脚本
├── install_esp_idf.bat     # ESP-IDF安装脚本
└── README.md               # 项目说明文档
```

## 功能说明

- 简单的LED闪烁测试
- 使用GPIO2控制LED
- 1秒间隔的开关循环
- ESP-IDF框架开发

## 编译和运行

### 前提条件

1. **安装ESP-IDF**
   - 运行 `install_esp_idf.bat` 获取安装指导
   - 或访问ESP-IDF官方文档
   - 推荐安装ESP-IDF v5.0或更高版本

### 编译步骤

1. **自动编译**（推荐）
   ```bash
   build.bat
   ```

2. **手动编译**
   ```bash
   # 设置ESP-IDF环境
   C:\Espressif\frameworks\esp-idf-v5.0\export.bat
   
   # 编译项目
   idf.py build
   ```

### 烧录到设备

```bash
# 烧录并监控（替换COM3为实际串口号）
idf.py -p COM3 flash monitor
```

## 硬件连接

- **LED正极** → GPIO2
- **LED负极** → GND（通过适当的限流电阻）

## 故障排除

### 编译错误：CMakeLists.txt BOM问题
- **问题**：`File starts with a Byte-Order-Mark that is not UTF-8`
- **解决**：已修复，所有文件使用UTF-8无BOM编码

### ESP-IDF未找到
- **问题**：`无法将"idf.py"项识别为cmdlet`
- **解决**：先安装ESP-IDF，运行`install_esp_idf.bat`获取安装指导

### 串口权限问题
- **问题**：无法连接到设备
- **解决**：确保ESP32设备已连接，检查设备管理器中的COM端口号

## 代码说明

### main.c 主要功能

```c
// GPIO配置
gpio_config_t io_conf = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << LED_GPIO),
    .pull_down_en = 0,
    .pull_up_en = 0,
};

// LED控制循环
while (1) {
    gpio_set_level(LED_GPIO, 1);  // LED开
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(LED_GPIO, 0);  // LED关
    vTaskDelay(pdMS_TO_TICKS(1000));
}
```

## 扩展功能建议

- 添加多个LED控制
- 实现PWM调光功能
- 添加按键交互
- 集成WiFi功能
- 实现LED灯条控制

## 许可证

本项目遵循MIT许可证。