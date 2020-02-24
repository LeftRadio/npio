/**
  ******************************************************************************
  * @file      common.h
  * @author    leftradio
  * @version   v1.0.0
  * @date      2019
  * @brief     header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MODE_UNKNOWN -1
#define BOARD        10
#define BCM          11
#define MODE_RAW     12
#define SERIAL       40
#define SPI          41
#define I2C          42
#define PWM          43

/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern int gpio_mode;

extern const int pin_to_gpio_rev1[27];
extern const int pin_to_gpio_rev2[27];
extern const int physToGpioR3 [64];
extern const int physToGpio_neo [64];
extern const int physToGpio_m1 [64];
extern const int physToGpio_duo [64];
extern const int physToGpio_duo2 [64];

extern const int (*pin_to_gpio)[64];

int gpio_direction[64];
int revision;

int check_gpio_priv(void);
int get_gpio_number(int channel, unsigned int *gpio);
int setup_error;
int module_setup;

/* Exported function ---------------------------------------------------------*/


#endif /* __COMMON_H */
/*******************************************************************************
      END FILE
*******************************************************************************/