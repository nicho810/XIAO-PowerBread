# dialSwitch/
> L2 | 父级: lib/CLAUDE.md

## 成员清单

dialSwitch.h: DialFunction 类声明，旋钮状态枚举
dialSwitch.cpp: ADC 初始化 (10bit)，平台特定阈值判定 → 离散事件 (0=reset, 1=up, 2=down, 3=press)

## 设计要点

每个平台 ADC 特性不同，阈值硬编码于 `readDialStatus()` 内的 `#ifdef` 分支。

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
