# STRIX
### Satellite True Random Information eXtractor

A portable wireless True Random Number Generator based on [neoTRNG](https://github.com/stnolting/neoTRNG) relized with an Arduino MKR Vidor 4000.

## Set Up Dev Environment

Windows:

1. Install Arduino IDE from here or via chocolatey: `choco install arduino`
2. Install [Intel Quartus Lite](https://www.intel.com/content/www/us/en/software-kit/825278/intel-quartus-prime-lite-edition-design-software-version-23-1-1-for-windows.html). In the list of devices select `Cyclone 10 LP device support`

## FPGA Development

- In Quartus > Open Project > select `strix.qpf` from the `fpga/src` directory in this repo.

## TODO

- Port neoTRNG to the MKR Vidor 4000 FPGA
- Fetch random numbers from the FPGA
- Serve random numbers via USB
- Make Vidor pairable via Bluetooth/Wifi
- serve randomness wirelessly
- battery pack
- 3D case

---

## [Development Notes](NOTES.md)