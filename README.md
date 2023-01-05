## board-stm32f103rc-berial

基于 stm32f103rct6 的开发板

---

例程包含：

* 假货宝上的常用模块例程
* ARM 数学库中的 DSP 例程
* CMSIS-NN 神经网络例程



|                   |  | desc | status |
| ----------------- | ------------- | ----------------------------------- | ------ |
| **GPIO**          |               |                                     | - |
| | ds1302                           | 实时时钟                            | √      |
|| dht11                            | 湿度传感器                          | √      |
| | ds18b20                          | 温度传感器                          | √      |
| | hx71x                            | 24 位 ADC | √      |
|  |matrixkey                       | 矩阵键盘                            | √      |
| **UART** |               |                                     | - |
|  | esp01 | wifi 模块 | |
|  | sim800 | | |
| **TIM**           |               | 定时器                              | - |
| | delayus                          | 微秒级延时                          | √      |
| | ws2812                           | 彩灯（PWM驱动） | √      |
|  |hcsr04                           | 超声波传感器（输入捕获）            | √      |
|  |encoder                          | 旋转编码器（定时器模式&编码器模式） | √      |
| | hx1828                           | 红外接收（输入捕获）                      | √      |
| **WDG**           |               | 窗口看门狗                          | √ |
| **ADC**           |               | 模拟量输入                          | - |
|  | irq | 单通道中断采集 | √ |
|  | dma | 单通道/多通道DMA采集 | √ |
|  |temt6000                         | 光照传感器                          | √      |
|  |s12d                             | 紫外光传感器                        | √      |
|  |pulse                           | 脉搏传感器                          | √      |
|  |sound                            | 声音传感器                          | √      |
| **DAC**           |               | 模拟量输出                          | - |
|                   | triangle wave | 三角波生成                          | √      |
|  |sine wave                        | 正弦波生成                          | √      |
| | heart-shaped wave                | 心形波生成                          | √      |
| | xy-mode | 示波器播放动画 |  |
| **I2C**           |               |                                     | - |
|                   | i2cbus | 模拟 I2C | √ |
| | lis3dh | 3轴加速度传感器 | √ |
|                | mpu6050（euler）   | 加速度+陀螺仪，欧拉角输出            | √      |
|                | mpu6050（kalman）  | 6轴，欧拉角+卡尔曼滤波输出 | √      |
|                | mpu6050（dmp）     | 6轴，DMP输出               | √      |
|                | mpuxxxx            | 6轴（加速度+陀螺仪）                 | √      |
|                |                    | 9轴（加速度+陀螺仪+地磁计）          | √      |
|  | mpu9250（dmp） | 9轴，DMP输出 |  |
|                | bmp280             | 大气压传感器                         | √      |
|                | bme280             | 温湿度大气压传感器                   |        |
|                | aht10              | 温湿度传感器                         |        |
|                | aht20              | 温湿度传感器                         |        |
|                | sht30              | 温湿度传感器 | √ |
|                | as5600             | 磁编码器 | √ |
|                | hmc5883l           | 地磁传感器 | √ |
|                | xmc5883l           | 地磁传感器（HMC\QMC\VMC） | √ |
|                | pcf8574            | IO 扩展 | √ |
|                | pcf8574（lcd1602） | 液晶屏 | √ |
|                | pcf8575            | IO 扩展 |  |
|                | axp173             | 电源管理 IC | √ |
|                | axp192             | 电源管理 IC |  |
|                | axp209             | 电源管理 IC |  |
|                | at24cxx            | eeprom | √ |
|                | ssd1306 | OLED |  |
| | ssd1306（animation） | 太空人 / IKUN  动画 | √ |
| | ssd1306（video） | 串口数据流播放视频 | √ |
| | ssd1306（u8g2） | 移植 u8g2 图形库 | √ |
|                | max30100 | 心率血氧传感器 | √ |
|                | max30102 | 心率血氧接近传感器 | √ |
| | max30105 |  | |
| | pn532 |  | |
| | tca9548 | | |
| | ina3221 | | |
| | adxl345 | | |
| | ad7705 | 16位ADC | |
| | ap3216 | | |
| | si5351 | 波形发生器 | |
| | tcs3472 | 颜色传感器 | √ |
| | lm75 | 温度传感器 | √ |
| | pca9685 | 多路PWM舵机控制器 | √ |
| | mlx90614 | 红外测温传感器 | √ |
| **SPI**           |               |                                     | - |
|  | spibus | 模拟 SPI | |
|                   | st7735_0.96 | 板载 0.96 寸屏幕                    | √      |
| | st7735_luat | 合宙 1.8 寸屏幕 | √ |
|                   | mfrc522       | RFID                                | √      |
|                   | w25qxx        | FLASH                               | √      |
|                   | ad9833        | 波形发生器 | √      |
| | ssd1306 | OLED |  |
| | pn532 |  |  |
| | w5500                | 以太网                              |        |
|                | lan8720              | 以太网                              |        |
|                | nrf24l01             | 2.4传输                             |        |
|                | mcp2551              | SPI转CAN                            |        |
|                | st7789               | 屏幕                                |        |
|                | ws2812               | 彩灯                                |  |
| |  |  |  |
| **I2S**           |               |                                     | - |
|  | i2sbus | 模拟 I2S |  |
|                   | inmp441       | 麦克风                              | √      |
| | mp34dt05 | 麦克风 |  |
| | max98357 | 喇叭 |  |
| **CAN** |  |  | - |
|  | tj1050 |  |  |
|  | vp232 |  |  |
| **FAFTS**         |               | 文件系统                            | - |
|                   | sd card（spi） | TF 卡（spi 接口）                   | √      |
|                   | sd card（sdio） | TF 卡（sdio 接口）                  | √      |
|                   | w25qxx（spi） | 外部 flash（spi 接口）              | √      |
| **USB**           |               |                                     | - |
| MSC               |               | 大容量设备（U盘）                   |        |
|                   | sram          | 内部 SRAM 模拟 U 盘                 | √      |
|                   | w25qxx        | 外部 Flash 模拟 U 盘                | √      |
|                   | rom           | 片上 Flash 模拟 U 盘                |        |
| CDC               |               | 虚拟串口                            | √      |
| HID               |               | 人体学接口设备 |        |
|                   | keypress      | 模拟键盘按键 | √      |
| **RTOS**          |               | 实时操作系统                          | - |
| UCOS-II | |  | |
| UCOS-III | |  | |
| FreeRTOS          |               |                                     |       |
|  | task | 任务 | √ |
| RT-Thread Nano    |               | RTT OS                              | - |
|  | start | 移植 | √ |
| TencentOS tiny    |               | 腾讯 OS                             | - |
|  | start | 移植 | √ |
|                   | task | 任务 | √ |
| | mutex | 互斥量 | √ |
| | sem | 信号量 |  |
| HUAWEI LiteOS |  | 华为 OS |  |
| **Language**      |               |                                     | - |
| Lua               |               |                                     |        |
|  | start | 移植 | √ |

```
DSP&NN ARM数学库
├─Sin 基本测试
├─Matrix 矩阵运算
├─Conv 卷积
├─FFT 傅里叶变换
├─SVM 支持向量机
├─Gaussian Naive Bayes 高斯贝叶斯分类
├─Biquad Cascade Filter 双二阶级联滤波器
├─Interpolation 插值
├─PID PID控制
├─Kalman Filter 卡尔曼滤波
├─Adaptive Filter 自适应滤波
├─DCT 离散傅里叶变换
├─Clarke 克拉克变化
├─QLearn 强化学习
├─Genetic Algorithm 遗传算法
├─Inductive Learning 归纳学习
├─Ant Colony Optimization 蚁群算法
├─Linear Regression 线性回归
├─Convolutional Neural Network 卷积神经网络CNN
└─Recurrent Neural Network 循环神经网络RNN
```

* 电机控制：FOC、BLDC、SVPWM
* GUI：LVGL，emWin，GuiLite，Pico3D
* 数据结构：链表、队列、堆、栈、哈希表、二分法

