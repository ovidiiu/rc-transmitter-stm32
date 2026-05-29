#pragma once
#include <Arduino.h>

// ============================================
//  PIN DEFINITIONS — STM32F412RET6 Custom RC TX
// ============================================

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

// --- Trims ---
#define PIN_TRIM1_PLUS      PD0
#define PIN_TRIM1_MINUS     PD1
#define PIN_TRIM2_PLUS      PD3
#define PIN_TRIM2_MINUS     PD4
#define PIN_TRIM3_PLUS      PD5
#define PIN_TRIM3_MINUS     PD6
#define PIN_TRIM4_PLUS      PD7
#define PIN_TRIM4_MINUS     PE0

#define NUM_TRIMS           4
#define TRIM_MAX            100
#define TRIM_MIN            -100
#define TRIM_STEP           2

// --- Switches ---
#define PIN_SW_A            PE1
#define PIN_SW_B_UP         PE2
#define PIN_SW_B_DN         PE3
#define PIN_SW_C_UP         PE4
#define PIN_SW_C_DN         PE5
#define PIN_SW_D            PE6
#define PIN_SW_F            PE7
#define PIN_SW_H            PE8

// --- Buzzer ---
#define PIN_BUZZER          PA8

// --- USB ---
#define PIN_USB_DM          PA11
#define PIN_USB_DP          PA12
#define PIN_USB_VBUS        PA9

// --- SD Card (SDIO) ---
#define PIN_SD_DETECT       PC7

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
