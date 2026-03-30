/**
 * @file       main.c
 * @copyright  Copyright (C) 2025 NEO. All rights reserved.
 * @license    This project is released under the NEO License.
 * @version    1.0.0
 * @date       2026-03
 * @author     Thach Nguyen Ba Vu
 * @brief      Main file
 * @note       None
 */
/* Includes ----------------------------------------------------------- */
#include "stdio.h"
#include "I2c.h"
/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
I2C_ConfigType i2c_config = {
    .i2c_port = 0,
    .sda_pin = 12,
    .scl_pin = 13,
};
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
void app_main(void)
{
    // Initialize BSP and libraries
    I2C_Init(&i2c_config);
    printf("No error!\n");
    printf("Hello, World!\n");
}
/* End of file -------------------------------------------------------- */
