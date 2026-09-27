# Demo 09 README (Servo & Fan)

Purpose
- Demonstrate servo control using PWM and fan control with MOSFET

CubeMX
- TIM1 for PWM (PA8 recommended)
- GPIO for MOSFET gate (control fan)
- USART1 debug

Wiring
- Servo Vcc -> 5V independent supply
- Servo GND -> common GND
- Servo signal -> PA8 (PWM)
- Fan + -> 5V, Fan - -> MOSFET drain, MOSFET source -> GND, Gate -> MCU GPIO

Validation
- Servo sweeps 0..180 deg
- Fan speed responds to PWM duty

Notes
- Use decoupling and sufficient current supply for servo
