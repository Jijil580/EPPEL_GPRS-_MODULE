/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/I1C V1.01.07.02 [08 Nov 2021]
* Device(s)    : R5F10NLE
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 11-10-2024
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_tau.h"

#include "r_cg_sau.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
uint8_t AT_COMMAND[]="AT+GMM\r\n";
uint8_t UART1_RECIEVED_DATA[100];
uint8_t UART1_SEND_DATA[100];
uint8_t UART0_RECIEVED_DATA[100];
uint8_t UART0_SEND_DATA[100];
uint8_t RECIEVED_DATA_SIZE;
uint8_t MODULE_FEEDBACK[100];
 long int TIMER_COUNT;


uint8_t MODULE_FEEDBACK[100];
int FEEDBACK_LENGTHG;

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

static void R_MAIN_UserInit(void);
void __delay_ms(unsigned int milliseconds);

void __delay_ms(unsigned int milliseconds) {
    volatile unsigned int i, j;
    for (i = 0; i < milliseconds; i++) {
        for (j = 0; j < 1000; j++) {
          //  __asm("nop");  // No operation, just waste some cycles
        }
    }
}

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    R_UART0_Start();
    R_UART1_Start();
    R_TAU0_Channel0_Start();
    P7&=~(1<<2);
    P7 |=(1<<1);
    
    
    
     P7 |=(1<<3);
    
    __delay_ms(1000);
    __delay_ms(1000);

    
     P7&=~(1<<3);
    
 
     R_UART0_Receive(UART0_RECIEVED_DATA, sizeof(UART0_RECIEVED_DATA));
     
     R_UART1_Receive(UART1_RECIEVED_DATA, sizeof(UART1_RECIEVED_DATA));
     
     //R_UART0_Send(at_command, sizeof(at_command));
     
     __delay_ms(500);
    /* Start user code. Do not edit comment generated here */
    while (1U)
    {
	if(TIMER_COUNT>10)
	{
		
             // R_UART0_Send(AT_COMMAND, sizeof(AT_COMMAND));
              TIMER_COUNT=0;
	}
     //__delay_ms(1000);
        ;
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
