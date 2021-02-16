/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Projekt: ARM-FITkit3: Pøerušení od modulu Periodic Interrupt Timer (PIT)
 * Autor: Michal Koval, xkoval17@stud.fit.vutbr.cz
 * 20.12.2020
 */


#include "MK60D10.h"

/////////////
////  MAKRA
/////////////

// Mapovanie LED, zdroj: fitkit3 demo
#define LED_D9  0x20      // Port B, bit 5
#define LED_D10 0x10      // Port B, bit 4
#define LED_D11 0x8       // Port B, bit 3
#define LED_D12 0x4       // Port B, bit 2
// Mapovanie tlacitok, zdroj: fitkit3 demo
#define BTN_SW2 0x400     // Port E, bit 10
#define BTN_SW3 0x1000    // Port E, bit 12
#define BTN_SW4 0x8000000 // Port E, bit 27
#define BTN_SW5 0x4000000 // Port E, bit 26

////////////////////////
////  GLOBALNE PREMENNE
////////////////////////
int rezim = 0;

/////////////////
////  PROTOTYPY
/////////////////
void setup_max_time(void);

void setup_min_time(void);

void delay(long long bound);

void setup_rezim_0(void);

void setup_rezim_1(void);

void setup_rezim_2(void);

void setup_rezim_3(void);

void clear_current_setup();

///////////////
////  FUNKCIE
///////////////

/* Demonstracna funkcia na ukazanie nastavenia maximalnej
   cakacej doby so zretazenim vsetkych casovacov.
   Nie je pouzita lebo by sme sa doslova nedockali kym by vyprsal cas.*/
void setup_max_time(void)
{
	PIT_LDVAL0 = 0xFFFFFFFF; // nastavenie TSV casovaca na maximum
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 0

	PIT_LDVAL1 = 0xFFFFFFFF; // nastavenie TSV casovaca na maximum
	PIT_TCTRL1 |= PIT_TCTRL_CHN_MASK; // zretazenie casovaca 1 s casovacom 0
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 1


	PIT_LDVAL2 = 0xFFFFFFFF; // nastavenie TSV casovaca na maximum
	PIT_TCTRL2 |= PIT_TCTRL_CHN_MASK; // zretazenie casovaca 2 s casovacom 1
	PIT_TCTRL2 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 2


	PIT_LDVAL3 = 0xFFFFFFFF; // nastavenie TSV casovaca na maximum
	PIT_TCTRL3 |= PIT_TCTRL_TIE_MASK; // aktivujeme prerusenie pre PIT casovac 3
	PIT_TCTRL3 |= PIT_TCTRL_CHN_MASK; // zretazenie casovaca 3 s casovacom 2
	PIT_TCTRL3 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 3

}

/* Demonstracna funkcia na ukazanie nastavenia minimalnej cakacej doby pre PIT timer.
   Nie je pouzita lebo tuna by sme si to prerusenie zas na druhej strane nestihli vsimnut.*/
void setup_min_time(void)
{
	PIT_LDVAL0 = 0; // nastavenie na prerusenia kazdy tik hodin
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK; // aktivujeme prerusenie pre PIT casovac 0
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 0
}

void PIT0_IRQHandler(void)
{
	GPIOB_PDOR ^= LED_D9; // Zapnutie ledky
	delay(10000000); // aktivne cakanie 0,1 sekundy
	GPIOB_PDOR ^= LED_D9; // Vypnutie ledky
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK; // odnastavime interrupt flag
}

void PIT1_IRQHandler(void)
{
	GPIOB_PDOR ^= LED_D10; // Zapnutie ledky
	delay(10000000); // aktivne cakanie 0,1 sekundy
	GPIOB_PDOR ^= LED_D10; // Vypnutie ledky
	PIT_TFLG1 |= PIT_TFLG_TIF_MASK; // odnastavime interrupt flag
}

void PIT2_IRQHandler(void)
{
	GPIOB_PDOR ^= LED_D11; // Zapnutie ledky
	delay(10000000); // aktivne cakanie 0,1 sekundy
	GPIOB_PDOR ^= LED_D11; // Vypnutie ledky
	PIT_TFLG2 |= PIT_TFLG_TIF_MASK; // odnastavime interrupt flag
}

void PIT3_IRQHandler(void)
{
	GPIOB_PDOR ^= LED_D12; // Zapnutie ledky
	delay(10000000); // aktivne cakanie 0,1 sekundy
	GPIOB_PDOR ^= LED_D12; // Vypnutie ledky
	PIT_TFLG3 |= PIT_TFLG_TIF_MASK; // odnastavime interrupt flag
}


/* Aktivne cakanie jadra, zdroj: fitkit3 demo */
void delay(long long bound)
{
  long long i;
  for(i=0;i<bound;i++);
}

/* Inicializacia MCU */
void MCUInit(void)
{
	MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
	SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
	WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK; // vypnutie watchdogu
}

/* Inicializacia PIT modulu */
void PITInit(void)
{
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; // zapnutie casu pre PIT
	PIT_MCR = 0x00; // zapnutie PIT modulu

	// Pre istotu vymazeme cakajuce PIT prerusenia
	NVIC_ClearPendingIRQ(PIT0_IRQn);
	NVIC_ClearPendingIRQ(PIT1_IRQn);
	NVIC_ClearPendingIRQ(PIT2_IRQn);
	NVIC_ClearPendingIRQ(PIT3_IRQn);

	// Aktivujeme nacuvanie PIT preruseniam
	NVIC_EnableIRQ(PIT0_IRQn);
	NVIC_EnableIRQ(PIT1_IRQn);
	NVIC_EnableIRQ(PIT2_IRQn);
	NVIC_EnableIRQ(PIT3_IRQn);
}

/* Inicializacia portov, zdroj: fitkit3 demo */
void PortsInit(void)
{
	// Nastavenie portov pre tlacitka
	PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
	PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5

	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTA_MASK;

	/* Set corresponding PTB pins (connected to LED's) for GPIO functionality */
	PORTB->PCR[5] = PORT_PCR_MUX(0x01); // D9
	PORTB->PCR[4] = PORT_PCR_MUX(0x01); // D10
	PORTB->PCR[3] = PORT_PCR_MUX(0x01); // D11
	PORTB->PCR[2] = PORT_PCR_MUX(0x01); // D12

	PTB->PDDR = GPIO_PDDR_PDD(0x3C);     // LED ports as outputs
	PTB->PDOR |= GPIO_PDOR_PDO(0x3C);    // turn all LEDs OFF
}

/* Nezretazeny 1 casovac
 * casovac 0 zasvieti LED9 raz za sekundu
 */
void setup_rezim_0(void)
{
	PIT_LDVAL0 = 0x05F5E0FF; // nastavenie TSV na 1 sekundu
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK; // aktivujeme prerusenie pre PIT casovac 0
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 0

	rezim = 0;
}

/* Nezretazene 2 casovace
 * casovac 0 zasvieti LED9 raz za 1 sekundu
 * casovac 1 zasvieti LED10 raz za 2 sekundy
 */
void setup_rezim_1(void)
{
	PIT_LDVAL0 = 0x05F5E0FF; // nastavenie TSV na 1 sekundu
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK; // aktivujeme prerusenie pre PIT casovac 0
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 0

	PIT_LDVAL1 = 0x0BEBC1FF; // nastavenie TSV na 2 sekundy
	PIT_TCTRL1 |= PIT_TCTRL_TIE_MASK; // aktivujeme prerusenie pre PIT casovac 1
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 1

	rezim = 1;
}


/* Zretazenie 2 casovacov
 * casovac 0 bude ratat kazdu 0.1 sekundu
 * casovac 1 bude rata 10 tikov od casovaca 0
 * Kazdu sekundu zasvieti LED11
 */
void setup_rezim_2(void)
{
	PIT_LDVAL0 = 0x0098967F; // nastavenie TSV na 0,1 sekundy
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 0

	PIT_LDVAL1 = 0x00000009; // nastavenie TSV na 10 tikov, dokopy teda 0.1s x 10 = 1s
	PIT_TCTRL1 |= PIT_TCTRL_TIE_MASK; // aktivujeme prerusenie pre PIT casovac 1
	PIT_TCTRL1 |= PIT_TCTRL_CHN_MASK; // zretazenie casovaca 1 s casovacom 0
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 1

	rezim = 2;
}


/* Zretazenie maximalneho poctu casovacov, tada 4
 * casovac 0 bude ratat nanosekundy po 10
 * casovac 1 bude ratat mikrosekundy
 * casovac 2 bude ratat milisekundy
 * casovac 3 bude ratat uz iba sekundy
 * Aby si aj casovac 3 aspon trochu zapracoval ale aby netrebalo dlho cakat,
 * tak to nastavime tak, aby sa LED12 zasvietila raz za 2 sekundy
 */
void setup_rezim_3(void)
{
	PIT_LDVAL0 = 0x00000063; // nastavenie TSV casovaca na 100, jeden takt trva pri 100MHz 10ns, takze ich ich naratat 100
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 0

	PIT_LDVAL1 = 0x000003E7; // nastavenie TSV casovaca na 1000, lebo 1ms = 1000us
	PIT_TCTRL1 |= PIT_TCTRL_CHN_MASK; // zretazenie casovaca 1 s casovacom 0
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 1

	PIT_LDVAL2 = 0x000003E7; // nastavenie TSV casovaca casovaca na 1000, lebo 1s = 1000ms
	PIT_TCTRL2 |= PIT_TCTRL_CHN_MASK; // zretazenie casovaca 2 s casovacom 1
	PIT_TCTRL2 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 2

	PIT_LDVAL3 = 0x00000001; // nastavenie TSV casovaca na 2 tiky, sekundy
	PIT_TCTRL3 |= PIT_TCTRL_TIE_MASK; // aktivujeme prerusenie pre PIT casovac 3
	PIT_TCTRL3 |= PIT_TCTRL_CHN_MASK; // zretazenie casovaca 3 s casovacom 2
	PIT_TCTRL3 |= PIT_TCTRL_TEN_MASK; // zapnutie casovaca 3

	rezim = 3;
}


/*
 * Deaktivuje aktualnr nastaveny rezim demonstracie PIT
 */
void clear_current_setup()
{
	if (rezim == 0){
		PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 0
		PIT_TCTRL0 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 0
	} else if (rezim == 1){
		PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 0
		PIT_TCTRL0 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 0

		PIT_TCTRL1 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 1
		PIT_TCTRL1 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 1
	} else if (rezim == 2){
		PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 0
		PIT_TCTRL0 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 0

		PIT_TCTRL1 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 1
		PIT_TCTRL1 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 1
		PIT_TCTRL1 &= ~PIT_TCTRL_CHN_MASK; // zrusenie zretazenia casovaca 1
	} else if (rezim == 3){
		PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 0
		PIT_TCTRL0 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 0

		PIT_TCTRL1 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 1
		PIT_TCTRL1 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 1
		PIT_TCTRL1 &= ~PIT_TCTRL_CHN_MASK; // zrusenie zretazenia casovaca 1

		PIT_TCTRL2 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 2
		PIT_TCTRL2 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 2
		PIT_TCTRL2 &= ~PIT_TCTRL_CHN_MASK; // zrusenie zretazenia casovaca 2

		PIT_TCTRL3 &= ~PIT_TCTRL_TEN_MASK; // zastavenie casovaca 3
		PIT_TCTRL3 &= ~PIT_TCTRL_TIE_MASK; // vypnutie preruseni casovacu 3
		PIT_TCTRL3 &= ~PIT_TCTRL_CHN_MASK; // zrusenie zretazenia casovaca 3
	}
}

/////////////
////  MAIN
/////////////

int main(void)
{
	MCUInit();
	PITInit();
	PortsInit();

	// Defaultne nastavime rezim 0
	setup_rezim_0();

	// Hlavna slucka kde kontrolujeme stlacenie tlacitok a zmeny rezimov
    while (1) {
    	if ((GPIOE_PDIR & BTN_SW5) && rezim != 3){
    		clear_current_setup(); // deaktivacia aktualneho rezimu
    		setup_rezim_3(); // zapnutie rezimu 3
    	} else if ((GPIOE_PDIR & BTN_SW4) && rezim != 0){
    		clear_current_setup(); // deaktivacia aktualneho rezimu
    		setup_rezim_0(); // zapnutie rezimu 0
    	} else if ((GPIOE_PDIR & BTN_SW3) && rezim != 1){
    		clear_current_setup(); // deaktivacia aktualneho rezimu
    		setup_rezim_1(); // zapnutie rezimu 1
    	} else if ((GPIOE_PDIR & BTN_SW2) && rezim != 2){
    		clear_current_setup(); // deaktivacia aktualneho rezimu
    		setup_rezim_2(); // zapnutie rezimu 2
    	}

    }
    /* Tu sa program nikdy nedostane */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
