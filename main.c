/*
 * File:   main.c
 * Author: Utku
 *
 * Created on December 30, 2022, 9:07 AM
 */

#define _XTAL_FREQ 16000000
#define TMR2PRESCALE 4


#include "config.h"
#include <xc.h>

#include "oscillator.h"
#include "tmr2.h"
#include "pwm.h"
#include "pwm1.h"

unsigned int value;

void main(void) {
    intosc_set_freq();

    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;

    ADCON0bits.CHS = 0b00000; // Select Channel AN0
    ADCON0bits.GO_nDONE = 0; // Clear "ADC in progress" bit
    ADCON0bits.ADON = 1; // Enable ADC

    ADCON1bits.ADCS = 0b101; // Fosc / 16
    ADCON1bits.ADFM = 1; // ADC Result Format Select bit (1 = Right Justified))

    ANSELAbits.ANSA0 = 1; // A0 pin is analog
    TRISA0 = 1; // A0 pin is input

    tmr2_init();
    pwm_set_freq(500);

    tmr2_enable();
    pwm1_enable();
    TRISC2 = 0;
    RC2 = 0;

    while (1) {
        ADCON0bits.GO_nDONE = 1; // Initiate A/D conversion
        while (ADCON0bits.GO_nDONE); // Wait until A/D conversion is done
        value = ((ADRESH << 8) + ADRESL); // Read registers

        pwm1_set_duty(value);
        __delay_ms(50);
    }
    return;
}
