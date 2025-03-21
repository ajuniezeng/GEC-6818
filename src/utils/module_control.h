#pragma once

#include <stdint.h>
#define CON2_PATH "/dev/ttySAC1"
#define CON3_PATH "/dev/ttySAC2"
#define CON4_PATH "/dev/ttySAC3"
#define CON5_PATH "/dev/ttySAC4"

const uint8_t command_get_brightness[] = {0xA5, 0x81, 0x26};
const uint8_t command_get_environment[] = {0xA5, 0x82, 0x27};
const uint8_t command_get_smoke_concentration[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

int uart_init(const char *device_path);
int set_mode_get_brightness(int device);
int set_mode_get_environment(int device);

int get_illuminance(int device);
int get_temperature(int device);
int get_atmospheric_pressure(int device);
int get_humidity(int device);
int get_altitude(int device);
