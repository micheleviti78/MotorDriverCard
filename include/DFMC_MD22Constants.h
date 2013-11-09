#ifndef MTCA4U_DMFC_MD22_CONSTANTS_H
#define MTCA4U_DMFC_MD22_CONSTANTS_H

#define SIZE_OF_SPI_ADDRESS_SPACE 0x40

#define SPI_CONTROL_WRITE_ADDRESS_STRING "WORD_SPI_CTRL"
#define SPI_CONTROL_READBACK_ADDRESS_STRING "WORD_SPI_CTRL_B"

#define SMDA_COMMON 0x3
#define JDX_DATAGRAM_LOW_WORD 0x0
#define JDX_DATAGRAM_HIGH_WORD 0x1
#define JDX_COVER_POSITION_AND_LENGTH 0x2
#define JDX_COVER_DATAGRAM 0x3
#define JDX_INTERFACE_CONFIGURATION 0x4
#define JDX_POSITION_COMPARE 0x5
#define JDX_POSITION_COMPARE_INTERRUPT 0x6
// 0x7 not defined
#define JDX_POWER_DOWN 0x8
#define JDX_CHIP_VERSION 0x9
// 0xA through 0xD not defined
#define JDX_REFERENCE_SWITCH 0xE
#define JDX_STEPPER_MOTOR_GLOBAL_PARAMETERS 0xF

#define CONTROLER_CHIP_VERSION 0x429101

#define IDX_TARGET_POSITION 0x0
#define IDX_ACTUAL_POSITION 0x1
#define IDX_MINIMUM_VELOCITY 0x2
#define IDX_MAXIMUM_VELOCITY 0x3
#define IDX_TARGET_VELOCITY 0x4
#define IDX_ACTUTAL_VELOCITY 0x5
#define IDX_MAXIMUM_ACCELERATION 0x6
#define IDX_ACTUAL_VELOCITY 0x7
#define IDX_ACCELERATION_THRESHOLD 0x8
#define IDX_PROPORTIONALITY_FACTORS 0x9
#define IDX_REF_SWITCH_CONF_AND_RAMP_MODE 0xA
#define IDX_INTERRUPT_MASK_AND_FLAGS 0xB
#define IDX_PULSE_AND_RAMP_DIV_AND_MICRO_STEP_RESOLUTION 0xC
#define IDX_DELTA_X_REFERENCE_TOLERANCE 0xD
#define IDX_POSITION_LATCHED 0xE
#define IDX_MICRO_STEP_COUNT 0xF

#define RW_WRITE 0
#define RW_READ 1

#endif// MTCA4U_DMFC_MD22_CONSTANTS_H
