#pragma once

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
extern I2S_HandleTypeDef hi2s3;

typedef enum
{
    C4 = 0,
    C5, D5, E5, F5, G5, A5, B5,
    C6, D6, E6, F6, G6, A6, B6,
    C7,
    MAX_VALUE
} soundToneType;

void CS43L22_Init(void);
void CS43L22_Beep(soundToneType pitch, uint32_t duration_ms);
void NoteWithLed(uint16_t pin, soundToneType note);
