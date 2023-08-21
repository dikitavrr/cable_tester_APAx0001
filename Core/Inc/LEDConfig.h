/*
 * LEDConfig.h
 *
 *  Created on: Aug 4, 2023
 *      Author: User
 */

#ifndef LEDCONFIG_H_
#define LEDCONFIG_H_

#define m_CALL_RED_LED_1_POS 4
#define m_CALL_RED_LED_2_POS 5
#define m_CALL_RED_LED_3_POS 6
#define m_CALL_RED_LED_4_POS 7
#define m_CALL_RED_LED_5_POS 0
#define m_CALL_RED_LED_6_POS 1
#define m_CALL_RED_LED_7_POS 2
#define m_CALL_RED_LED_8_POS 3

#define m_CALL_GREEN_LED_1_POS 3
#define m_CALL_GREEN_LED_2_POS 2
#define m_CALL_GREEN_LED_3_POS 1
#define m_CALL_GREEN_LED_4_POS 0
#define m_CALL_GREEN_LED_5_POS 7
#define m_CALL_GREEN_LED_6_POS 6
#define m_CALL_GREEN_LED_7_POS 5
#define m_CALL_GREEN_LED_8_POS 4

#define m_RESP_GREEN_LED_1_POS 3
#define m_RESP_GREEN_LED_2_POS 2
#define m_RESP_GREEN_LED_3_POS 1
#define m_RESP_GREEN_LED_4_POS 0
#define m_RESP_GREEN_LED_5_POS 7
#define m_RESP_GREEN_LED_6_POS 6
#define m_RESP_GREEN_LED_7_POS 5
#define m_RESP_GREEN_LED_8_POS 4

#define m_RESP_RED_LED_1_POS 4
#define m_RESP_RED_LED_2_POS 5
#define m_RESP_RED_LED_3_POS 6
#define m_RESP_RED_LED_4_POS 7
#define m_RESP_RED_LED_5_POS 0
#define m_RESP_RED_LED_6_POS 1
#define m_RESP_RED_LED_7_POS 2
#define m_RESP_RED_LED_8_POS 3

#define m_LED_ON 1
#define m_LED_OFF 0

#define m_NUMBER_OF_COLORS 4
#define m_NOTHING_COLOR 0
#define m_GREEN_COLOR 1
#define m_RED_COLOR 2
#define m_YELLOW_COLOR 3

#define m_CALL_OK_COLOR m_GREEN_COLOR
#define m_CALL_MISTAKE_COLOR m_YELLOW_COLOR
#define m_CALL_BROKEN_LINE_COLOR m_RED_COLOR
#define m_RESP_OK_COLOR m_GREEN_COLOR
#define m_RESP_MISTAKE_COLOR m_YELLOW_COLOR
#define m_RESP_BROKEN_LINE_COLOR m_RED_COLOR
#define m_RESP_EXPECTED_LINE_COLOR m_RED_COLOR

#define m_USE_TIMER 1

#define m_TIME_BLINKING_LED_MS 100
//#define m_CLOCK_FREQUENCY 16000000
//#define m_CLOCK_PRESCALER 125
//#define m_CLOCK_PERIOD 32499

#define m_NUMBER_OF_LINES 8
#define m_NUMBER_OF_LEDS 16

#define m_NUMBER_OF_SR 2

#define m_SR_DATA_bm 0b00000001
#endif /* LEDCONFIG_H_ */
