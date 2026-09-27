# Demo 07 README (HC-SR04)

Purpose
- Measure distance using HC-SR04 (TRIG/ECHO) with safe level shifting

CubeMX
- GPIO for TRIG output
- TIMx Input Capture for ECHO (or EXTI + timed micros)
- USART1 for debug

Wiring
- TRIG -> PB10 (or chosen GPIO)
- ECHO -> level shifter -> TIM input (e.g., PB3)
- VCC -> 5V
- GND -> GND

Safety
- Use resistor divider or MOSFET-based level shifter for ECHO

Validation
- Print measured distance to serial
