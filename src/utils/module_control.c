#include "module_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <termios.h>
#include <unistd.h>

int uart_init(const char *device_path) {
  int uart_fd = open(device_path, O_RDWR);
  if (uart_fd == -1) {
    perror("Failed to open device");
    return -1;
  }

  struct termios myserial;
  memset(&myserial, 0, sizeof(myserial));

  myserial.c_cflag |= (CLOCAL | CREAD);

  myserial.c_cflag &= ~CSIZE;  // 清空数据位
#ifdef CRTSCTS
  myserial.c_cflag &= ~CRTSCTS;  // 无硬件流控制
#else
  // If CRTSCTS is not defined, use the common value directly
  // CRTSCTS is typically defined as 020000000000 (octal)
  myserial.c_cflag &= ~020000000000;  // 无硬件流控制
#endif
  myserial.c_cflag |= CS8;  // 数据位:8

  myserial.c_cflag &= ~CSTOPB;  //   1位停止位
  myserial.c_cflag &= ~PARENB;  // 不要校验

  cfsetospeed(&myserial, B9600);  // 设置波特率
  cfsetispeed(&myserial, B9600);

  // 刷新输出队列,清除正接受的数据
  tcflush(uart_fd, TCIFLUSH);

  /* 改变配置 */
  tcsetattr(uart_fd, TCSANOW, &myserial);
  return uart_fd;
}

int set_mode_get_brightness(int device) {
  if (write(device, command_get_brightness, 3) == -1) {
    perror("Failed to write command");
    return -1;
  }
  sleep(1);  // Wait for the device to process the command
  return 0;
}

int get_illuminance(int device) {
  if (device == -1) {
    perror("Failed to open device");
    return -1;
  }

  uint8_t response[9] = {0};

  if (read(device, response, sizeof(response)) == 9) {
    // response[2] should be 0x15
    if (response[2] != 0x15) {
      fprintf(stderr, "Please set the mode to get brightness\n");
      return -1;
    }

    int brightness = response[4] << 24 | response[5] << 16 | response[6] << 8 | response[7];
    brightness /= 100;
    return brightness;
  } else {
    perror("Failed to read response");
    return -1;
  }
}

int set_mode_get_environment(int device) {
  if (write(device, command_get_environment, 3) == -1) {
    perror("Failed to write command");
    return -1;
  }
  sleep(1);  // Wait for the device to process the command
  return 0;
}

int get_temperature(int device) {
  if (device == -1) {
    perror("Failed to open device");
    return -1;
  }

  uint8_t response[15] = {0};

  if (read(device, response, sizeof(response)) == 15) {
    // response[2] should be 0x45
    if (response[2] != 0x45) {
      fprintf(stderr, "Please set the mode to get environment\n");
      return -1;
    }

    int temperature = response[4] << 8 | response[5];
    temperature /= 100;
    return temperature;
  } else {
    perror("Failed to read response");
    return -1;
  }
}

int get_atmospheric_pressure(int device) {
  if (device == -1) {
    perror("Failed to open device");
    return -1;
  }

  uint8_t response[15] = {0};

  if (read(device, response, sizeof(response)) == 15) {
    // response[2] should be 0x45
    if (response[2] != 0x45) {
      fprintf(stderr, "Please set the mode to get environment\n");
      return -1;
    }

    int atmospheric_pressure = response[6] << 24 | response[7] << 16 | response[8] << 8 | response[9];
    atmospheric_pressure /= 100;
    return atmospheric_pressure;
  } else {
    perror("Failed to read response");
    return -1;
  }
}

int get_humidity(int device) {
  if (device == -1) {
    perror("Failed to open device");
    return -1;
  }

  uint8_t response[15] = {0};

  if (read(device, response, sizeof(response)) == 15) {
    // response[2] should be 0x45
    if (response[2] != 0x45) {
      fprintf(stderr, "Please set the mode to get environment\n");
      return -1;
    }

    int humidity = response[10] << 8 | response[11];
    humidity /= 100;
    return humidity;
  } else {
    perror("Failed to read response");
    return -1;
  }
}

int get_altitude(int device) {
  if (device == -1) {
    perror("Failed to open device");
    return -1;
  }

  uint8_t response[15] = {0};

  if (read(device, response, sizeof(response)) == 15) {
    // response[2] should be 0x45
    if (response[2] != 0x45) {
      fprintf(stderr, "Please set the mode to get environment\n");
      return -1;
    }

    int altitude = response[12] << 8 | response[13];
    return altitude;
  } else {
    perror("Failed to read response");
    return -1;
  }
}

int get_smoke_concentration(int device) {
  if (device == -1) {
    perror("Failed to open device");
    return -1;
  }

  uint8_t response[9] = {0};

  write(device, command_get_smoke_concentration, 9);

  if (read(device, response, sizeof(response)) == 9) {
    int smoke_concentration = response[2] | response[3];
    return smoke_concentration;
  } else {
    perror("Failed to read response");
    return -1;
  }
}
