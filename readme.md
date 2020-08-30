# 便携式USB直流稳压电源

## 参数简介

1. 电压输出范围：0V~10V
2. 最大电流500mA
3. 显示精度：±0.01V

## 备注

​	（1）需要更大电流输出的，可更换更大体积的电感，把USB修改为Type-C接口，续流二极管D4和保险丝F1也需要根据实际情况调整，需要注意芯片layout时散热处理；

​	（2）需要更大电压输出的，可以修改R15和R19的比例，最大输出28V

​	（3）需要采用顶针进行下载的，可以增加下载相关TP点

​	（4）需要降低静态功耗的，可以修改R13、R14、R16及R18等电阻参数

