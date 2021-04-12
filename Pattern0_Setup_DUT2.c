#include "platform.h"

u8 _by_Pattern0_Setup_dut2()
{
	u8 i;
	u8 j;
	u8 Buff_dut2_XGPIO_0[10];
	u16 adc_code;
	u8 read_icstatus_data[15];
	u8 efuse_datavalid;
	u8 efuse_testvalid;
	u8 efuse_data_valid;
	u8 efuse_ldo3v3_valid;
	u8 efuse_rp12k_valid;
	u8 efuse_cc_valid;
	u8 efuse_package_valid;
	u8 efuse_writedata_valid;
	u32 start_smbus_runtime_count;
	u8 START01_GPIO22;
	u32 ft1_config_scan=0;
	u8 ft1_config_scan_count=0x1ff;
	u8 gpio26_output_value=0;

	u8  smbus_writedata_count;
	u8  smbus_writecmd_count;
	u8  smbus_writecmd_buf[30];
	u8  smbus_get_pinstatus_result = dut2.g_smbus_status_buf[2];

	//u32 gpio_data;
	//u32 gpio_tri;

	switch(dut2.g_pattern_step)
	{
	//write pattern index and power on IC
	case 0x0000:
	{
		dut2.g_uartPatternNum = 0;

		//gpio17: low bit, gpio20: high bit
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,17, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,18, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,19, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,20, 0);
		msdelay(5);

		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,15, 0);
		msdelay(5);

		XGpio_2_WriteBit(0,dut2_FT2_PWR_CTRL_OFFSET,0x01);	//S0-PWR_EN: ����S0-ATX_5V�Ƿ񿪳�S0-5V��δ�����������裩
		msdelay(5);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,1, 1);	//S0-PWR_EN_2: ����S0-5V�Ƿ񿪳�S0-5V_IN/S0-VCON_IN
		msdelay(5);

		//gpio_data = XGpio_ReadReg(XPAR_AXI_GPIO_dut2_1_BASEADDR,XGPIO_DATA_OFFSET);
		//gpio_tri = XGpio_ReadReg(XPAR_AXI_GPIO_dut2_1_BASEADDR,XGPIO_TRI_OFFSET);
		//xil_printf("gpio_data = %x\r\n", gpio_data);
		//xil_printf("gpio_tri = %x\r\n", gpio_tri);

		xil_printf("dut2 IC power on!\r\n\r\n");
		dut2.g_pattern_step++;
	}

	case 0x0001:
	{
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,16, 1);	//enable power switch
		msdelay(50);

		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xf0;
		dut2.g_pattern_smbus_control_buf[3] = 0x02;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0xaa;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			if(dut2.g_start_smbus_retry >0)
			{
				smbus_writedata_count = dut2.g_pattern_smbus_control_buf[4];
				smbus_writecmd_count = smbus_writedata_count + 5;
				smbus_writecmd_buf[0] = dut2.g_pattern_smbus_control_buf[1];
				smbus_writecmd_buf[1] = smbus_writedata_count+3;
				smbus_writecmd_buf[2] = dut2.g_pattern_smbus_control_buf[2];
				smbus_writecmd_buf[3] = dut2.g_pattern_smbus_control_buf[3];
				smbus_writecmd_buf[4] = dut2.g_pattern_smbus_control_buf[4];

				for(i=0; i<smbus_writedata_count; i++)
				{
					smbus_writecmd_buf[i+5] = dut2.g_pattern_smbus_control_buf[i+5];
				}
				//for(i=0;i<10;i++)
				//{
					msdelay(10);
					smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, smbus_writecmd_count);
					msdelay(25);
					smbus_get_pinstatus_result = smbus_master_get_pinstate_dut2(SMBUS_DEV2_ADDR, smbus_get_pinstatus_result);
					msdelay(10);

					if(smbus_get_pinstatus_result == 1)
					{
						xil_printf("dut2_start_smbus_write_pass!\r\n\r\n");

						for(i=1; i<30; i++)
						{
							dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
						}

						dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
						dut2.g_pattern_step++;
						return;
					}
					//xil_printf("dut2.i = %x\r\n\r\n",i);
			   //}
			   //smbus_get_pinstatus_result = smbus_master_get_pinstate_dut2(SMBUS_DEV2_ADDR, smbus_get_pinstatus_result);
			   //xil_printf("dut2.smbus_get_pinstatus_result = %x\r\n",smbus_get_pinstatus_result);
			   dut2.g_start_smbus_retry--;
			}
		}

		else
		{
			for(i=1; i<30; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}

		break;
	}

	//write register 0x02f0 data 0xaa
	case 0x0002:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xf0;
		dut2.g_pattern_smbus_control_buf[3] = 0x02;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0xaa;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<30; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
			msdelay(50);
		}

		break;
	}

	//get IC information
	case 0x0003:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
		dut2.g_pattern_smbus_control_buf[2] = 0x00;
		dut2.g_pattern_smbus_control_buf[3] = 0x00;
		dut2.g_pattern_smbus_control_buf[4] = 0x0A;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<11;i++)
			{
				read_icstatus_data[i] = dut2.g_pattern_smbus_control_buf[i+10];
			}

			if((read_icstatus_data[0] == 0x01) && (read_icstatus_data[6] == 0xDA) && (read_icstatus_data[7] == 0x0B) && (read_icstatus_data[9] == 0x65) && (read_icstatus_data[8] == 0x58))
			{
				dut2.g_pattern_step++;
			}
			else
			{
				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
				xil_printf("dut2 get IC information fail!!!\r\n\r\n");
			}

			for(i=0; i<30; i++)
			{
				dut2.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}

		break;
	}

	//efuse_clock_enable
	case 0x0004:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x05;
		dut2.g_pattern_smbus_control_buf[3] = 0x11;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x07;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<30; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}

		break;
	}

	//Read Efuse data validbit
	case 0x0005:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut2.g_pattern_smbus_control_buf[2] = 0x00;
		dut2.g_pattern_smbus_control_buf[3] = 0x00;
		dut2.g_pattern_smbus_control_buf[4] = 0x02;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			//xil_printf("dut2.g_pattern_smbus_control_buf[10] =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
			//xil_printf("dut2.g_pattern_smbus_control_buf[11] =%x\r\n", dut2.g_pattern_smbus_control_buf[11]);
			efuse_datavalid = dut2.g_pattern_smbus_control_buf[10] & 0x01;
			efuse_testvalid = dut2.g_pattern_smbus_control_buf[11]>>0x07;

			efuse_data_valid    =  dut2.g_pattern_smbus_control_buf[10]&0x01;
			efuse_ldo3v3_valid  = (dut2.g_pattern_smbus_control_buf[10]>>1)&0x01;
			efuse_rp12k_valid   = (dut2.g_pattern_smbus_control_buf[10]>>2)&0x01;
			efuse_cc_valid      = (dut2.g_pattern_smbus_control_buf[10]>>3)&0x01;
			efuse_package_valid = (dut2.g_pattern_smbus_control_buf[10]>>4)&0x03;
			efuse_writedata_valid = (efuse_data_valid && efuse_ldo3v3_valid && efuse_rp12k_valid && efuse_cc_valid);

			while((ft1_config_scan!=0x0)|(ft1_config_scan!=0xffffffff))
			{
				//XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,26, ~gpio26_output_value);
				START01_GPIO22 = XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,22);
				usdelay(1);
				ft1_config_scan = ((ft1_config_scan<<1)|START01_GPIO22);
				ft1_config_scan_count--;
				if(ft1_config_scan_count == 0)
				{
					break;
				}
			}

			usdelay(20);
			xil_printf("dut2_START01_GPIO22 = %x\r\n\r\n",START01_GPIO22);
			//xil_printf("dut2_ft1_config_scan = %x\r\n\r\n",ft1_config_scan);
			if(efuse_package_valid==0)
			{
				dut2.g_retest = 0;
				dut2.g_efuse_status = EFUSE_FT1_TEST;
				dut2.g_dut_pattern_status_buf[2]++;
				dut2.g_uartPatternEnable = 0x00;
				dut2.g_uartPatternNum = (0x0c & 0xff);
				//dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
				xil_printf("dut2 -> this is a new ic without package code, go to pattern12!\r\n\r\n");
			}
			else if((efuse_package_valid!=0) && (efuse_writedata_valid==0))
			{
				dut2.g_retest = 0;
				if(ft1_config_scan == 0xffffffff)
				{
					//xil_printf("START01_GPIO2222 = %x\r\n\r\n",START01_GPIO22);
					dut2.g_efuse_status = EFUSE_FT1_TEST;
					dut2.g_dut_pattern_status_buf[2]++;
					dut2.g_uartPatternEnable = 0x00;
					dut2.g_uartPatternNum = (0x0c & 0xff);
					//dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
					//xil_printf("dut2.g_efuse_status = %x\r\n\r\n",dut2.g_efuse_status);
					xil_printf("dut2 -> this is a new ic with package code, but gpio22 is high, go to pattern12!\r\n\r\n");
				}
				else
				{
					//xil_printf("START01_GPIO2233 = %x\r\n\r\n",START01_GPIO22);
					dut2.g_efuse_status = EFUSE_FT2_TEST;
					dut2.g_dut_pattern_status_buf[2]++;
					dut2.g_uartPatternEnable = 0x00;
					dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
					//dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
					xil_printf("dut2 -> this is a new ic with package code and gpio22 is low, go to pattern1!\r\n\r\n");
				}
			}
			else if((efuse_package_valid!=0) && (efuse_writedata_valid==1))
			{
				dut2.g_retest = 1;
				if(ft1_config_scan == 0xffffffff)
				{
					dut2.g_efuse_status = EFUSE_FT1_TEST;
					dut2.g_dut_pattern_status_buf[2]++;
					dut2.g_uartPatternEnable = 0x00;
					dut2.g_uartPatternNum = (0x0c & 0xff);
					//dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
					xil_printf("dut2 -> this is a retest ic and gpio22 is high, go to pattern12!\r\n\r\n");
				}
				else
				{
					dut2.g_efuse_status = EFUSE_RETEST;
					dut2.g_dut_pattern_status_buf[2]++;
					dut2.g_uartPatternEnable = 0x00;
					dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
					//dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
					xil_printf("dut2 -> this is a retest ic and gpio22 is low, go to pattern1!\r\n\r\n");
				}
			}
			else
			{
				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
				dut2.g_dut_pattern_status_buf[2]++;
				dut2.g_uartPatternEnable = 0x00;
				dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
			}

			for(i=1; i<31; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_timer = 0xfff;
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step = 0x00;

			result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
			xil_printf("dut2_pattern0_setup_pass!     efuse_testvalid =0x%x\r\n\r\n",dut2.g_efuse_status);
			//xil_printf("dut2.g_uartPatternNum = %x\r\n\r\n",dut2.g_uartPatternNum);
			//xil_printf("dut2.g_pattern_step = %x\r\n\r\n",dut2.g_pattern_step);
			//xil_printf("dut2.g_dut_start_ready = %x\r\n\r\n",dut2.g_dut_start_ready);
			//xil_printf("dut2.g_result_fail = %x\r\n\r\n",dut2.g_result_fail);
			//xil_printf("dut2.g_dut_pattern_status_buf[2] = %x\r\n\r\n",dut2.g_dut_pattern_status_buf[2]);
			//xil_printf("dut2.g_efuse_status = %x\r\n\r\n",dut2.g_efuse_status);
		}

		break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		dut2.g_ft2_test_done = 0x00;
		pattern_safe_state_dut2();	//add safe state
		xil_printf("dut2_pattern0_fail! fail_step = %x\r\n\r\n",dut2.g_pattern_step);
	}

	//xil_printf("dut2_pattern0_setup_step = %x\r\n\r\n",dut2.g_pattern_step);
	//xil_printf("dut2.g_result_fail = %x\r\n\r\n",dut2.g_result_fail);
	//xil_printf("dut2.g_uartPatternNum = %x\r\n\r\n",dut2.g_uartPatternNum);
	//xil_printf("dut2.g_uartPatternEnable = %x\r\n\r\n",dut2.g_uartPatternEnable);
	return 0;
}


