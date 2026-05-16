# sysConfig/
> L2 | 父级: lib/CLAUDE.md

## 成员清单

sysConfig.h: SysConfig 类 (EEPROM 读写)，ConfigMode 类 (状态机)，sysConfig_data 结构体 (11项参数)，ConfigIndex 枚举
sysConfig.cpp: EEPROM 128字节持久化，版本迁移 (cfg_version=2)，参数增减逻辑 (带边界检查)，configStateMutex 信号量保护
sysConfig_user.h: 用户可调默认值定义

## 配置参数 (11项)

default_mode, default_channel, shuntResistorCHA/CHB, serial_enable, serial_baudRate, serial_mode, serial_printInterval, chart_updateInterval, chart_scaleMode, chart_scale

## ConfigMode 状态机

```
enterConfigMode() → isActive=true, cursor=0
  ↕ updateConfigCursor() / increment/decrementConfigValue()
exitConfigMode() → isActive=false, cursorStatus=-1
```

线程安全: configStateMutex 保护 ConfigModeState 的读写。

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
