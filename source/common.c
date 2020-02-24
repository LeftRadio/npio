/**
  *****************************************************************************
  * @file      common.c
  * @author    leftradio
  * @version   v1.0.0
  * @date      2019
  * @brief     sourse for common
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include "Python.h"
#include "c_gpio.h"
#include "common.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
#define MAX_PIN_COUNT                     ((uint8_t)64)

/* Private macro ------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
int gpio_mode = MODE_UNKNOWN;

const int pin_to_gpio_rev1[27];
const int pin_to_gpio_rev2[27];
const int physToGpioR3 [64];

/* NanoPi NEO/NEO2 */
const int physToGpio_neo [MAX_PIN_COUNT] = {
    -1,
    /* 24 Pin */
    -1, -1,   //1, 2
    12, -1,   //3, 4
    11, -1,   //5, 6
    203, 198, //7, 8
    -1,  199, //9, 10
    0,   6,   //11, 12
    2,   -1,  //13, 14
    3,  200,  //15, 16
    -1, 201,  //17, 18
    64, -1,   //19, 20
    65, 1,    //21, 22
    66, 67,   //23, 24

    /* 12 Pin */
    -1, -1,   //25, 26
    -1, -1,   //27, 28
    -1, -1,   //29, 30
    -1, -1,   //31, 32
    -1, -1,   //33, 34
    -1, -1,   //35, 36

    /* UART0 Tx,Rx */
    4, 5,     //37, 38

    /* 39~63 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
/* */
const int physToGpio_m1 [MAX_PIN_COUNT] = {
    -1, // 0
    -1, -1, // 1, 2
    12, -1, // 3, 4
    11, -1, // 5, 6
    203, 198, // 7, 8
    -1, 199, // 9, 10
    0, 6, //11, 12
    2, -1, //13, 14
    3, 200, //15, 16
    -1, 201, //17, 18
    64, -1, //19, 20
    65, 1, //21, 22
    66, 67, //23, 24
    -1, 17, //25, 26
    19, 18, //27, 28
    20, -1, //29, 30
    21, 7, //31, 32
    8, -1, //33, 34
    16, 13, //35, 36
    9, 15, //37, 38
    -1, 14, //39, 40
    -1, -1, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
    -1, -1, -1, -1, -1, -1, -1, -1 // 56-> 63
};
/* phys pin number to /sys/gpio number */
const int physToGpio_duo [MAX_PIN_COUNT] = {
   -1,        //0
 /* 32 Pin */
   -1, -1,   //1, 2
   -1, -1,   //3, 4
   -1, -1,   //5, 6
   -1, -1,   //7, 8
   -1, -1,   //9, 10
   198,-1,   //11, 12
   199,-1,   //13, 14
   15, -1,   //15, 16
   16, -1,   //17, 18
   14, -1,   //19, 20
   13, 203,  //21, 22
   12, -1,  //23, 24
   11, -1,   //25, 26
   -1, -1,   //27, 28
   4,  -1,   //29, 30
   5,  -1,   //31, 32
   /* ---------nanopi duo end----------- */

    -1, -1,   //33, 34
    -1, -1,   //35, 36

    /* UART0 Tx,Rx */
    -1, -1,     //37, 38

    /* 39~63 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
// phys pin number to /sys/gpio number
const int physToGpio_duo2 [MAX_PIN_COUNT] ={
    -1,        // 0
    /* 32 Pin */
    -1,   5,  //  1, 2
    -1,   4,  //  3, 4
    -1,  -1,  //  5, 6
    -1,  11,  //  7, 8
    363, 12,  //  9, 10
    203, 13,  // 11, 12
    -1,  14,  // 13, 14
    -1,  16,  // 15, 16
    -1,  15,  // 17, 18
    -1, 199,  // 19, 20
    -1, 198,  // 21, 22
    -1,  -1,  // 23, 24
    -1,  -1,  // 25, 26
    -1,  -1,  // 27, 28
    -1,  -1,  // 29, 30
    -1,  -1,  // 31, 32
    /* ---------nanopi duo end----------- */

     -1, -1,   // 33, 34
     -1, -1,   // 35, 36

     /* UART0 Tx,Rx */
     -1, -1,   // 37, 38

     /* 39~63 */
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

const int (*pin_to_gpio)[64];

/* */
int setup_error = 0;
int module_setup = 0;
int revision = -1;
/* */
extern int nanopi_found;

/* Extern function ----------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/

/**
  * @brief
  * @param  None
  * @retval None
  */
int check_gpio_priv(void)
{
    // check module has been imported cleanly
    if (setup_error)
    {
        PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
        return 1;
    }

    // check mmap setup has worked
    if (!module_setup)
    {
        PyErr_SetString(PyExc_RuntimeError, "No access to /dev/mem.  Try running as root!");
        return 2;
    }
    return 0;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
int is_valid_raw_port(int channel)
{
    if (channel >= 0 && channel < 18) return 1; // PA
    if (channel >= 32 && channel < 56) return 2; // PB
    if (channel >= 64 && channel < 89) return 3; // PC
    if (channel >= 96 && channel < 124) return 4; // PD
    if (channel >= 128 && channel < 140) return 5; // PE
    if (channel >= 160 && channel < 166) return 6; // PF
    if (channel >= 192 && channel < 204) return 7; // PG
    if (channel >= 224 && channel < 252) return 8; // PH
    if (channel >= 256 && channel < 278) return 9; // PI
    return 0;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
int get_gpio_number(int channel, unsigned int *gpio)
{
    // check channel number is in range
    if ((gpio_mode == BCM && (channel < 0 || channel > 64))
        || (gpio_mode == BOARD && (channel < 1 || channel > 64)))
    {
        PyErr_SetString(PyExc_ValueError, "The channel sent is invalid on a NanoPi [BOARD or RAW]");
        return 4;
    }

    // convert channel to gpio
    if (gpio_mode == BOARD)
    {
        if (*(*pin_to_gpio + channel) == -1)
        {
            PyErr_SetString(PyExc_ValueError, "The channel sent is invalid on a NanoPi [BOARD]");
            return 5;
        }
        else {
            *gpio = *(*pin_to_gpio + channel);  //pin_to_gpio is initialized in py_gpio.c, the last several lines
        }
    }
    // else if (gpio_mode == BCM)
    // {
    //      *gpio = *(pinTobcm_BP + channel);
    // }
    else if (gpio_mode == MODE_RAW)
    {
        if (!is_valid_raw_port(channel))
        {
            PyErr_SetString(PyExc_ValueError, "The channel sent does not exist [RAW]");
            return 5;
        }
        *gpio = channel;
    }
    else {
        // setmode() has not been run
        PyErr_SetString(PyExc_RuntimeError, "Please set pin numbering mode using npio.setmode(mode) method with mode [npio.BOARD or npio.RAW]");
        return 3;
    }

    #if NP_DEBUG
        printf("get GPIO = %d\n", *gpio);
    #endif

    return 0;
}


/******************************************************************************
      END FILE
******************************************************************************/