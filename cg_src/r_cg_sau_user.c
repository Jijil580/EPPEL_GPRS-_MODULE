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
* File Name    : r_cg_sau_user.c
* Version      : Code Generator for RL78/I1C V1.01.07.02 [08 Nov 2021]
* Device(s)    : R5F10NLE
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 18-10-2024
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "string.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_uart0_interrupt_send(vect=INTST0)
#pragma interrupt r_uart0_interrupt_receive(vect=INTSR0)
#pragma interrupt r_uart1_interrupt_send(vect=INTST1)
#pragma interrupt r_uart1_interrupt_receive(vect=INTSR1)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
extern volatile uint8_t * gp_uart1_tx_address;         /* uart1 send buffer address */
extern volatile uint16_t  g_uart1_tx_count;            /* uart1 send data number */
extern volatile uint8_t * gp_uart1_rx_address;         /* uart1 receive buffer address */
extern volatile uint16_t  g_uart1_rx_count;            /* uart1 receive data number */
extern volatile uint16_t  g_uart1_rx_length;           /* uart1 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
 uint8_t LINE_END_COUNT=0;
 
 
 uint8_t RX0_BUFFER[200];
 uint8_t TX0_BUFFER[100];
 uint8_t RX1_BUFFER[100];
 uint8_t TX1_BUFFER[500];
 
int  RX0_BUFFER_COUNT=0;
 uint8_t TX0_BUFFER_COUNT=0;
 uint8_t RX1_BUFFER_COUNT=0;
 uint8_t TX1_BUFFER_COUNT=0;
 uint8_t MAIN_RX_STORE_COUNT=0;
 uint8_t END_OF_RESPONSE=0;
  uint8_t END_OF_RESPONSE1=0;
uint8_t MAIN_RX_STORE[200];
 uint8_t final_buffer[200]; // Adjust size as per your needs
/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = RXD0;

    if ( rx_data>=32&& rx_data<=126|| rx_data=='\r'|| rx_data=='\n')
    {
	    
	    
	if (RX0_BUFFER_COUNT < 100- 1 && MAIN_RX_STORE_COUNT < 1000 - 1) // Ensure we don't overflow
        {
              
      //  *gp_uart0_rx_address = rx_data;
	RX0_BUFFER[RX0_BUFFER_COUNT]= rx_data;
	MAIN_RX_STORE[MAIN_RX_STORE_COUNT]=rx_data;
	RX0_BUFFER_COUNT++;
	MAIN_RX_STORE_COUNT++;
	
	
	}
	if (rx_data == '>')
	{
		LINE_END_COUNT++;
	}
	
        if (rx_data == '\n')
        {
            LINE_END_COUNT++;
            // Check for the end of response
            if (RX0_BUFFER_COUNT >= 3 &&   // Ensure we have enough characters
                RX0_BUFFER[RX0_BUFFER_COUNT - 1] == '\n' &&
                RX0_BUFFER[RX0_BUFFER_COUNT - 2] == '\r' &&
                RX0_BUFFER[RX0_BUFFER_COUNT - 3] == 'K'&& // Check for "OK\r\n"
		RX0_BUFFER[RX0_BUFFER_COUNT - 4] == 'O') 
            {
                END_OF_RESPONSE = 1;
                //RX0_BUFFER_COUNT = 0; // Reset buffer count for the next response
            }
        }
        // gp_uart0_rx_address++;
        // g_uart0_rx_count++;

       
	
    }
    
       if(END_OF_RESPONSE==1)
       {
	 
          
                   
     // r_uart0_callback_receiveend();
      UART0_RECIEVED_DATA[ g_uart0_rx_count]='\0';
      RX0_BUFFER[RX0_BUFFER_COUNT]='\0';
      r_uart0_callback_receiveend();
 
    
      }
    
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
}
/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
	//memset(UART1_RECIEVED_DATA,0,100);
	memset(RX1_BUFFER,0,100);
	memset(MAIN_RX_STORE,0,200);
	memset(final_buffer,0,200);
	RX1_BUFFER_COUNT=0;
	MAIN_RX_STORE_COUNT=0;
	LINE_END_COUNT=0;
        g_uart1_rx_count=0;
        r_uart0_callback_sendend();
    }
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
     R_UART1_Send(RX0_BUFFER, sizeof(RX0_BUFFER));
     
    
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_receive
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart1_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR03 & 0x0007U);
    SIR03 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart1_callback_error(err_type);
    }
    
    rx_data = RXD1;

   if ( rx_data>=32&& rx_data<=126|| rx_data=='\r'|| rx_data=='\n')
    {
	    
	    
	if (RX1_BUFFER_COUNT < 100- 1 && MAIN_RX_STORE_COUNT < 100 - 1) // Ensure we don't overflow
         {
               
      //  *gp_uart0_rx_address = rx_data;
	  RX1_BUFFER[RX1_BUFFER_COUNT]= rx_data;
	 // MAIN_RX_STORE[MAIN_RX_STORE_COUNT]=rx_data;
	  RX1_BUFFER_COUNT++;
	  MAIN_RX_STORE_COUNT++;
	
	   }
        if (rx_data == '\n')
        {
            LINE_END_COUNT++;
            // Check for the end of response
            //if (RX1_BUFFER_COUNT >= 4 &&   // Ensure we have enough characters
                //RX1_BUFFER[RX1_BUFFER_COUNT - 1] == '\n' &&
               
           // {
                END_OF_RESPONSE1 = 1;
                //RX0_BUFFER_COUNT = 0; // Reset buffer count for the next response
           // }
        }
        // gp_uart0_rx_address++;
        // g_uart0_rx_count++;

       
	
    }
    
       if(END_OF_RESPONSE1)
       {
	 
         //UART1_RECIEVED_DATA[ g_uart0_rx_count]='\0';
         RX1_BUFFER[RX1_BUFFER_COUNT]='\0';
	 
	 strcpy((char *)final_buffer, (char *)(RX1_BUFFER + 2));    
         r_uart1_callback_receiveend();
 
          END_OF_RESPONSE1 = 0;
        }
    
    else
    {
        r_uart1_callback_softwareoverrun(rx_data);
    }
}
/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_send
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart1_interrupt_send(void)
{
    if (g_uart1_tx_count > 0U)
    {
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
    }
    else
    {
	memset(UART0_RECIEVED_DATA,0,200);
	memset(RX0_BUFFER,0,200);
	memset(RX1_BUFFER,0,200);
	memset(MAIN_RX_STORE,0,200);
	memset(final_buffer,0,200);
	RX0_BUFFER_COUNT=0;
	RX1_BUFFER_COUNT=0;
	MAIN_RX_STORE_COUNT=0;
	LINE_END_COUNT=0;
        g_uart0_rx_count=0;
        r_uart1_callback_sendend();
	STMK1 = 1U; 
    }
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_receiveend
* Description  : This function is a callback function when UART1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    
    R_UART0_Send(final_buffer, strlen((char *)final_buffer));

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_softwareoverrun
* Description  : This function is a callback function when UART1 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_sendend
* Description  : This function is a callback function when UART1 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_error
* Description  : This function is a callback function when UART1 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */