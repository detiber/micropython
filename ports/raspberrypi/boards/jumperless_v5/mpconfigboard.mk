USB_VID = 0x1D50
USB_PID = 0xACAB
USB_PRODUCT = "Jumperless V5"
USB_MANUFACTURER = "Architeuthis Flux"

CHIP_VARIANT = RP2350
CHIP_PACKAGE = B
CHIP_FAMILY = rp2

# Not sure which of these two it should be... Difference
# Differences are:
# memory_type = 0x40 for xQ vs 0x70 for xM
# write_status_register_split = false for xQ vs true for xM
# 01_continuous_status_write undefined for xQ vs being set to true for xM
EXTERNAL_FLASH_DEVICES = "W25Q128JVxQ,W25Q128JVxM"
