# J-Link、Keil 调试与 Stack Trace

## 1. J-Link SWD 接线

| J-Link | STM32F411 |
|---|---|
| VTref | 目标板 3.3V |
| GND | GND |
| SWDIO | PA13 |
| SWCLK | PA14 |
| nRESET | NRST |

VTref 只用于检测目标电平；是否由 J-Link 给目标板供电，应按你的 J-Link 型号和最小系统板说明决定。不要同时连接多个不受控电源。

## 2. Keil 设置

1. 打开 CubeMX 生成的 `.uvprojx`。
2. `Options for Target` -> `Debug` -> 选择 `J-LINK/J-TRACE Cortex`。
3. 点击 `Settings`，确认识别到 STM32F411。
4. Interface 选择 SWD，速度初始设置 1 MHz；稳定后再提高。
5. `Utilities` 中选择 J-Link，并勾选编程后复位运行。
6. 先 Build，再 Download，最后 Reset/Run。

## 3. 调用栈定位流程

1. 在疑似入口函数设置断点。
2. 进入 `Debug` 模式，打开 Call Stack 窗口。
3. 发生 HardFault 时暂停 CPU。
4. 查看 `PC`、`LR`、当前栈帧和调用路径。
5. 对照 `.map` 文件和反汇编定位函数。
6. 重点检查：空指针、栈溢出、数组越界、ISR 调用阻塞函数、DMA 缓冲区越界。

## 4. HardFault 记录原则

调试阶段不要在 HardFault 中调用复杂 printf。优先保存寄存器：
- R0-R3
- R12
- LR
- PC
- xPSR

之后使用 Keil 的地址跳转和 map 文件定位。若使用 RTOS，另外检查发生故障的任务和任务栈余量。

## 5. Ozone/SystemView

- Ozone：单步、寄存器、调用栈、内存和性能分析。
- SystemView：任务切换、ISR、信号量和队列时间线。
- 先让 Demo01 稳定运行，再接入追踪；不要在硬件尚未稳定时同时引入复杂 trace 配置。

## 6. 下载失败排查

- 目标板是否有稳定 3.3V。
- VTref/GND/SWDIO/SWCLK 是否接对。
- SWDIO 与 SWCLK 是否被外设强拉。
- NRST 是否被错误拉低。
- 降低 SWD 速度。
- 在复位下连接。
- 检查芯片读保护状态。
