# INA3221Sensor/
> L2 | 父级: lib/CLAUDE.md

## 成员清单

INA3221Sensor.h: INAData/DualChannelData 数据结构定义，INA3221Sensor 类声明，isDirty 脏标志机制
INA3221Sensor.cpp: I2C 初始化 (平台选 Wire/Wire1)，双通道采集，脏标志计算 (5mV/5mA 阈值)

## 核心数据结构

```cpp
struct INAData { busVoltage, busCurrent, busPower, shuntVoltage, shuntResistor, isDirty };
struct DualChannelData { INAData channel0, channel1; };
```

## 关键接口

- `begin()` → 初始化 I2C + INA3221
- `readCurrentSensors()` → 返回 DualChannelData，isDirty 驱动选择性 UI 刷新
- `setParameter(shuntA, shuntB)` → 运行时配置分流电阻

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
