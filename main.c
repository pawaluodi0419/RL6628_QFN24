/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550         9600
 *   uartlite          Configurable only in HW design
 *   print_baudrate    115200 (configured by bootrom/bsp)
 *   data              8bit
 */


#include "platform.h"

//function declare
//void uart0_intrq_handle();
//void uart1_intrq_handle();
//void uart2_intrq_handle();
//void uart3_intrq_handle();

void axi_tmr0_intrq_handle();
void axi_tmr1_intrq_handle();
void axi_tmr2_intrq_handle();

void XGPIO_0_Intrq_Handle();
void XGPIO_dut0_1_Intrq_Handle();
void XGPIO_dut1_1_Intrq_Handle();
void XGPIO_dut2_1_Intrq_Handle();
void XGPIO_dut3_1_Intrq_Handle();

void print(char *str);

//struct define
struct DUT dut0;
struct DUT dut1;
struct DUT dut2;
struct DUT dut3;

//globe define
u8 g_opaLedTg=0;
u8 g_ledD2Tg=0;
u8 g_ledD3Tg=0;
u8 g_ledD4Tg=0;
u8 g_ledD5Tg=0;
u8 g_ledD6Tg=0;
u8 g_ledD7Tg=0;

int main()
{
	pattern_initial_state();

	XGpio_0_WriteBit(0,LED_D6_OFFSET,g_ledD6Tg);
	g_ledD6Tg=~g_ledD6Tg;
	msdelay(3);
	XGpio_0_WriteBit(0,LED_D6_OFFSET,g_ledD6Tg);
	g_ledD6Tg=~g_ledD6Tg;

	//used for display FPGA code version
	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,17, 1);	//CH11
	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,18, 0);	//CH9
	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,19, 0);	//CH7
	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,20, 0);	//CH5

	xil_printf("FT2 start!\r\nFPGA Version:RTS5444V_ft_merge_release_bypass_3a_2021042101\r\n");

	u16 i=0;
	u16 j=0;
	u16 k=0;
	u8 h;

	u8 ADC_CHSel_Prev=0;

	init_platform();

    //initial the axi timer
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR,0,XTC_CSR_AUTO_RELOAD_MASK|XTC_CSR_DOWN_COUNT_MASK); //config the TCSR as down counter, auto reload
    XTmrCtr_EnableIntr(XPAR_AXI_TIMER_0_BASEADDR,0);

    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_1_BASEADDR,0,XTC_CSR_AUTO_RELOAD_MASK|XTC_CSR_DOWN_COUNT_MASK); //config the TCSR as down counter, auto reload
    XTmrCtr_EnableIntr(XPAR_AXI_TIMER_1_BASEADDR,0);

    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_2_BASEADDR,0,XTC_CSR_AUTO_RELOAD_MASK|XTC_CSR_DOWN_COUNT_MASK); //config the TCSR as down counter, auto reload
    XTmrCtr_EnableIntr(XPAR_AXI_TIMER_2_BASEADDR,0);

    //initialize the Gpios
    GpioIniti();

    //initialize spi to Master
    SpiInitiToMaster();

    //no used on RL6558/RL6628 LoadBoard
    //initialize as 7705
    //AD7705Initial(AD7705_DEV0_ADDR);

    //initial SPI flash
    XGpio_0_WriteBit(1,MCM_FLASH_DBG_SEL_OFFSET, 0);  //select the MCM flash that isn't used for debug

    //initial AD7994
    //AD7994_EN_LOW;
    //AD7994_EN_HIGH;

    //register the interrupt
    microblaze_enable_interrupts();
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_0_INTERRUPT_INTR,(XInterruptHandler)uart0_intrq_handle,(void*)NULL); //register the uart interrupt
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_1_INTERRUPT_INTR,(XInterruptHandler)uart1_intrq_handle,(void*)NULL); //register the uart interrupt
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_2_INTERRUPT_INTR,(XInterruptHandler)uart2_intrq_handle,(void*)NULL); //register the uart interrupt
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_3_INTERRUPT_INTR,(XInterruptHandler)uart3_intrq_handle,(void*)NULL); //register the uart interrupt

    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_TIMER_0_INTERRUPT_INTR,(XInterruptHandler)axi_tmr0_intrq_handle,(void*)NULL); //register the axi timer interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_TIMER_1_INTERRUPT_INTR,(XInterruptHandler)axi_tmr1_intrq_handle,(void*)NULL); //register the axi timer interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_TIMER_2_INTERRUPT_INTR,(XInterruptHandler)axi_tmr2_intrq_handle,(void*)NULL); //register the axi timer interrupt

    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_0_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_0_Intrq_Handle,(void*)NULL); //register the GPIO 0 interrupt

    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut0_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT0_1 interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_DUT1_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut1_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT1_1 interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_DUT2_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut2_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT2_1 interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_DUT3_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut3_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT3_1 interrupt

    XIntc_MasterEnable(XPAR_AXI_INTC_0_BASEADDR); //enable intc
    XIntc_MasterEnable(XPAR_AXI_INTC_0_HIGHADDR);

    //load/enable the timer
    TmrIntvlLoad(XPAR_AXI_TIMER_0_BASEADDR,0,10000);
    XTmrCtr_Enable(XPAR_AXI_TIMER_0_BASEADDR, 0);

    TmrIntvlLoad(XPAR_AXI_TIMER_1_BASEADDR,0,10000);
    XTmrCtr_Enable(XPAR_AXI_TIMER_1_BASEADDR, 0);

    TmrIntvlLoad(XPAR_AXI_TIMER_2_BASEADDR,0,700);
    XTmrCtr_Enable(XPAR_AXI_TIMER_2_BASEADDR, 0);

    //enable the GPIO 0 interrupt
    XGpio_0_ReadBit(1,FT2_START_OFFSET); // configure the ft2_start gpio as input
    XGpio_WriteReg(XPAR_AXI_GPIO_0_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_0_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH2_MASK); //enable the GPIO_0 channel 2 interrupt

    //enable the GPIO_DUT0_1/GPIO_DUT1_1/GPIO_DUT2_1/GPIO_DUT3_1 interrupt
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_1_BASEADDR,0,21);     // configure the dut0 ft2_start00 gpio as input
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_DUT1_1_BASEADDR,0,21);// configure the dut1 ft2_start00 gpio as input
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_DUT2_1_BASEADDR,0,21);// configure the dut2 ft2_start00 gpio as input
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_DUT3_1_BASEADDR,0,21);// configure the dut3 ft2_start00 gpio as input

    XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000);      //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT1_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT2_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT3_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable

    XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT1_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT2_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT3_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt

    //enable the each int in the intc
    XIntc_EnableIntr(XPAR_AXI_INTC_0_BASEADDR, XPAR_AXI_TIMER_0_INTERRUPT_MASK|XPAR_AXI_TIMER_1_INTERRUPT_MASK|XPAR_AXI_TIMER_2_INTERRUPT_MASK|XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK
    		|XPAR_AXI_GPIO_1_IP2INTC_IRPT_MASK | XPAR_AXI_GPIO_DUT1_1_IP2INTC_IRPT_MASK | XPAR_AXI_GPIO_DUT2_1_IP2INTC_IRPT_MASK |XPAR_AXI_GPIO_DUT3_1_IP2INTC_IRPT_MASK);
    //XIntc_EnableIntr(XPAR_AXI_INTC_0_BASEADDR, XPAR_AXI_UARTLITE_0_INTERRUPT_MASK|XPAR_AXI_UARTLITE_1_INTERRUPT_MASK|XPAR_AXI_UARTLITE_2_INTERRUPT_MASK|XPAR_AXI_UARTLITE_3_INTERRUPT_MASK);

    for(i=0;i<12;i++)
	{
    	dut0.g_ccdet_calibration_data_buf[i] = 0x88;
  	    dut1.g_ccdet_calibration_data_buf[i] = 0x88;
        dut2.g_ccdet_calibration_data_buf[i] = 0x88;
  		dut3.g_ccdet_calibration_data_buf[i] = 0x88;
	}

	dut0.g_retest = 0;
	dut1.g_retest = 0;
	dut2.g_retest = 0;
	dut3.g_retest = 0;
	dut0.g_ccdet_step = 0;
	dut1.g_ccdet_step = 0;
	dut2.g_ccdet_step = 0;
	dut3.g_ccdet_step = 0;
	dut0.g_ccdet_retest_signbit = 0;
	dut1.g_ccdet_retest_signbit = 0;
	dut2.g_ccdet_retest_signbit = 0;
	dut3.g_ccdet_retest_signbit = 0;

	dut0.g_pattern_timer = 0xff;
	dut1.g_pattern_timer = 0xff;
	dut2.g_pattern_timer = 0xff;
	dut3.g_pattern_timer = 0xff;

	dut0.g_start_smbus_timer = 0xffffffff;
	dut1.g_start_smbus_timer = 0xffffffff;
	dut2.g_start_smbus_timer = 0xffffffff;
	dut3.g_start_smbus_timer = 0xffffffff;

	dut0.g_smbus_writeblock_status = 0;
	dut1.g_smbus_writeblock_status = 0;
	dut2.g_smbus_writeblock_status = 0;
	dut3.g_smbus_writeblock_status = 0;

	dut0.g_start_smbus_retry = 0x2;
	dut1.g_start_smbus_retry = 0x2;
	dut2.g_start_smbus_retry = 0x2;
	dut3.g_start_smbus_retry = 0x2;

	dut0.g_clock_reread_timer = 10;
	dut1.g_clock_reread_timer = 10;
 	dut2.g_clock_reread_timer = 10;
	dut3.g_clock_reread_timer = 10;

	dut0.g_dut_pattern_status_buf[0] = 0x00;
	dut0.g_dut_pattern_status_buf[1] = 0x00;
	dut0.g_dut_pattern_status_buf[2] = 0x00;
	dut0.g_dut_pattern_status_buf[3] = 0x00;
	dut0.g_dut_pattern_status_buf[4] = 0x00;
	dut0.g_dut_pattern_status_buf[5] = 0x00;
	dut0.g_dut_pattern_status_buf[6] = 0x00;
	dut0.g_dut_pattern_status_buf[7] = 0x00;

	dut0.g_smbus_status_buf[0] = 0x00;
	dut0.g_smbus_status_buf[1] = 0xff;
	dut0.g_smbus_style = 0x00;
	dut0.g_smbus_timer = 0xff;
	dut0.g_relay_control_timer = 0xff;

	dut1.g_dut_pattern_status_buf[0] = 0x01;
	dut1.g_dut_pattern_status_buf[1] = 0x00;
	dut1.g_dut_pattern_status_buf[2] = 0x00;
	dut1.g_dut_pattern_status_buf[3] = 0x02;
	dut1.g_dut_pattern_status_buf[4] = 0x00;
	dut1.g_dut_pattern_status_buf[5] = 0x00;
	dut1.g_dut_pattern_status_buf[6] = 0x00;
	dut1.g_dut_pattern_status_buf[7] = 0x00;

	dut1.g_smbus_status_buf[0] = 0x00;
	dut1.g_smbus_status_buf[1] = 0xff;
	dut1.g_smbus_style = 0x00;
	dut1.g_smbus_timer = 0xff;
	dut1.g_relay_control_timer = 0x00;

	dut2.g_dut_pattern_status_buf[0] = 0x01;
	dut2.g_dut_pattern_status_buf[1] = 0x00;
	dut2.g_dut_pattern_status_buf[2] = 0x00;
	dut2.g_dut_pattern_status_buf[3] = 0x02;
	dut2.g_dut_pattern_status_buf[4] = 0x00;
	dut2.g_dut_pattern_status_buf[5] = 0x00;
	dut2.g_dut_pattern_status_buf[6] = 0x00;
	dut2.g_dut_pattern_status_buf[7] = 0x00;

	dut2.g_smbus_status_buf[0] = 0x00;
	dut2.g_smbus_status_buf[1] = 0xff;
	dut2.g_smbus_style = 0x00;
	dut2.g_smbus_timer = 0xff;
	dut2.g_relay_control_timer = 0xff;

	dut3.g_dut_pattern_status_buf[0] = 0x01;
	dut3.g_dut_pattern_status_buf[1] = 0x00;
	dut3.g_dut_pattern_status_buf[2] = 0x00;
	dut3.g_dut_pattern_status_buf[3] = 0x02;
	dut3.g_dut_pattern_status_buf[4] = 0x00;
	dut3.g_dut_pattern_status_buf[5] = 0x00;
	dut3.g_dut_pattern_status_buf[6] = 0x00;
	dut3.g_dut_pattern_status_buf[7] = 0x00;

	dut3.g_smbus_status_buf[0] = 0x00;
	dut3.g_smbus_status_buf[1] = 0xff;
	dut3.g_smbus_style = 0x00;
	dut3.g_smbus_timer = 0xff;
	dut3.g_relay_control_timer = 0xff;

	dut0.g_smbus_getstatus_timer = 0x02;
	dut1.g_smbus_getstatus_timer = 0x02;
	dut2.g_smbus_getstatus_timer = 0x02;
	dut3.g_smbus_getstatus_timer = 0x02;

	dut0.g_vbus_ocp_calibrationdata_buf[0] =0;
	dut0.g_vbus_ocp_calibrationdata_buf[1] =0;
	dut1.g_vbus_ocp_calibrationdata_buf[0] =0;
	dut1.g_vbus_ocp_calibrationdata_buf[1] =0;
	dut2.g_vbus_ocp_calibrationdata_buf[0] =0;
	dut2.g_vbus_ocp_calibrationdata_buf[1] =0;
	dut3.g_vbus_ocp_calibrationdata_buf[0] =0;
	dut3.g_vbus_ocp_calibrationdata_buf[1] =0;

	dut0.g_vbusocp_trimming_low  =0;
	dut0.g_vbusocp_trimming_mid  =0x7f;
	dut0.g_vbusocp_trimming_high =0xff;
	dut1.g_vbusocp_trimming_low  =0;
	dut1.g_vbusocp_trimming_mid  =0x7f;
	dut1.g_vbusocp_trimming_high =0xff;
	dut2.g_vbusocp_trimming_low  =0;
	dut2.g_vbusocp_trimming_mid  =0x7f;
	dut2.g_vbusocp_trimming_high =0xff;
	dut3.g_vbusocp_trimming_low  =0;
	dut3.g_vbusocp_trimming_mid  =0x7f;
	dut3.g_vbusocp_trimming_high =0xff;

	dut0.g_vbusocp_readadc_ready = 0x00;
	dut1.g_vbusocp_readadc_ready = 0x00;
	dut2.g_vbusocp_readadc_ready = 0x00;
	dut3.g_vbusocp_readadc_ready = 0x00;

	//flag used to turn off 3V3_OUT
	dut0.g_ft2_test_done = 0x00;
	dut1.g_ft2_test_done = 0x00;
	dut2.g_ft2_test_done = 0x00;
	dut3.g_ft2_test_done = 0x00;

	dut0.g_test_count=0;
	dut1.g_test_count=0;
	dut2.g_test_count=0;
	dut3.g_test_count=0;

	dut0.g_dut_start_ready = 0;
	dut1.g_dut_start_ready = 0;
	dut2.g_dut_start_ready = 0;
	dut3.g_dut_start_ready = 0;
	dut0.g_uartPatternEnable = 0;
	dut1.g_uartPatternEnable = 0;
	dut2.g_uartPatternEnable = 0;
	dut3.g_uartPatternEnable = 0;
	dut0.g_uartPatternNum = 0x0f;
	dut1.g_uartPatternNum = 0x0f;
	dut2.g_uartPatternNum = 0x0f;
	dut3.g_uartPatternNum = 0x0f;

	//g_vbusocp_3a_status=0;

	while(1)
	{
		dut0.g_dut_pattern_status_buf[2] = (dut0.g_uartPatternNum & 0xff);
		dut1.g_dut_pattern_status_buf[2] = (dut1.g_uartPatternNum & 0xff);
		dut2.g_dut_pattern_status_buf[2] = (dut2.g_uartPatternNum & 0xff);
		dut3.g_dut_pattern_status_buf[2] = (dut3.g_uartPatternNum & 0xff);

		u8 progress_initial_start = 0x0;
		progress_initial_start = (dut0.g_dut_start_ready | dut1.g_dut_start_ready | dut2.g_dut_start_ready | dut3.g_dut_start_ready);

		if(progress_initial_start==0)
		{
			//xil_printf("wait FT start signal...\r\n");
		}
		if(progress_initial_start == 0x11)
		{
			xil_printf("FT start signal start!\r\n");
			platform_initial();
		}

		if(dut0.g_dut_start_ready == 1)
		{
			//if any fail issue//
			if(dut0.g_result_fail != 0x00)
			{
				//break;
				//xil_printf("\r\n dut1_run_fail!\r\n\r\n");
			}
			else
			{
				switch(dut0.g_dut_pattern_status_buf[2])
				{
				//if(g_vbusocp_3a_status== 0x00 | g_vbusocp_3a_status== 0x10)
				//{
					case 0x00:
					{
						_by_Pattern0_Setup_dut0();
						break;
					}
					case 0x01:
					{
						_by_Pattern1_RC_Clock_dut0();
						break;
					}
					case 0x02:
					{
						_by_Pattern2_ADCcalibration_dut0();
						break;
					}
					case 0x03:
					{
						_by_Pattern3_RPcalibration_dut0();
						break;
					}
					case 0x05:
					{
						_by_Pattern5_LDO3V3calibration_dut0();
						break;
					}
					case 0x06:
					{
						_by_Pattern6_CCDet_dut0();
						break;
					}
					case 0x07:
					{
						_by_Pattern7_VBUS_OCP_dut0();
						break;
					}
					case 0x0c:
					{
						_by_Pattern12_efuse_dut0();
						break;
					}
					case 0x0f:
					{
						dut0.g_uartPatternNum = 0x00;
						break;
					}
				//}
				}
			}
		}

		if(dut1.g_dut_start_ready == 1)
	   	{
	   		//if any fail issue//
	   		if(dut1.g_result_fail != 0x00)
		   	{
		   		//break;
	   			//xil_printf("\r\n dut1_run_fail!\r\n\r\n");
		   	}
	   		else
	   		{
	   			//if(g_vbusocp_3a_status== 0x00 | g_vbusocp_3a_status== 0x11)
	   			//{
	   				switch(dut1.g_dut_pattern_status_buf[2])
	   				{
	   				case 0x00:
	   				{
	   					_by_Pattern0_Setup_dut1();
	   					break;
	   				}
	   				case 0x01:
	   				{
	   					_by_Pattern1_RC_Clock_dut1();
	   					break;
	   				}
	   				case 0x02:
	   				{
	   					_by_Pattern2_ADCcalibration_dut1();
	   					break;
	   				}
	   				case 0x03:
	   				{
	   					_by_Pattern3_RPcalibration_dut1();
	   					break;
	   				}
	   				case 0x05:
	   				{
	   					_by_Pattern5_LDO3V3calibration_dut1();
	   					break;
	   				}
	   				case 0x06:
	   				{
	   					_by_Pattern6_CCDet_dut1();
	   					break;
	   				}
	   				case 0x07:
	   				{
	   					_by_Pattern7_VBUS_OCP_dut1();
	   					break;
	   				}
	   				case 0x0c:
	   				{
	   					_by_Pattern12_efuse_dut1();
	   					break;
	   				}
	   				case 0x0f:
	   				{
	   					dut1.g_uartPatternNum = 0x00;
	   					break;
	   				}
	   				}
	   			//}
	   		}
	   	}

		if(dut2.g_dut_start_ready == 1)
		{
			//if any fail issue//
			if(dut2.g_result_fail != 0x00)
			{
				//break;
				//xil_printf("\r\n dut1_run_fail!\r\n\r\n");
			}
			else
			{
				//if(g_vbusocp_3a_status== 0x00 | g_vbusocp_3a_status== 0x12)
				//{
					switch(dut2.g_dut_pattern_status_buf[2])
					{
					case 0x00:
					{
						_by_Pattern0_Setup_dut2();
						break;
					}
					case 0x01:
					{
						_by_Pattern1_RC_Clock_dut2();
						break;
					}
					case 0x02:
					{
						_by_Pattern2_ADCcalibration_dut2();
						break;
					}
					case 0x03:
					{
						_by_Pattern3_RPcalibration_dut2();
						break;
					}
					case 0x05:
					{
						_by_Pattern5_LDO3V3calibration_dut2();
						break;
					}
					case 0x06:
					{
						_by_Pattern6_CCDet_dut2();
						break;
					}
					case 0x07:
					{
						_by_Pattern7_VBUS_OCP_dut2();
						break;
					}
					case 0x0c:
					{
						_by_Pattern12_efuse_dut2();
						break;
					}
					case 0x0f:
					{
						dut2.g_uartPatternNum = 0x00;
						break;
					}
					}
				//}
			}
		}

		if(dut3.g_dut_start_ready == 1)
		{
			//if any fail issue//
			if(dut3.g_result_fail != 0x00)
			{
				//break;
				//xil_printf("\r\n dut1_run_fail!\r\n\r\n");
			}
			else
			{
				//if(g_vbusocp_3a_status== 0x00 | g_vbusocp_3a_status== 0x13)
				//{
					switch(dut3.g_dut_pattern_status_buf[2])
					{
					case 0x00:
					{
						_by_Pattern0_Setup_dut3();
						break;
					}
					case 0x01:
					{
						_by_Pattern1_RC_Clock_dut3();
						break;
					}
					case 0x02:
					{
						_by_Pattern2_ADCcalibration_dut3();
						break;
					}
					case 0x03:
					{
						_by_Pattern3_RPcalibration_dut3();
						break;
					}
					case 0x05:
					{
						_by_Pattern5_LDO3V3calibration_dut3();
						break;
					}
					case 0x06:
					{
						_by_Pattern6_CCDet_dut3();
						break;
					}
					case 0x07:
					{
						_by_Pattern7_VBUS_OCP_dut3();
						break;
					}
					case 0x0c:
					{
						_by_Pattern12_efuse_dut3();
						break;
					}
					case 0x0f:
					{
						dut3.g_uartPatternNum = 0x00;
						break;
					}
					}
				//}
			}
		}

		if((dut0.g_ft2_test_done == 0x00) && (dut1.g_ft2_test_done == 0x00) && (dut2.g_ft2_test_done == 0x00) && (dut3.g_ft2_test_done == 0x00))
		{
			XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x00);
			XGpio_2_WriteBit(0,dut1_FT2_PWR_CTRL_OFFSET,0x00);
			XGpio_2_WriteBit(0,dut2_FT2_PWR_CTRL_OFFSET,0x00);
			XGpio_2_WriteBit(0,dut3_FT2_PWR_CTRL_OFFSET,0x00);

			XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,16, 0);
			XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,16, 0);
			XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,16, 0);
			XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,16, 0);

			//xil_printf("without start signal or ft2 test done, turn off 3V3_OUT!\r\n");
		}
	}

	return 0;
}


