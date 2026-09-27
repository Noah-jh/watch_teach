# Demo 13：Bootloader / OTA 学习笔记

## 目标
对应 Bootloader 初探、OTA、下载算法和项目集成章节。该 demo 先作为独立阅读与安全验证工程，不建议一开始覆盖现有程序。

## 必须掌握
- Bootloader 与 APP 的 Flash 地址分区。
- APP 的向量表重定位。
- 镜像长度、版本、CRC/Hash 校验。
- 下载失败回滚和断电保护。
- J-Link 烧录地址与 Keil target 配置必须一致。

## 安全验证顺序
1. 先用 J-Link 恢复整片/下载已知可运行程序。
2. 在 RAM 或独立测试芯片上验证跳转。
3. 再验证固定地址 APP。
4. 最后才接入串口/YMODEM/无线升级。

## 验收
- Bootloader 能识别有效 APP。
- 无效镜像不会跳转。
- APP 能正确运行。
- 断电后仍能恢复到有效镜像。

## 注意
Bootloader 地址、链接脚本、向量表偏移和中断重定位必须成套修改；不要只改一个宏后直接烧录。
