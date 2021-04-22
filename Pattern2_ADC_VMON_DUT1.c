#include "platform.h"
#include "Pattern2_ADC_VMON.h"
#include "adc_vmon_calibration.h"

u8 _by_Pattern2_ADCcalibration_dut1()
{
	u8 i;
	u16 adc_code;
	u8 Buff_dut1_XGPIO_0[8];

	switch(dut1.g_pattern_step)
	{
	//read 0x1045 data and set calibration register signbit
	case 0x0000:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut1.g_pattern_smbus_control_buf[2] = 0x45;
		dut1.g_pattern_smbus_control_buf[3] = 0x10;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			dut1.g_vmon_temp_voltage[0] = dut1.g_pattern_smbus_control_buf[10];
			dut1.g_vmon_temp_voltage[1] = dut1.g_pattern_smbus_control_buf[11];
			//xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
			//xil_printf("dut1.g_vmon_temp_voltage[0] =%x\r\n", dut1.g_vmon_temp_voltage[0]);
			for(i=0; i<31; i++)
			{
				dut1.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			}
			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}
		//xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
		//xil_printf("dut1.g_calibrate_signbit =%x\r\n", dut1.g_calibrate_signbit);
		//xil_printf("dut1.g_calibration_offset =%x\r\n", dut1.g_calibration_offset);
		//xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
		//xil_printf("dut1_pattern5_read_cc1_0xc1a9 = %x\r\n",dut1.g_pattern_smbus_control_buf[10]);
		break;
	}


	//REG_VBUS_COMP_DBC2 reg 0x1045 bit7(AIF:cfg_vbus_comp_en)write1��b1
	case 0x0001:
	{
       dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
	   dut1.g_pattern_smbus_control_buf[2] = 0x45;
	   dut1.g_pattern_smbus_control_buf[3] = 0x10;
	   dut1.g_pattern_smbus_control_buf[4] = 0x01;
	   dut1.g_pattern_smbus_control_buf[5] = (dut1.g_vmon_temp_voltage[0] |(1<<7));

	   //////xil_printf("dut1.g_pattern_smbus_control_buf[5] = %x\r\n",dut1.g_pattern_smbus_control_buf[5]);

	   smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
	   if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	   {
		   break;
	   }
	   else
	   {
			   for(i=1; i<31; i++)
			   {
				   dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }
			       dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			       dut1.g_pattern_step++;
	   }
	   //////xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);



	break;
}


	//read 0x1047 data and set calibration register signbit
	case 0x0002:
	{
		   dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		   dut1.g_pattern_smbus_control_buf[2] = 0x47;
		   dut1.g_pattern_smbus_control_buf[3] = 0x10;
		   dut1.g_pattern_smbus_control_buf[4] = 0x01;

		   smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		   if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   dut1.g_vmon_temp_voltage[2] = dut1.g_pattern_smbus_control_buf[10];
			   //xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
			   //xil_printf("dut1.g_vmon_temp_voltage[2] =%x\r\n", dut1.g_vmon_temp_voltage[2]);

			   //////xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);

			   for(i=0; i<31; i++)
			   {
				   dut1.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			   }
			       dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			       dut1.g_pattern_step++;
			       //dut1.g_pattern_step++;


		   }
		   //////xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
		   //////xil_printf("dut1.g_calibrate_signbit =%x\r\n", dut1.g_calibrate_signbit);
		   //////xil_printf("dut1.g_calibration_offset =%x\r\n", dut1.g_calibration_offset);
	       //////xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
		   //////xil_printf("dut1_pattern5_read_cc1_0xc1a9 = %x\r\n",dut1.g_pattern_smbus_control_buf[10]);
	break;
	}




	   /****************************************
	    *VMON_calibrator Calibration start!
	    *
	    *REG_CC1_REF_SELx/REG_CC2_REF_SELx control
	    *smbus write memeory 0xc1a0 0x00
	    *CC1:0.66V/1.23V
	    *CC2:0.66V/1.23V
	    *CC1 Calibration phy:0x0109
	    *
	    *1.enable VBUS��ѹ��Ƚ����� ��
	    	*REG_VBUS_COMP_DBC2 reg 0x1045 bit7(AIF:cfg_vbus_comp_en)��1��b1;
	    	*REG_VBUS_COMP_CFG2 reg 0x1047 bit2(AIF:REG_POW2)��1'b1;
	    *2.REG_VBUS_COMP_CFG2 reg 0x1047 bit[1:0] (AIF:REG_VMON_SEL)д2'b00, ����ѹ�����е�0.6V��λ
	    *3.VMON_APAD���0.6V ��ѹ
	    *4.�ı�REG_VBUS_COMP_CFG4 reg 0x1049 bit[4:0] (AIF:REG_VTH2_TUNE)���ñȽ��������ת
	    *5.ͨ��REG_VBUS_COMP_DBC2 reg 0x1045 bit5ȷ�ϱȽ��������ת;
	    *6.��REG_VBUS_COMP_CFG4 reg 0x1049 bit[4:0] (AIF:REG_VTH2_TUNE )����滮��efuse:VBUS 0.6V Cal[5:0]
	    *7.REG_VBUS_COMP_CFG2 reg 0x1047 bit[1:0] (AIF:REG_VMON_SEL)д2'b01, ����ѹ�����е�1V��λ,VMON_APAD���1V���ظ�����4~6����trimmingֵд��efuse:VBUS 1V Cal[5:0]
	    *8.REG_VBUS_COMP_CFG2 reg 0x1047 bit[1:0] (AIF:REG_VMON_SEL)д2'b10, ����ѹ�����е�2.5V��λ,VMON_APAD���2.5V���ظ�����4~6����trimmingֵд��efuse:VBUS 2.5V Cal[5:0]
	    *9.REG_VBUS_COMP_CFG2 reg 0x1047 bit[1:0] (AIF:REG_VMON_SEL)д2'b11, ����ѹ�����е�3V��λ,VMON_APAD���3V���ظ�����4~6����trimmingֵд��efuse:VBUS 3V Cal[5:0]
	    *
	    *
	    ******************************************/
	case 0x0003:
	{
		dut1.g_vmon_calib_level = vmon_calib_600mv;
		dut1.g_vmon_calib_voltage = 600;
		//i2c_max5825_output(SMBUS_DEV0_ADDR,MAX5825_DEV0_SLAVE_ADDR,5,dut1.g_vmon_calib_voltage);
		_vmon_calibration_dut1();

		//xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
		//xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		if(dut1.g_vmoncompare_process_bit != vmon_process_done_pass)
		{

		}
		else
		{
			dut1.g_vmon_writeefuse_600mv[0] = dut1.g_vmon_calibration_data_buf[2];
			dut1.g_vmon_writeefuse_600mv[1] = dut1.g_vmon_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_vmon_calibration_data_buf[i] = 0;
			}
			dut1.g_vmoncompare_process_bit = vmon_process_waiting;
			dut1.g_vmon_compare_step = 0;
			dut1.g_pattern_step++;
			//dut1.g_result_fail = 0x01;
			//xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
			//xil_printf("dut1_pattern5_CC1_0.66V_Calibration\r\n");
			//xil_printf("dut1.g_vmon_writeefuse_600mv[0] = %x\r\n",dut1.g_vmon_writeefuse_600mv[0]);
			//xil_printf("dut1.g_vmon_writeefuse_600mv[1] = %x\r\n",dut1.g_vmon_writeefuse_600mv[1]);
		}
		//xil_printf("dut1_pattern2_adcvmon_0.6v_calibration_pass!\r\n");
		break;
	}


	case 0x0004:
	{
		dut1.g_vmon_calib_level = vmon_calib_1000mv;
		dut1.g_vmon_calib_voltage = 1000;
		//i2c_max5825_output(SMBUS_DEV0_ADDR,MAX5825_DEV0_SLAVE_ADDR,5,dut1.g_vmon_calib_voltage);
		_vmon_calibration_dut1();

		//xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
		//xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		if(dut1.g_vmoncompare_process_bit != vmon_process_done_pass)
		{

		}
		else
		{
			dut1.g_vmon_writeefuse_1000mv[0] = dut1.g_vmon_calibration_data_buf[2];
			dut1.g_vmon_writeefuse_1000mv[1] = dut1.g_vmon_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_vmon_calibration_data_buf[i] = 0;
			}
			   dut1.g_vmoncompare_process_bit = vmon_process_waiting;
			   dut1.g_vmon_compare_step = 0;;
			   dut1.g_pattern_step++;

			////xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
			//xil_printf("dut1_pattern5_CC1_0.66V_Calibration\r\n");
			//xil_printf("dut1.g_vmon_writeefuse_1000mv[0] = %x\r\n",dut1.g_vmon_writeefuse_1000mv[0]);
			//xil_printf("dut1.g_vmon_writeefuse_1000mv[1] = %x\r\n",dut1.g_vmon_writeefuse_1000mv[1]);
		}
			//xil_printf("dut1_pattern2_adcvmon_Calibration\r\n");
		    break;
	}

	case 0x0005:
	{
		dut1.g_vmon_calib_level = vmon_calib_2500mv;
		dut1.g_vmon_calib_voltage = 2500;
		//i2c_max5825_output(SMBUS_DEV0_ADDR,MAX5825_DEV0_SLAVE_ADDR,5,dut1.g_vmon_calib_voltage);
		_vmon_calibration_dut1();

		//xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
		//xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		if(dut1.g_vmoncompare_process_bit != vmon_process_done_pass)
		{

		}
		else
		{
			dut1.g_vmon_writeefuse_2500mv[0] = dut1.g_vmon_calibration_data_buf[2];
			dut1.g_vmon_writeefuse_2500mv[1] = dut1.g_vmon_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_vmon_calibration_data_buf[i] = 0;
			}
			   dut1.g_vmoncompare_process_bit = vmon_process_waiting;
			   dut1.g_vmon_compare_step = 0;;
			   dut1.g_pattern_step++;

			////xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
			//xil_printf("dut1_pattern5_CC1_0.66V_Calibration\r\n");
			//xil_printf("dut1.g_vmon_writeefuse_2500mv[0] = %x\r\n",dut1.g_vmon_writeefuse_2500mv[0]);
			//xil_printf("dut1.g_vmon_writeefuse_2500mv[1] = %x\r\n",dut1.g_vmon_writeefuse_2500mv[1]);
		}
			//xil_printf("dut1_pattern2_adcvmon_Calibration\r\n");
		    break;
	}

	case 0x0006:
	{
		dut1.g_vmon_calib_level = vmon_calib_3000mv;
		dut1.g_vmon_calib_voltage = 3000;
		//i2c_max5825_output(SMBUS_DEV0_ADDR,MAX5825_DEV0_SLAVE_ADDR,5,dut1.g_vmon_calib_voltage);
		_vmon_calibration_dut1();

		//xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
		//xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		if(dut1.g_vmoncompare_process_bit != vmon_process_done_pass)
		{

		}
		else
		{
			dut1.g_vmon_writeefuse_3000mv[0] = dut1.g_vmon_calibration_data_buf[2];
			dut1.g_vmon_writeefuse_3000mv[1] = dut1.g_vmon_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_vmon_calibration_data_buf[i] = 0;
			}
			   dut1.g_vmoncompare_process_bit = vmon_process_waiting;
			   dut1.g_vmon_compare_step = 0;;
			   dut1.g_pattern_step++;

			////xil_printf("dut1.g_vmon_compare_step = %x\r\n",dut1.g_vmon_compare_step);
			//xil_printf("dut1_pattern5_CC1_0.66V_Calibration\r\n");
			//xil_printf("dut1.g_vmon_writeefuse_3000mv[0] = %x\r\n",dut1.g_vmon_writeefuse_3000mv[0]);
			//xil_printf("dut1.g_vmon_writeefuse_3000mv[1] = %x\r\n",dut1.g_vmon_writeefuse_3000mv[1]);
		}
			//xil_printf("dut1_pattern2_adcvmon_Calibration\r\n");
		    break;
	}

	case 0x0007:
	{
		if((dut1.g_vmon_writeefuse_600mv[0]>vmon_writeefuse_600mv_min)&&(dut1.g_vmon_writeefuse_600mv[0]<vmon_writeefuse_600mv_max))
		{
			dut1.g_pattern_step++;
		}
		else
		{
			dut1.g_result_fail = 0x01;
			xil_printf("dut1_g_vmon_writeefuse_600mv fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0008:
	{
		if((dut1.g_vmon_writeefuse_1000mv[0]>vmon_writeefuse_1000mv_min)&&(dut1.g_vmon_writeefuse_1000mv[0]<vmon_writeefuse_1000mv_max))
		{
			dut1.g_pattern_step++;
		}
		else
		{
			dut1.g_result_fail = 0x01;
			xil_printf("dut1_g_vmon_writeefuse_1000mv fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0009:
	{
		if((dut1.g_vmon_writeefuse_2500mv[0]>vmon_writeefuse_2500mv_min)&&(dut1.g_vmon_writeefuse_2500mv[0]<vmon_writeefuse_2500mv_max))
		{
			dut1.g_pattern_step++;
		}
		else
		{
			dut1.g_result_fail = 0x01;
			xil_printf("dut1_g_vmon_writeefuse_2500mv fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x000a:
	{
		if((dut1.g_vmon_writeefuse_3000mv[0]>vmon_writeefuse_3000mv_min)&&(dut1.g_vmon_writeefuse_3000mv[0]<vmon_writeefuse_3000mv_max))
		{
			dut1.g_pattern_step++;
		}
		else
		{
			dut1.g_result_fail = 0x01;
			xil_printf("dut1_g_vmon_writeefuse_3000mv fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x000b:
	{

			for(i=1; i<31; i++)
		   {
			   dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
		   }
	   	       dut1.g_pattern_timer = 0xfff;
			   dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		       dut1.g_pattern_step = 0x00;
		       dut1.g_dut_pattern_status_buf[2]++;
		       dut1.g_uartPatternEnable = 0x00;
		       dut1.g_uartPatternNum = (dut1.g_uartPatternNum++ & 0xff);
		       //dut1.g_uartPatternNum = (dut1.g_uartPatternNum++ & 0xff);
		       //dut1.g_uartPatternNum = (dut1.g_uartPatternNum++ & 0xff);
		       //dut1.g_uartPatternNum = (dut1.g_uartPatternNum++ & 0xff);
		       result_output_for_v50(XPAR_AXI_GPIO_dut1_1_BASEADDR,dut1.g_uartPatternNum);
			   xil_printf("dut1_pattern2_adc_vmon_calibration_pass!\r\n");
			   xil_printf("vmon_calibration_0.6v_data= 0x%x 1v_data= 0x%x 2.5v_data= 0x%x 3v_data= 0x%x\r\n\r\n", dut1.g_vmon_writeefuse_600mv[0],dut1.g_vmon_writeefuse_1000mv[0],dut1.g_vmon_writeefuse_2500mv[0],dut1.g_vmon_writeefuse_3000mv[0]);
		       break;
	}


	}
	if(dut1.g_result_fail == 0x01)
	{
		dut1.g_ft2_test_done = 0x00;
		pattern_safe_state_dut1();	//add safe state
		xil_printf("dut1_pattern2_fail! fail_step = %x\r\n\r\n",dut1.g_pattern_step);
	}
	//xil_printf("dut1_pattern2_adcvmon_step = %x\r\n",dut1.g_pattern_step);
	return 0;
}


