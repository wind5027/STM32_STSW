@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM 


"D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\common\bin\cspybat" "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\arm\bin\armproc.dll" "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\arm\bin\armstlink.dll"  %1 --plugin "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\arm\bin\armbat.dll" --flash_loader "G:\STM32F429\hg\Start\Setup\Flashloader\FlashSTM32F4xxI.board" --backend -B "--endian=little" "--cpu=Cortex-M4F" "--fpu=VFPv4" "-p" "G:\STM32F429\hg\Start\Setup\DeviceSupport\STM32F429ZI.ddf" "--drv_verify_download" "--semihosting" "--stlink_interface=SWD" "--stlink_reset_strategy=0,0" "--drv_swo_clock_setup=168000000,0,2000000" 


