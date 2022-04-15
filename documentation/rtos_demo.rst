=========
RTOS Demo
=========

This application example demonstrates basic use of the
<a href="http://www.freertos.org">FreeRTOS</a> real-time operating system.
More specifically, it demonstrates use of tasks queues.

After start-up, the application blinks the LED continuously.
By inputting the character from the EDBG Virtual COM Port the LED changes its
blinking state.

Input between:

* "a": LED blink task **active**
* "s": LED OFF and blink task **suspend**

Supported evaluation kit
------------------------
- SAMC21 Xplained Pro
- SAMD10 Xplained Pro
- SAMD11 Xplained Pro
- SAMD20 Xplained Pro
- SAMD21 Xplained Pro
- SAMD51 Xplained Pro
- SAMDA1 Xplained Pro
- SAME54 Xplained Pro
- SAME70 Xplained Pro
- SAMG53 Xplained Pro
- SAMG55 Xplained Pro
- SAML21 Xplained Pro
- SAML22 Xplained Pro
- SAMR21 Xplained Pro
- SAMR30 Xplained Pro
- SAMV71 Xplained Ultra

Drivers
-------
* RTOS USART
* GPIO

Default interface settings
--------------------------
* USART

  * No parity
  * 8-bit character size
  * 1 stop bit
  * 9600 baud-rate
