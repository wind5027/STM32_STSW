ReadMe.txt for the ST STM32F429 Discovery start project
This project was built for the IAR ARM Compiler 6.30.1.
It has been tested with the following versions:
- V6.30.1

Supported hardware:
===================
The sample project was built for the ST STM32F4-Discovery eval board.
Using other hardware may require modification on the BSP.

Configurations
==============
- Debug_JLink
  This configuration is prepared for download into
  internal Flash using J-Link and CSpy.
  An embOS debug and profiling library is used.

- Release_JLink
  This configuration is prepared for download into
  internal Flash using J-Link and CSpy.
  An embOS release library is used.

- Debug
  This configuration is prepared for download into
  internal Flash using ST-Link and CSpy.
  An embOS debug and profiling library is used.

- Release
  This configuration is prepared for download into
  internal Flash using ST-Link and CSpy.
  An embOS release library is used.

Included middleware components (trial versions):
================================================
- emFile V3.32b
  - (2.00.01) emFile FAT (supporting FAT12, FAT16 and FAT32)
  - (2.10.10) emFile FAT LFN Module (support for Long File Name)

- emWin V5.22
  - (3.00.01) emWin BASIC color (complete graphic library)
  - (3.01.00) emWin WM/Widgets (Window manager and Widgets such as: Progress bar, Button, Frame window, Edit box, ...)
  - (3.01.02) emWin Memory Devices (Memory devices for flicker-free animation)
  - (3.01.03) emWin Antialiasing (Antialiasing smoothes curves and diagonal lines)
  - Drivers:
    - (3.10.02) emWin LCDLin (supported color depths: 1, 2, 4, 8, 15, 16, 32 bpp)

- embOS V3.88g
  - (1.23.01.01) embOS-Cortex-M-IAR (embOS designed for ARM Cortex-M and IAR compiler, single developer object license (SOL))

- emUSB-Host V1.12c
  - (9.30.00) emUSB Host 
  - (9.35.00) emUSB-Host Printer Class
  - Drivers:
    - (9.40.05) emUSB-Host STM32 HighPerformance Line Full Speed driver (emUSB Host Full Speed driver for STM32 HighPerformance Line (STM32F2/F4) host controllers)
