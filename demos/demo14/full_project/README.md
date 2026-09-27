# Demo14 full_project README

Purpose
- Demonstrate EXTI-based button handling with short/long/very-long press detection

Usage
1. CubeMX: enable PA0 as GPIO_EXTI0, enable USART1 for logs, enable FreeRTOS CMSIS v2. Generate code.
2. Copy this main.c into Src/ and add to project.
3. Build and flash.
4. Expected serial outputs when pressing the user button:
   - short press: prints "short press"
   - long press (>1s): prints "long press"
   - very long press (>2s): prints "very long press"

Notes
- Ensure button wiring matches active level used in code (this code assumes active low).
