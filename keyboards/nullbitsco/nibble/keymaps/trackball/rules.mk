OLED_DRIVER_ENABLE = yes
WPM_ENABLE = yes
VIA_ENABLE = yes
NKRO_ENABLE = yes
QUANTUM_LIB_SRC += i2c_master.c
USE_OLED_BITMAP_COMPRESSION = yes
CONSOLE_ENABLE = no
SRC += features/pimoroni_trackball.c
POINTING_DEVICE_ENABLE = yes
