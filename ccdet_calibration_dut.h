/*
 * Copyright (c) 2008 Xilinx, Inc.  All rights reserved.
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

#ifndef _CCDET_Calibration_DUT_H_
#define _CCDET_Calibration_DUT_H_



#include <math.h>
#include <stdio.h>

//xilix system header file
#include "platform_config.h"
#include "xparameters.h"
#include "xspi.h"
#include "xuartlite_l.h"
#include "xtmrctr_l.h"
#include "xgpio_l.h"
#include "xintc_l.h"
#include "xiic_l.h"
#include "state_define.h"

//user define header file
#include "ad7705_drv.h"
#include "ad7994_drv.h"
#include "gpio_drv.h"
#include "dac_5825_4822_drv.h"
#include "spi_drv.h"
#include "spi_flash_drv.h"
#include "timer_drv.h"
#include "uart_drv.h"
#include "var_initial.h"
#include "smbus_drv.h"
#include "smbus_irq_handle.h"



#define lower_voltage_retest             0x22
#define higer_voltage_retest             0x11
#define ccdet_process_waiting	         0x00
#define ccdet_process_doing	             0x01
#define ccdet_process_done_pass	         0x3f
#define ccdet_process_done_fail	         0x1f


//extern global statement

u8 _ccdet_calibration_dut0();
u8 _ccdet_calibration_dut1();
u8 _ccdet_calibration_dut2();
u8 _ccdet_calibration_dut3();




#endif
