# LED������Ŀ

����һ��ESP32 LED������Ŀ�����ڲ���GPIO����LED�ƵĻ������ܡ�

## ��Ŀ�ṹ

```
LED_TEST/
������ CMakeLists.txt          # ����ĿCMake�ļ�
������ main/
��   ������ CMakeLists.txt      # main���CMake�ļ�
��   ������ main.c              # ������Դ��
������ build.bat               # ����ű�
������ install_esp_idf.bat     # ESP-IDF��װ�ű�
������ README.md               # ��Ŀ˵���ĵ�
```

## ����˵��

- �򵥵�LED��˸����
- ʹ��GPIO2����LED
- 1�����Ŀ���ѭ��
- ESP-IDF��ܿ���

## ���������

### ǰ������

1. **��װESP-IDF**
   - ���� `install_esp_idf.bat` ��ȡ��װָ��
   - �����ESP-IDF�ٷ��ĵ�
   - �Ƽ���װESP-IDF v5.0����߰汾

### ���벽��

1. **�Զ�����**���Ƽ���
   ```bash
   build.bat
   ```

2. **�ֶ�����**
   ```bash
   # ����ESP-IDF����
   C:\Espressif\frameworks\esp-idf-v5.0\export.bat
   
   # ������Ŀ
   idf.py build
   ```

### ��¼���豸

```bash
# ��¼����أ��滻COM3Ϊʵ�ʴ��ںţ�
idf.py -p COM3 flash monitor
```

## Ӳ������

- **LED����** �� GPIO2
- **LED����** �� GND��ͨ���ʵ����������裩

## �����ų�

### �������CMakeLists.txt BOM����
- **����**��`File starts with a Byte-Order-Mark that is not UTF-8`
- **���**�����޸��������ļ�ʹ��UTF-8��BOM����

### ESP-IDFδ�ҵ�
- **����**��`�޷���"idf.py"��ʶ��Ϊcmdlet`
- **���**���Ȱ�װESP-IDF������`install_esp_idf.bat`��ȡ��װָ��

### ����Ȩ������
- **����**���޷����ӵ��豸
- **���**��ȷ��ESP32�豸�����ӣ�����豸�������е�COM�˿ں�

## ����˵��

### main.c ��Ҫ����

```c
// GPIO����
gpio_config_t io_conf = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << LED_GPIO),
    .pull_down_en = 0,
    .pull_up_en = 0,
};

// LED����ѭ��
while (1) {
    gpio_set_level(LED_GPIO, 1);  // LED��
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(LED_GPIO, 0);  // LED��
    vTaskDelay(pdMS_TO_TICKS(1000));
}
```

## ��չ���ܽ���

- ��Ӷ��LED����
- ʵ��PWM���⹦��
- ��Ӱ�������
- ����WiFi����
- ʵ��LED��������

## ���֤

����Ŀ��ѭMIT���֤��