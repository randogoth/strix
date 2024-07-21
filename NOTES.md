# IDE Setup (Windows)

- Installed Intel Quartus Lite from https://www.intel.com/content/www/us/en/software-kit/825278/intel-quartus-prime-lite-edition-design-software-version-23-1-1-for-windows.html
- Installed Arduino IDE 2 `choco install arduino`

# Project Setup

based on https://docs.arduino.cc/tutorials/mkr-vidor-4000/vidor-quartus-vhdl/

- New Project Wizard
    - 'strix' for name and top level design entity
    - Skip next screen
    - Family, Device, Board Settings > Name filter: `10CL016YU256C8G`
    - Finish

- Import Pin Settings:
    - Menu > Assignments > Import Assignments > vidor_s_pins.qsf 
        - from https://github.com/vidor-libraries/VidorFPGA/blob/master/constraints/MKRVIDOR4000/vidor_s_pins.qsf 