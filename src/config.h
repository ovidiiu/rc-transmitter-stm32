#pragma once
#include <Arduino.h>

// ============================================
//  PIN DEFINITIONS — STM32F412RET6 (LQFP-64)
//  Only Port A, B, C available
// ============================================

// --- Power Latch ---
#define PIN_POWER_HOLD      PB15
#define PIN_POWER_SENSE     PA15
#define POWER_OFF_HOLD_MS   2000    // 2 second long press to turn off

// --- Display (SH1107 128x128 I2C) ---
#define PIN_I2C_SDA         PB7
#define PIN_I2C_SCL         PB6
#define DISPLAY_I2C_ADDR    0x3C
#define DISPLAY_OFFSET      96

// --- ELRS Module (CRSF over USART2) ---
#define PIN_CRSF_TX         PA2
#define PIN_CRSF_RX         PA3
#define PIN_MODULE_EN       PB14
#define CRSF_BAUDRATE       420000
#define CRSF_MAX_FRAME      64
#define CRSF_NUM_CHANNELS   16
#define CRSF_CHANNEL_MIN    172
#define CRSF_CHANNEL_MID    992
#define CRSF_CHANNEL_MAX    1811

// --- Gimbals & Pots (ADC1) ---
#define PIN_STICK_AIL       PA0
#define PIN_STICK_ELE       PC0
#define PIN_STICK_THR       PC1
#define PIN_STICK_RUD       PC2
#define PIN_POT_VRA         PC3
#define PIN_POT_VRB         PC4
#define PIN_VBAT            PC5

#define NUM_STICKS          4
#define NUM_POTS            2
#define NUM_ANALOGS         7
#define ADC_RESOLUTION      12
#define ADC_MAX             4095
#define ADC_OVERSAMPLE      16

#define VBAT_SCALE          3.128f
#define VBAT_LOW            6.6f
#define VBAT_CRITICAL       6.2f

// --- Buttons (active low, internal pullup) ---
#define PIN_BTN_MENU        PB3
#define PIN_BTN_PAGE        PB4
#define PIN_BTN_EXIT        PB5
#define PIN_BTN_ENTER       PB12
#define PIN_ENC_A           PB13
#define PIN_ENC_B           PB8
#define PIN_ENC_BTN         PB9

// --- Trims (remapped to Port A/B/C — no PD/PE on LQFP-64) ---
#define PIN_TRIM1_PLUS      PA4
#define PIN_TRIM1_MINUS     PA5
#define PIN_TRIM2_PLUS      PA6
#define PIN_TRIM2_MINUS     PA7
#define PIN_TRIM3_PLUS      PB0
#define PIN_TRIM3_MINUS     PB1
#define PIN_TRIM4_PLUS      PB2
#define PIN_TRIM4_MINUS     PB10

#define NUM_TRIMS           4
#define TRIM_MAX            100
#define TRIM_MIN            -100
#define TRIM_STEP           2

// --- Switches (remapped to Port B/C) ---
#define PIN_SW_A            PB15
#define PIN_SW_B_UP         PC6
#define PIN_SW_B_DN         PC7
#define PIN_SW_C_UP         PC8
#define PIN_SW_C_DN         PC9
#define PIN_SW_D            PC10
#define PIN_SW_F            PC11
#define PIN_SW_H            PC12

// --- Buzzer ---
#define PIN_BUZZER          PA8

// --- USB ---
#define PIN_USB_DM          PA11
#define PIN_USB_DP          PA12
#define PIN_USB_VBUS        PA9

// --- SD Card (SDIO) ---
#define PIN_SD_DETECT       PC13

// --- Trainer (USART3) ---
#define PIN_TRAINER_TX      PB10
#define PIN_TRAINER_RX      PB11

// --- Timing ---
#define LOOP_INTERVAL_US    1000
#define CRSF_INTERVAL_MS    4
#define DISPLAY_INTERVAL_MS 20
#define TELEM_INTERVAL_MS   100
#define INPUT_DEBOUNCE_MS   20

// --- Model ---
#define MAX_MODELS          32
#define MAX_MIXES_PER_CH    4
#define MODEL_NAME_LEN      12
