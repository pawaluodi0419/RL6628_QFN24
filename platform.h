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

#ifndef __PLATFORM_H_
#define __PLATFORM_H_

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
#include "platform_initial.h"
#include "ad7705_drv.h"
#include "ad7994_drv.h"
#include "adc_vmon_calibration.h"
#include "ccdet_calibration_dut.h"
#include "dac_5825_4822_drv.h"
#include "gpio_drv.h"
#include "smbus_drv.h"
#include "spi_drv.h"
#include "spi_flash_drv.h"
#include "state_define.h"
#include "timer_drv.h"
#include "uart_drv.h"
#include "var_initial.h"
#include "vbus_ocp_calibration.h"

#include "Pattern0_Setup_DUT.h"
#include "Pattern1_RCClock.h"
#include "Pattern2_ADC_VMON.h"
#include "Pattern3_RP.h"
#include "Pattern5_LDO3V3.h"
#include "Pattern6_CCDet.h"
#include "Pattern7_VBUS_OCP.h"


//extern global statement

#define XPAR_AXI_GPIO_dut0_1_BASEADDR   XPAR_AXI_GPIO_1_BASEADDR
#define EFUSE_FT1_TEST            1
#define EFUSE_FT2_TEST            2
#define EFUSE_RETEST              3
#define QFN12_Package_Num         1
#define QFN20_Package_Num         2
#define QFN24_Package_Num         3
u8 g_vbusocp_3a_status;

struct DUT{
u8  g_dut_pattern_status_buf[40];
u8  g_FT2StartSignal;
u8  g_dut_start_ready;
u8  g_patern_number;
u8  g_dut_result_status;
u8  g_result_fail;
u32  g_result_fail_tmrcount;
u8 g_result_output_ch;
u32 g_test_count;
u8 g_start00_pulsh_count;
u8  g_calibrate_signbit;
u8  g_calibration_offset;

u8  g_pattern_smbus_control_buf[31];
u8  g_efuse_status;
u8  g_efuse_temp_buf[30];
u16 g_pattern_step;
u8  g_efuse_data_buf[30];
u8  g_efuse_valid_ldo3v3;
u8  g_efuse_data_ldo3v3;
u16 g_clock_frq_cnt;


u8  g_smbus_fpga_baseaddr;
u8  g_smbus_timer;
u8  g_smbus_process_state;
u8  g_smbus_style;
u8  g_smbus_retry_timer;
u8  g_smbus_getstatus_timer;
u8  g_smbus_status_buf[3];
u8  g_dut_start_state;
u8 g_smbus_writeblock_status;

u16  g_relay_control_timer;
u8  g_clock_reread_timer;
u32 g_start_smbus_timer;
u8 g_start_smbus_retry;
u32 g_start_smbus_timer_count1;
u32 g_start_smbus_timer_count2;

u32 g_pattern_timer;

u8 g_vmoncompare_process_bit;
u8 g_vmon_compare_step;
u32 g_vmon_calib_voltage;
u8 g_vmon_temp_voltage[5];
u8 g_vmon_calib_level;
u8 g_vmon_dac_tempdata_buf[8];
u8 g_vmon_calibration_data_buf[4];
u8 g_vmon_retest_signbit;
u8 g_vmon_writeefuse_600mv[2];
u8 g_vmon_writeefuse_1000mv[2];
u8 g_vmon_writeefuse_2500mv[2];
u8 g_vmon_writeefuse_3000mv[2];

u8 g_vbusocp_process_bit;
u8 g_vbusocp_step;
u8 g_vbusocp_pre_flt_status;
u8 g_vbusocp_readadc_ready;
u8 vbus_load_retry_time;
u8 vbus_calibraion_ready_retry_time;
u8 g_vbusocp_trimming_low;
u8 g_vbusocp_trimming_high;
u8 g_vbusocp_trimming_mid;
u8 g_vbusocp_retry_time;
u8 g_vbusocp_3a_en_status;
u8 g_vbusocp_board_en;
u8 g_vbusocp_0p9a_nmos_control;
u8 g_vbusocp_1p5a_nmos_control;
u8 g_vbusocp_3a_nmos_control;
u8 g_vbus_ocp_calibrationdata_buf[2];
u8 g_vbusocp_writeefuse_0p9a;
u8 g_vbusocp_writeefuse_1p5a;
u8 g_vbusocp_writeefuse_3a;
u8 g_ft2_test_done;

u8 	g_uartRecBuf[4];
u8 	g_uartRecBufNum;
u8 	g_uartRecDataBuf;
u8 	g_uartRecAddrBufL;
u8 	g_uartRecAddrBufH;
u8 	g_uartStartReadFlag;
u8	g_uartStartWriteFlag;
u8 	g_uartRecAddrBufCnt;
u8 	g_uartRecDataBufNum;
u8 	g_uartWriteValidFlg;
u8 	g_uartReadValidFlg;
u16 g_uartRecAddrBuf;
u8 	g_uartLnkChk;
u8 	g_uartTranDataBuf;
u8  g_uartPatternNum;
u8  g_uartStartAck;
u8  g_uartPatternEnable;

u16 g_axiTmr0Cnt;
u16 g_axiTmr1Cnt;
u16 g_axiTmr2Cnt;



u8 	g_trigSendFT2ResultFlg;
u8 	g_ft2Result;
u8 	g_ft2ResultCnt;
u8 	g_ft2StartFlg;
u8 	g_ft2StartReLoadFlg;
u16 g_ft2StartReloadCnt;

u8 	g_adcOpaFlg;
u8 	g_adcChSel;
u16	g_adcData;
u16 g_adc_1v_code;
u16 g_adc_0p2v_code;
u32 g_adc_a;
u32 g_adc_b_signbit;
u32 g_adc_b_offset;
u8  g_retest;




u8 g_dac_data_buf[6]; // not initial in var_initial
u8 g_dacOpaFlg;
u8 g_dacChSel;
u8 g_dacGainSel;
u8 g_dacTransDataL;
u8 g_dacTransDataH;

u8 g_flashDLStart;
u8 g_flashSize;
u8 g_flashMCMEraseEn;
u8 g_flashMCMRdAccChkEn;
u8 g_flashRdCompEn;
u8 g_flashRdCompResult;
u32 g_flashMCMRdAccValue;

u8    g_ad7994ConversionStart;
u8    g_ad7994ChSel;
u8    g_ad7994RrevCh; // initial the register with a value that different from g_ad7994ChSel
u8    g_ad7994DataLowByte;
u8    g_ad7994DataHighByte;
u8    g_ad7994AvgPionts;
u16   g_as7994Cvsdata;


u16  g_ccdet_step;
u8  g_ccdet_process_bit;
u8  g_ccdet_voltage_sel;
//u8  g_dac_data_buf[8]; define up
u8  g_ccdet_calibration_data_buf[12];
u16 g_ccdet_phy_calibration_addr_buf[2];
u8  g_ccdet_phy_calibration_bit;
u8  g_ccdet_ana_result_bit;
u8  g_ccdet_retest_signbit;

};

extern u8 g_opaLedTg;
extern u8 g_ledD2Tg;
extern u8 g_ledD3Tg;
extern u8 g_ledD4Tg;
extern u8 g_ledD5Tg;
extern u8 g_ledD6Tg;
extern u8 g_ledD7Tg;

extern u32 g_main_count;

extern struct DUT dut0;
extern struct DUT dut1;
extern struct DUT dut2;
extern struct DUT dut3;


void init_platform();
void cleanup_platform();

#endif
