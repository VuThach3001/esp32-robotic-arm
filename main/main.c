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
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "I2c.h"
/* Private defines ---------------------------------------------------- */
#define I2C_SERVICE_PERIOD_MS    5u
#define DISPLAY_UPDATE_PERIOD_MS 500u

#define SAFE_MS_TO_TICKS(ms_) ((pdMS_TO_TICKS(ms_) > 0u) ? pdMS_TO_TICKS(ms_) : 1u)

#define I2C_SEQ_SSD1306          0u
#define SSD1306_I2C_ADDR         0x3Cu
#define SSD1306_WIDTH            128u
#define SSD1306_PAGE_BYTES       SSD1306_WIDTH
#define SSD1306_DATA_PKT_LEN     (SSD1306_PAGE_BYTES + 1u)

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
static I2C_ConfigType i2c_config = {
    .hwUnit          = 0u,
    .baudRate        = 400000u,
    .asyncMode       = I2C_POLLING_MODE,
    .i2c_port        = 0u,
    .scl_pin         = 13u,
    .sda_pin         = 12u,
    .hwUnitMode      = I2C_HW_UNIT_MODE_CONTROLLER,
    .targetListening = FALSE,
    .deviceAddress   = 0x28u,
};
/* Private variables -------------------------------------------------- */
static SemaphoreHandle_t            s_i2cMutex      = NULL;
static volatile boolean             s_ssd1306Done   = FALSE;
static volatile I2C_SequenceResultType s_ssd1306Result = I2C_SEQ_OK;

/* Private function prototypes ---------------------------------------- */
static void I2CServiceTask(void *arg);
static void SSD1306Task(void *arg);
static void App_I2CSeqEndCallback(I2C_SequenceType SequenceId, I2C_SequenceResultType Result);
static Std_ReturnType SSD1306_SendSync(const uint8 *txData, I2C_NumberOfDataType len);
static Std_ReturnType SSD1306_SendAsync(const uint8 *txData, I2C_NumberOfDataType len);

/* Function definitions ----------------------------------------------- */
static void App_I2CSeqEndCallback(I2C_SequenceType SequenceId, I2C_SequenceResultType Result)
{
    if (SequenceId == I2C_SEQ_SSD1306)
    {
        s_ssd1306Result = Result;
        s_ssd1306Done   = TRUE;
    }
}

static Std_ReturnType SSD1306_SendSync(const uint8 *txData, I2C_NumberOfDataType len)
{
    I2C_DataConstPtrType txPtr = txData;

    if (I2C_SetupEB(I2C_SEQ_SSD1306, SSD1306_I2C_ADDR, &txPtr, NULL_PTR, len) != E_OK)
    {
        return E_NOT_OK;
    }
    return I2C_SyncTransmit(I2C_SEQ_SSD1306);
}

static Std_ReturnType SSD1306_SendAsync(const uint8 *txData, I2C_NumberOfDataType len)
{
    I2C_DataConstPtrType txPtr = txData;

    if (I2C_SetupEB(I2C_SEQ_SSD1306, SSD1306_I2C_ADDR, &txPtr, NULL_PTR, len) != E_OK)
    {
        return E_NOT_OK;
    }
    return I2C_AsyncTransmit(I2C_SEQ_SSD1306);
}

static void I2CServiceTask(void *arg)
{
    (void) arg;

    TickType_t lastWake = xTaskGetTickCount();
    const TickType_t periodTicks = SAFE_MS_TO_TICKS(I2C_SERVICE_PERIOD_MS);

    for (;;)
    {
        if (xSemaphoreTake(s_i2cMutex, SAFE_MS_TO_TICKS(2u)) == pdTRUE)
        {
            I2C_MainFunction();
            xSemaphoreGive(s_i2cMutex);
        }

        vTaskDelayUntil(&lastWake, periodTicks);
    }
}

static void SSD1306Task(void *arg)
{
    (void) arg;

    static const uint8 initCmds[] = {
        0x00u, /* Control byte: following bytes are commands */
        0xAEu, 0x20u, 0x00u, 0x21u, 0x00u, 0x7Fu, 0x22u, 0x00u, 0x07u,
        0xA1u, 0xC8u, 0x8Du, 0x14u, 0xAFu
    };

    uint8 pattern = 0x00u;
    uint8 frame[SSD1306_DATA_PKT_LEN];

    if (xSemaphoreTake(s_i2cMutex, SAFE_MS_TO_TICKS(100u)) == pdTRUE)
    {
        if (SSD1306_SendSync(initCmds, (I2C_NumberOfDataType) sizeof(initCmds)) != E_OK)
        {
            printf("SSD1306 init failed\n");
        }
        xSemaphoreGive(s_i2cMutex);
    }

    for (;;)
    {
        frame[0] = 0x40u; /* Control byte: following bytes are display data */
        for (uint16 i = 1u; i < SSD1306_DATA_PKT_LEN; i++)
        {
            frame[i] = pattern;
        }
        pattern = (pattern == 0x00u) ? 0xFFu : 0x00u;

        if (xSemaphoreTake(s_i2cMutex, SAFE_MS_TO_TICKS(20u)) == pdTRUE)
        {
            s_ssd1306Done = FALSE;
            if (SSD1306_SendAsync(frame, (I2C_NumberOfDataType) SSD1306_DATA_PKT_LEN) != E_OK)
            {
                printf("SSD1306 async send rejected\n");
                xSemaphoreGive(s_i2cMutex);
                vTaskDelay(SAFE_MS_TO_TICKS(DISPLAY_UPDATE_PERIOD_MS));
                continue;
            }
            xSemaphoreGive(s_i2cMutex);
        }

        /* Completion is deferred to I2C_MainFunction in polling mode. */
        uint8 waitStep = 0u;
        for (waitStep = 0u; waitStep < 20u; waitStep++)
        {
            if (s_ssd1306Done == TRUE)
            {
                break;
            }
            vTaskDelay(SAFE_MS_TO_TICKS(5u));
        }

        if ((s_ssd1306Done == FALSE) || (s_ssd1306Result != I2C_SEQ_OK))
        {
            printf("SSD1306 async result=%u\n", (unsigned int) s_ssd1306Result);
        }

        vTaskDelay(SAFE_MS_TO_TICKS(DISPLAY_UPDATE_PERIOD_MS));
    }
}

void app_main(void)
{
    /* Register the sequence-end callback used by async transmit path. */
    I2C_SeqEndNotification = App_I2CSeqEndCallback;

    I2C_Init(&i2c_config);

    s_i2cMutex    = xSemaphoreCreateMutex();

    if (s_i2cMutex == NULL)
    {
        printf("I2C mutex creation failed\n");
        return;
    }

    (void) xTaskCreate(SSD1306Task, "ssd1306_task", 4096u, NULL, 5u, NULL);
    (void) xTaskCreate(I2CServiceTask, "i2c_service", 3072u, NULL, 6u, NULL);

    printf("SSD1306 I2C demo started\n");
}
/* End of file -------------------------------------------------------- */
