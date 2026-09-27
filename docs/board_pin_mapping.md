# docs/board_pin_mapping.md

## Updates from user confirmation
- Onboard LED: present (as documented earlier on PC13)
- Onboard button: PA0 (the user confirmed PA0 is the board button)

Notes
- PC13 remains tied to RTC/LED functions on some boards; user specifically confirmed PA0 is the onboard button, so demos that use a user button will use PA0 (ADC0 pin) only if code configures it as EXTI input. Be mindful: PA0 is also ADC0 and only 3.3V tolerant.

Safety reminder
- PA0 and PB5 only support 3.3V. Do not connect 5V signals to these pins.

