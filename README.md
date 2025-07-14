This repository documents my progress with the [Blinky To Bootloader series on YouTube](https://www.youtube.com/playlist?list=PLP29wDx6QmW7HaCrRydOnxcy8QmW0SNdQ)

There is a separate [GitHub repo](https://github.com/lowbyteproductions/bare-metal-series) provided by the creator, but I am not using it as I want to structure my codebase differently.


I am using an [STM32 Smart V2.0](https://stm32-base.org/boards/STM32F103C8T6-STM32-Smart-V2.0) board with a JLink debugger for this course.

A (quick) rundown of the specs of the microcontroller:
- Arm® 32-bit [Cortex®-M3](https://developer.arm.com/documentation/ddi0337/h/?lang=en) processor (72MHz max)
    -  ARMv7-M architecture
- 64 Kbytes of flash memory
- 20 Kbytes of SRAM

I have included the datasheet for the STM32F103, the STM32F103 programmer reference manual, as well as the  Cortex®-M3 processor technical reference manual.

The C standard I will be using in my code is [C23](https://en.cppreference.com/w/c/23.html). If you find some of the keywords/syntax confusing, please brush up on C23 before following along.