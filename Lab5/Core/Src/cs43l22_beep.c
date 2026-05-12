#include "cs43l22_beep.h"

#define CS43L22_I2C_ADDRESS  0x94
#define I2C_TIMEOUT          10

void CS43L22_Init(void)
{
    // Release reset via PD4 (Audio_RST)
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

    uint8_t buf[2];

    // Unlock protected registers
    buf[0] = 0x0D; buf[1] = 0x01;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x00; buf[1] = 0x99;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x47; buf[1] = 0x80;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x32; buf[1] = 0xFF;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x32; buf[1] = 0x7F;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Lock protected registers
    buf[0] = 0x00; buf[1] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Output routing: headphones on, speaker off
    buf[0] = 0x04; buf[1] = 0xAF;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x0D; buf[1] = 0x70;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Clocking: auto-detect
    buf[0] = 0x05; buf[1] = 0x81;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // I2S interface setup
    buf[0] = 0x06; buf[1] = 0x07;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x0A; buf[1] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x27; buf[1] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Headphone volume (channels A and B)
    buf[0] = 0x1A; buf[1] = 0x0A;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    buf[0] = 0x1B; buf[1] = 0x0A;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Master volume
    buf[0] = 0x1F; buf[1] = 0x0F;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Power on
    buf[0] = 0x02; buf[1] = 0x9E;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);
}

void CS43L22_Beep(soundToneType pitch, uint32_t duration_ms)
{
    uint8_t buf[2];

    // Clear previous beep state
    buf[0] = 0x1D; buf[1] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Load frequency for the requested note into reg 0x1C
    buf[0] = 0x1C;
    switch (pitch)
    {
        case C4: buf[1] = 0x00; break;
        case C5: buf[1] = 0x10; break;
        case D5: buf[1] = 0x20; break;
        case E5: buf[1] = 0x30; break;
        case F5: buf[1] = 0x40; break;
        case G5: buf[1] = 0x50; break;
        case A5: buf[1] = 0x60; break;
        case B5: buf[1] = 0x70; break;
        case C6: buf[1] = 0x80; break;
        case D6: buf[1] = 0x90; break;
        case E6: buf[1] = 0xA0; break;
        case F6: buf[1] = 0xB0; break;
        case G6: buf[1] = 0xC0; break;
        case A6: buf[1] = 0xD0; break;
        case B6: buf[1] = 0xE0; break;
        case C7: buf[1] = 0xF0; break;
        default: buf[1] = 0x00; break;
    }
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    // Start continuous beep (reg 0x1E)
    buf[0] = 0x1E; buf[1] = 0xC0;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);

    HAL_Delay(duration_ms);

    // Stop beep
    buf[0] = 0x1E; buf[1] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDRESS, buf, 2, I2C_TIMEOUT);
}

// Toggle an LED and play the given note for 300 ms
void NoteWithLed(uint16_t pin, soundToneType note)
{
    HAL_GPIO_TogglePin(GPIOD, pin);
    CS43L22_Beep(note, 300);
    HAL_Delay(100);
}
