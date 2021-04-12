#include "platform.h"
#include "PatternC_Efuse.h"

u8 _by_Pattern12_efuse_dut3()
{
	u8 i;
	u8 j;
	u8 Buff_dut3_XGPIO_0[8];
	u32 adc_a;
	u32 adc_b;
	u8  adc_a_h;
	u8  adc_a_l;
	u32 adc_full_voltage;
	u8 efuse_data_valid;
	u8 efuse_testvalid;
	u8 efuse_compare_bit;
	u8 efuse_ldo3v3_valid;
	u8 efuse_rp12k_valid;
	u8 efuse_cc_valid;
	u8 efuse_package_valid;
	u8 efuse_writedata_valid;
	u8 START01_GPIO22;

	u8 efuse_read_buf[40];
	u8 efuse_ldo3v3_tolerance;
	u8 efuse_rp12k_tolerance;
	u8 efuse_vbus0p6v_tolerance;
	u8 efuse_vbus1v_tolerance;
	u8 efuse_vbus2p5v_tolerance;
	u8 efuse_vbus3v_tolerance;
	u8 efuse_cfg0_l_tolerance;
	u8 efuse_cfg0_h_tolerance;
	u8 efuse_cfg1_l_tolerance;
	u8 efuse_cfg1_h_tolerance;
	u8 efuse_cfg2_l_tolerance;
	u8 efuse_cfg2_h_tolerance;
	u8 efuse_cfg3_l_tolerance;
	u8 efuse_cfg3_h_tolerance;
	u8 efuse_vbusocp_3a_tolerance;
	u8 efuse_vbusocp_1p5a_tolerance;
	u8 efuse_vbusocp_0p9a_tolerance;
	u8 efuse_rc_clock_tolerance;
	u8 efuse_tpp_l_tolerance;
	u8 offline_loop_run;

	u8 RCClock_Tolerance_Compare_Data=5;
	u8 VMON_Tolerance_Compare_Data=3;
	u8 RP12K_Tolerance_Compare_Data=3;
	u8 LDO3V3_Tolerance_Compare_Data=3;
	u8 CCDET_Tolerance_Compare_Data=5;
	u8 VBUS_3A_Tolerance_Compare_Data=5;
	u8 VBUS_1P5A_Tolerance_Compare_Data=5;
	u8 VBUS_0P5A_Tolerance_Compare_Data=5;
	u8 Loop_run_enable=0;


	   /******************************************************************************

	  			    *
	  		        *
	  		        *
	  		        *
	  		        *
	  ********************************************************************************/

	//////////////xil_printf("dut3_pattern0_start!\r\n");


	switch(dut3.g_pattern_step)
	{
	//Read Efuse data validbit


	case 0x0000:
	{

		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x00;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x02;

		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   ////////////////xil_printf("dut3.g_pattern_smbus_control_buf[10] =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
			   ////////////////xil_printf("dut3.g_pattern_smbus_control_buf[11] =%x\r\n", dut3.g_pattern_smbus_control_buf[11]);

			   efuse_data_valid    =  dut3.g_pattern_smbus_control_buf[10]&0x01;
			   efuse_ldo3v3_valid  = (dut3.g_pattern_smbus_control_buf[10]>>1)&0x01;
			   efuse_rp12k_valid   = (dut3.g_pattern_smbus_control_buf[10]>>2)&0x01;
			   efuse_cc_valid      = (dut3.g_pattern_smbus_control_buf[10]>>3)&0x01;
			   efuse_package_valid = (dut3.g_pattern_smbus_control_buf[10]>>4)&0x03;
			   efuse_writedata_valid = (efuse_data_valid && efuse_ldo3v3_valid && efuse_rp12k_valid && efuse_cc_valid);

			   START01_GPIO22 = XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,22);
			   usdelay(10);
			   if((efuse_package_valid==0) && (efuse_writedata_valid==0))
			   {
				   dut3.g_retest = 0;
				   dut3.g_efuse_status = EFUSE_FT1_TEST;
			       dut3.g_pattern_step = 0x0200;
			       //dut3.g_pattern_step++;
			       xil_printf("dut3 -> this is a new ic without package code, go to write package code!\r\n\r\n");

			   }
			   else if((efuse_package_valid!=0) && (efuse_writedata_valid==0))
			   {
				   dut3.g_retest = 0;
				   if(START01_GPIO22 == 1)
				   {
					   ////xil_printf("START01_GPIO2222 = %x\r\n\r\n",START01_GPIO22);
					   dut3.g_efuse_status = EFUSE_FT1_TEST;
				       dut3.g_pattern_step = 0x0200;
				       //dut3.g_pattern_step++;
				       xil_printf("dut3 -> this is a new ic with package code, but gpio22 is high, go to write package code!\r\n\r\n");
				   }
				   else
				   {
					   ////xil_printf("START01_GPIO2233 = %x\r\n\r\n",START01_GPIO22);
					   dut3.g_efuse_status = EFUSE_FT2_TEST;
				       //dut3.g_pattern_step = 0x0d;
				       dut3.g_pattern_step++;
				       xil_printf("dut3 -> this is a new ic with package code and gpio22 is low, go to write efuse!\r\n\r\n");
				   }

			   }
			   else if((efuse_package_valid!=0) && (efuse_writedata_valid==1))
			   {
				   dut3.g_retest = 1;
				   if(START01_GPIO22 == 1)
				   {
					   dut3.g_efuse_status = EFUSE_FT1_TEST;
				       dut3.g_pattern_step = 0x0200;
				       //dut3.g_pattern_step++;
				       xil_printf("dut3 -> this is a retest ic, but gpio22 is high, go to write package code!\r\n\r\n");
				   }
				   else
				   {
					   dut3.g_efuse_status = EFUSE_RETEST;
					   dut3.g_pattern_step = 0x0100;
					   //dut3.g_pattern_step++;
					   xil_printf("dut3 -> this is a retest ic and gpio22 is low, go to start tolerance compare!\r\n\r\n");
				   }

			   }
			   else
			   {
				   dut3.g_result_fail = 0x01;
				   dut3.g_result_fail_tmrcount = 0xffff;
			       dut3.g_dut_pattern_status_buf[2]++;
			       dut3.g_uartPatternEnable = 0x00;

			   }
			   for(i=1; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }

				   dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   //xil_printf("dut3.g_efuse_status =%x\r\n", dut3.g_efuse_status);
				   //xil_printf("dut3_patternc_efuse_step = %x\r\n\r\n",dut3.g_pattern_step);

		   }
	break;
	}



	//collect_efuse_data
	case 0x0001:
	{
		   for(i=0;i<30;i++)
		   {
		   dut3.g_efuse_temp_buf[i] = 0;
		   }
		   dut3.g_clock_frq_cnt = (dut3.g_efuse_data_buf[18]<<8 | dut3.g_efuse_data_buf[17]);

		   dut3.g_efuse_temp_buf[0] = ((QFN24_Package_Num<<4)|0x0f);                             //efuse validbit
		   dut3.g_efuse_temp_buf[1] = (dut3.g_retest<<7|0x0c);                                   //efuse validbit
		   dut3.g_efuse_temp_buf[2] = 0x40|dut3.g_efuse_data_ldo3v3;                             //efuse ldo3v3 data bit0~3
		                                                                                         //efuse vbus ocp validbit
		   dut3.g_efuse_temp_buf[3] = 0xe0;                                                      //efuse vmon comparitor
                                                                                                 //efuse vbus threshould
		   dut3.g_efuse_temp_buf[4] = (dut3.g_efuse_data_buf[5]<<5 | dut3.g_efuse_data_buf[6]);  //efuse rp data
                                                                                                 //g_efuse_data_buf[5]:rp signbit
		   dut3.g_efuse_temp_buf[5] = 0;
		   dut3.g_efuse_temp_buf[6] = (dut3.g_vmon_writeefuse_600mv[0]&0x3f);                        //efuse VBUS 0.6V cal
		   dut3.g_efuse_temp_buf[7] = (dut3.g_vmon_writeefuse_1000mv[0]&0x3f);                        //efuse VBUS 1V   cal
		   dut3.g_efuse_temp_buf[8] = (dut3.g_vmon_writeefuse_2500mv[0]&0x3f);                        //efuse VBUS 2.5V cal                                                                        //g_efuse_data_buf[6]:rp databit
		   dut3.g_efuse_temp_buf[9] = (dut3.g_vmon_writeefuse_3000mv[0]&0x3f);                        //efuse VBUS 3V   cal
		   dut3.g_efuse_temp_buf[10] = 0;
		   dut3.g_efuse_temp_buf[11] = 0;                                                     //efuse vcon ocp write experience data
		   dut3.g_efuse_temp_buf[12] = 0;
		   dut3.g_efuse_temp_buf[13] = 0;
		   dut3.g_efuse_temp_buf[14] = 0;
		   dut3.g_efuse_temp_buf[15] = 0;
		   dut3.g_efuse_temp_buf[16] = 0;
		   dut3.g_efuse_temp_buf[17] = dut3.g_ccdet_calibration_data_buf[4];                    //efuse cc_det_0x109_l
		   dut3.g_efuse_temp_buf[18] = dut3.g_ccdet_calibration_data_buf[5];                    //efuse cc_det_0x109_h
		   dut3.g_efuse_temp_buf[19] = dut3.g_ccdet_calibration_data_buf[6];                    //efuse cc_det_0x10a_l
		   dut3.g_efuse_temp_buf[20] = dut3.g_ccdet_calibration_data_buf[7];                    //efuse cc_det_0x10a_h
		   dut3.g_efuse_temp_buf[21] = dut3.g_ccdet_calibration_data_buf[8];                    //efuse cc_det_0x10b_l
		   dut3.g_efuse_temp_buf[22] = dut3.g_ccdet_calibration_data_buf[9];                    //efuse cc_det_0x10b_h
		   dut3.g_efuse_temp_buf[23] = dut3.g_ccdet_calibration_data_buf[10];                   //efuse cc_det_0x10c_l
		   dut3.g_efuse_temp_buf[24] = dut3.g_ccdet_calibration_data_buf[11];                   //efuse cc_det_0x10c_h
	       dut3.g_efuse_temp_buf[25] = 0x2A;                            						//vbus ocp3A                                       //efuse vbus ocp 3A
		   dut3.g_efuse_temp_buf[26] = dut3.g_vbusocp_writeefuse_1p5a;                          //vbus ocp1p5A                                                   //efuse vbus ocp 1.5A
		   dut3.g_efuse_temp_buf[27] = dut3.g_vbusocp_writeefuse_0p9a;                          //vbus ocp0p9A
                                                    //efuse vbus ocp 0.9A
		   dut3.g_efuse_temp_buf[28] = (dut3.g_clock_frq_cnt/10 - 1);                           //efuse tpp_l
		   dut3.g_efuse_temp_buf[29] = (dut3.g_clock_frq_cnt/20 - 1);                           //efuse rc 10us timer


		   for(i=0;i<27;i++)
		   {
		   ////xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i],i);
		   }
		   dut3.g_pattern_step++;
		   //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);

		   //////////xil_printf("dut3_pattern0_get_ic_status\r\n");
	break;
	}

	//write_efuse_data00~09
	case 0x0002:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writeefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x00;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x0a;


		   for(i=0;i<20;i++)
		   {
		   dut3.g_pattern_smbus_control_buf[i+5] = dut3.g_efuse_temp_buf[i];
		   }


		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_pattern_smbus_control_buf[i] =%x %x\r\n", dut3.g_pattern_smbus_control_buf[i+5],i);
			   }


			   for(i=0; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			   }
			       dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			       dut3.g_pattern_step++;
			       //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   }
		   //////////xil_printf("dut3_pattern0_write_efuse_data00\r\n");
	break;
	}

	//write_efuse_data10~19
	case 0x0003:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writeefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x0a;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x0a;


		   for(i=0;i<20;i++)
		   {
		   dut3.g_pattern_smbus_control_buf[i+5] = dut3.g_efuse_temp_buf[i+10];
		   }


		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_pattern_smbus_control_buf[i] =%x %x\r\n", dut3.g_pattern_smbus_control_buf[i+5],i+10);
			   }


			   for(i=0; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			   }
			       dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			       dut3.g_pattern_step++;
			       //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   }
		   //////////xil_printf("dut3_pattern0_write_efuse_data01\r\n");
	break;
	}

	//write_efuse_data20~29
	case 0x0004:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writeefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x14;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x0a;


		   for(i=0;i<20;i++)
		   {
		   dut3.g_pattern_smbus_control_buf[i+5] = dut3.g_efuse_temp_buf[i+20];
		   }


		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
			   for(i=0;i<10;i++)
			   {
			   ////xil_printf("dut3.g_pattern_smbus_control_buf[i] =%x %x\r\n", dut3.g_pattern_smbus_control_buf[i+5],i+20);
			   }


			   for(i=0; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			   }
			       dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			       dut3.g_pattern_step++;
			       //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   }
		   //////////xil_printf("dut3_pattern0_write_efuse_data02\r\n");
	break;
	}

	//Read Efuse data compare00~09
	case 0x0005:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x00;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x0a;

		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_pattern_smbus_control_buf[i] =%x %x\r\n", dut3.g_pattern_smbus_control_buf[i+10],i);
			   }
			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i],i);
			   }

			   for(i=0;i<10;i++)
			   {

			   if(dut3.g_efuse_temp_buf[i] != dut3.g_pattern_smbus_control_buf[i+10])
			   {
				   efuse_compare_bit = 1;
				   dut3.g_result_fail = 0x01;
				   dut3.g_result_fail_tmrcount = 0xffff;
			   }
			   }
			   for(i=1; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }
				   dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut3.g_pattern_step++;
				   //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   }

		   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   ////////////xil_printf("efuse_datavalid =%x\r\n", efuse_datavalid);
		   //////////xil_printf("dut3_patternc_efuse_valid_read\r\n\r\n");
	break;
	}

	//Read Efuse data compare10~19
	case 0x0006:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x0a;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x0a;

		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_pattern_smbus_control_buf[i] =%x %x\r\n", dut3.g_pattern_smbus_control_buf[i+10],i+10);
			   }
			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i+10],i+10);
			   }

			   for(i=0;i<10;i++)
			   {
			   if(dut3.g_efuse_temp_buf[i+10] != dut3.g_pattern_smbus_control_buf[i+10])
			   {
				   efuse_compare_bit = 1;
				   dut3.g_result_fail = 0x01;
				   dut3.g_result_fail_tmrcount = 0xffff;
			   }
			   }
			   for(i=1; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }
				   dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut3.g_pattern_step++;
				   //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   }

		   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   ////////////xil_printf("efuse_datavalid =%x\r\n", efuse_datavalid);
		   //////////xil_printf("dut3_patternc_efuse_valid_read\r\n\r\n");
	break;
	}

	//Read Efuse data compare20~29
	case 0x0007:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x14;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x0a;

		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {

			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_pattern_smbus_control_buf[i] =%x %x\r\n", dut3.g_pattern_smbus_control_buf[i+10],i+20);
			   }
			   for(i=0;i<10;i++)
			   {
			   //xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i+20],i+20);
			   }
			   for(i=0;i<10;i++)
			   {
			   if(dut3.g_efuse_temp_buf[i+20] != dut3.g_pattern_smbus_control_buf[i+10])
			   {
				   efuse_compare_bit = 1;
				   dut3.g_result_fail = 0x01;
				   dut3.g_result_fail_tmrcount = 0xffff;
			   }
			   }
			   for(i=1; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }
				   dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut3.g_pattern_step++;
				   //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   }

		   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   ////////////xil_printf("efuse_datavalid =%x\r\n", efuse_datavalid);
		   //////////xil_printf("dut3_patternc_efuse_valid_read\r\n\r\n");
	break;
	}

	case 0x0008:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writeefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x01;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;
		   dut3.g_pattern_smbus_control_buf[5] = 0x80;

		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
			   for(i=0; i<30; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			   }
			       dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			       dut3.g_pattern_step=0x0100;
			       //xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		   }
		   //////////xil_printf("dut3_pattern0_get_ic_status\r\n");
	break;
	}

	//Tolerance Efuse data compare00~09
	case 0x0100:
	{
		//collect efuse data
		for(i=0;i<30;i++)
		{
			dut3.g_efuse_temp_buf[i] = 0;
		}
		dut3.g_clock_frq_cnt = (dut3.g_efuse_data_buf[18]<<8 | dut3.g_efuse_data_buf[17]);

		dut3.g_efuse_temp_buf[0] = ((QFN24_Package_Num<<4)|0x0f);                             //efuse validbit
		dut3.g_efuse_temp_buf[1] = (dut3.g_retest<<7|0x0c);                                   //efuse validbit
		dut3.g_efuse_temp_buf[2] = 0x40|dut3.g_efuse_data_ldo3v3;                             //efuse ldo3v3 data bit0~3
		                                                                                         //efuse vbus ocp validbit
		dut3.g_efuse_temp_buf[3] = 0xe0;                                                      //efuse vmon comparitor
                                                                                                 //efuse vbus threshould
		dut3.g_efuse_temp_buf[4] = (dut3.g_efuse_data_buf[5]<<5 | dut3.g_efuse_data_buf[6]);  //efuse rp data
                                                                                                 //g_efuse_data_buf[5]:rp signbit
		dut3.g_efuse_temp_buf[5] = 0;
		dut3.g_efuse_temp_buf[6] = (dut3.g_vmon_writeefuse_600mv[0]&0x3f);                        //efuse VBUS 0.6V cal
		dut3.g_efuse_temp_buf[7] = (dut3.g_vmon_writeefuse_1000mv[0]&0x3f);                        //efuse VBUS 1V   cal
		dut3.g_efuse_temp_buf[8] = (dut3.g_vmon_writeefuse_2500mv[0]&0x3f);                        //efuse VBUS 2.5V cal                                                                        //g_efuse_data_buf[6]:rp databit
		dut3.g_efuse_temp_buf[9] = (dut3.g_vmon_writeefuse_3000mv[0]&0x3f);                        //efuse VBUS 3V   cal
		dut3.g_efuse_temp_buf[10] = 0;
		dut3.g_efuse_temp_buf[11] = 0;                                                     //efuse vcon ocp write experience data
		dut3.g_efuse_temp_buf[12] = 0;
		dut3.g_efuse_temp_buf[13] = 0;
		dut3.g_efuse_temp_buf[14] = 0;
		dut3.g_efuse_temp_buf[15] = 0;
		dut3.g_efuse_temp_buf[16] = 0;
		dut3.g_efuse_temp_buf[17] = dut3.g_ccdet_calibration_data_buf[4];                    //efuse cc_det_0x109_l
		dut3.g_efuse_temp_buf[18] = dut3.g_ccdet_calibration_data_buf[5];                    //efuse cc_det_0x109_h
		dut3.g_efuse_temp_buf[19] = dut3.g_ccdet_calibration_data_buf[6];                    //efuse cc_det_0x10a_l
		dut3.g_efuse_temp_buf[20] = dut3.g_ccdet_calibration_data_buf[7];                    //efuse cc_det_0x10a_h
		dut3.g_efuse_temp_buf[21] = dut3.g_ccdet_calibration_data_buf[8];                    //efuse cc_det_0x10b_l
		dut3.g_efuse_temp_buf[22] = dut3.g_ccdet_calibration_data_buf[9];                    //efuse cc_det_0x10b_h
		dut3.g_efuse_temp_buf[23] = dut3.g_ccdet_calibration_data_buf[10];                   //efuse cc_det_0x10c_l
		dut3.g_efuse_temp_buf[24] = dut3.g_ccdet_calibration_data_buf[11];                   //efuse cc_det_0x10c_h
		dut3.g_efuse_temp_buf[25] = 0x2A;                            						 //vbus ocp3A                                       //efuse vbus ocp 3A
		dut3.g_efuse_temp_buf[26] = dut3.g_vbusocp_writeefuse_1p5a;                          //vbus ocp1p5A                                                   //efuse vbus ocp 1.5A
		dut3.g_efuse_temp_buf[27] = dut3.g_vbusocp_writeefuse_0p9a;                          //vbus ocp0p9A
		dut3.g_efuse_temp_buf[28] = (dut3.g_clock_frq_cnt/10 - 1);                           //efuse tpp_l
		dut3.g_efuse_temp_buf[29] = (dut3.g_clock_frq_cnt/20 - 1);                           //efuse rc 10us timer

		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut3.g_pattern_smbus_control_buf[2] = 0x00;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
		dut3.g_pattern_smbus_control_buf[4] = 0x0a;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<10;i++)
			{
				efuse_read_buf[i]=dut3.g_pattern_smbus_control_buf[i+10];
			}
			for(i=0;i<10;i++)
			{
				xil_printf("dut3.efuse_read[i] =0x%x i =0x%x\r\n", efuse_read_buf[i],i);
			}
			//for(i=0;i<10;i++)
			//{
			   //xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i],i);
			//}

			////////////LDO3V3 Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[2]<4)
			{
				dut3.g_efuse_temp_buf[2]=dut3.g_efuse_temp_buf[2]+8;
			}
			if(efuse_read_buf[2]<4)
			{
				efuse_read_buf[2]=efuse_read_buf[2]+8;
			}
			if(dut3.g_efuse_temp_buf[2]>efuse_read_buf[2])
			{
				efuse_ldo3v3_tolerance=dut3.g_efuse_temp_buf[2]-efuse_read_buf[2];
				if(efuse_ldo3v3_tolerance > LDO3V3_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_ldo3v3_tolerance=efuse_read_buf[2]-dut3.g_efuse_temp_buf[2];
				if(efuse_ldo3v3_tolerance > LDO3V3_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////LDO3V3 Tolerance Compare/////////////////

			////////////RP12K Tolerance Compare/////////////////
			if(dut3.g_efuse_data_buf[5]==1)
			{
				if(((efuse_read_buf[4]>>5)&0x01) ==1)
				{
					if(dut3.g_efuse_data_buf[6]>(efuse_read_buf[4]&0x0f))
					{
						efuse_rp12k_tolerance=dut3.g_efuse_data_buf[6]-(efuse_read_buf[4]&0x0f);
					}
					else
					{
						efuse_rp12k_tolerance=(efuse_read_buf[4]&0x0f)-dut3.g_efuse_data_buf[6];
					}
					if(efuse_rp12k_tolerance > RP12K_Tolerance_Compare_Data)
					{
						efuse_compare_bit = 1;
						dut3.g_result_fail = 0x01;
						dut3.g_result_fail_tmrcount = 0xffff;
					}
					else
					{

					}
				}
				else if(((efuse_read_buf[4]>>5)&0x01) ==0)
				{
					efuse_rp12k_tolerance=dut3.g_efuse_data_buf[6]+(efuse_read_buf[4]&0x0f);
					if(efuse_rp12k_tolerance > RP12K_Tolerance_Compare_Data)
					{
						efuse_compare_bit = 1;
						dut3.g_result_fail = 0x01;
						dut3.g_result_fail_tmrcount = 0xffff;
					}
					else
					{

					}
				}
			}

			if(dut3.g_efuse_data_buf[5]==0)
			{
				if(((efuse_read_buf[4]>>5)&0x01) ==0)
				{
					if(dut3.g_efuse_data_buf[6]>(efuse_read_buf[4]&0x0f))
					{
						efuse_rp12k_tolerance=dut3.g_efuse_data_buf[6]-(efuse_read_buf[4]&0x0f);
					}
					else
					{
						efuse_rp12k_tolerance=(efuse_read_buf[4]&0x0f)-dut3.g_efuse_data_buf[6];
					}
					if(efuse_rp12k_tolerance > RP12K_Tolerance_Compare_Data)
					{
						efuse_compare_bit = 1;
						dut3.g_result_fail = 0x01;
						dut3.g_result_fail_tmrcount = 0xffff;
					}
					else
					{

					}
				}
				else if(((efuse_read_buf[4]>>5)&0x01) ==1)
				{
					efuse_rp12k_tolerance=dut3.g_efuse_data_buf[6]+(efuse_read_buf[4]&0x0f);
					if(efuse_rp12k_tolerance > RP12K_Tolerance_Compare_Data)
					{
						efuse_compare_bit = 1;
						dut3.g_result_fail = 0x01;
						dut3.g_result_fail_tmrcount = 0xffff;
					}
					else
					{

					}
				}
			}
			////////////RP12K Tolerance Compare/////////////////

			////////////VBUS/VMON 0P6V Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[6]>efuse_read_buf[6])
			{
				efuse_vbus0p6v_tolerance = dut3.g_efuse_temp_buf[6]-efuse_read_buf[6];
			}
			else
			{
				efuse_vbus0p6v_tolerance = efuse_read_buf[6]-dut3.g_efuse_temp_buf[6];
			}
			if(efuse_vbus0p6v_tolerance > VMON_Tolerance_Compare_Data)
			{
				efuse_compare_bit = 1;
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
			else
			{

			}
			////////////VBUS/VMON 0P6V Tolerance Compare/////////////////

			////////////VBUS/VMON 1V Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[7]>efuse_read_buf[7])
			{
				efuse_vbus0p6v_tolerance = dut3.g_efuse_temp_buf[7]-efuse_read_buf[7];
			}
			else
			{
				efuse_vbus0p6v_tolerance = efuse_read_buf[7]-dut3.g_efuse_temp_buf[7];
			}
			if(efuse_vbus0p6v_tolerance > VMON_Tolerance_Compare_Data)
			{
				efuse_compare_bit = 1;
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
			else
			{

			}
			////////////VBUS/VMON 1V Tolerance Compare/////////////////

			////////////VBUS/VMON 2.5V Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[8]>efuse_read_buf[8])
			{
				efuse_vbus0p6v_tolerance = dut3.g_efuse_temp_buf[8]-efuse_read_buf[8];
			}
			else
			{
				efuse_vbus0p6v_tolerance = efuse_read_buf[8]-dut3.g_efuse_temp_buf[8];
			}
			if(efuse_vbus0p6v_tolerance > VMON_Tolerance_Compare_Data)
			{
				efuse_compare_bit = 1;
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
			else
			{

			}
			////////////VBUS/VMON 2.5V Tolerance Compare/////////////////

			////////////VBUS/VMON 3V Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[9]>efuse_read_buf[9])
			{
				efuse_vbus0p6v_tolerance = dut3.g_efuse_temp_buf[9]-efuse_read_buf[9];
			}
			else
			{
				efuse_vbus0p6v_tolerance = efuse_read_buf[9]-dut3.g_efuse_temp_buf[9];
			}
			if(efuse_vbus0p6v_tolerance > VMON_Tolerance_Compare_Data)
			{
				efuse_compare_bit = 1;
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
			else
			{

			}
			////////////VBUS/VMON 3V Tolerance Compare/////////////////

			for(i=1; i<30; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	//Tolerance Efuse data compare10~16
	case 0x0101:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut3.g_pattern_smbus_control_buf[2] = 0x0a;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
		dut3.g_pattern_smbus_control_buf[4] = 0x07;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<7;i++)
			{
				efuse_read_buf[i+10]=dut3.g_pattern_smbus_control_buf[i+10];
			}
			for(i=0;i<7;i++)
			{
				xil_printf("dut3.efuse_read[i] =0x%x i =0x%x\r\n", efuse_read_buf[i+10],i+10);
			}
			//for(i=0;i<7;i++)
			//{
				//xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i+10],i+10);
			//}

			for(i=1; i<30; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	//Tolerance Efuse data compare17~24
	case 0x0102:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut3.g_pattern_smbus_control_buf[2] = 0x11;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
		dut3.g_pattern_smbus_control_buf[4] = 0x08;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<8;i++)
			{
				efuse_read_buf[i+17]=dut3.g_pattern_smbus_control_buf[i+10];
			}
			for(i=0;i<8;i++)
			{
				xil_printf("dut3.efuse_read[i] =0x%x i =0x%x\r\n", efuse_read_buf[i+17],i+17);
			}
			//for(i=17;i<25;i++)
			//{
				//xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i],i);
			//}

			////////////CC_CFG0_L Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[17]&0xf0)>>4)>((efuse_read_buf[17]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[17]&0xf0)>>4)-((efuse_read_buf[17]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[17]&0xf0)>>4)-((dut3.g_efuse_temp_buf[17]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[17]&0xf)>(efuse_read_buf[17]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[17]&0xf)-(efuse_read_buf[17]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[17]&0xf)-(dut3.g_efuse_temp_buf[17]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////CC_CFG0_L Tolerance Compare/////////////////

			////////////CC_CFG0_H Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[18]&0xf0)>>4)>((efuse_read_buf[18]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[18]&0xf0)>>4)-((efuse_read_buf[18]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[18]&0xf0)>>4)-((dut3.g_efuse_temp_buf[18]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[18]&0xf)>(efuse_read_buf[18]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[18]&0xf)-(efuse_read_buf[18]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[18]&0xf)-(dut3.g_efuse_temp_buf[18]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{
				}
			}
			////////////CC_CFG0_H Tolerance Compare/////////////////

			////////////CC_CFG1_L Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[19]&0xf0)>>4)>((efuse_read_buf[19]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[19]&0xf0)>>4)-((efuse_read_buf[19]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[19]&0xf0)>>4)-((dut3.g_efuse_temp_buf[19]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[19]&0xf)>(efuse_read_buf[19]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[19]&0xf)-(efuse_read_buf[19]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[19]&0xf)-(dut3.g_efuse_temp_buf[19]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////CC_CFG1_L Tolerance Compare/////////////////

			////////////CC_CFG1_H Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[20]&0xf0)>>4)>((efuse_read_buf[20]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[20]&0xf0)>>4)-((efuse_read_buf[20]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[20]&0xf0)>>4)-((dut3.g_efuse_temp_buf[20]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[20]&0xf)>(efuse_read_buf[20]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[20]&0xf)-(efuse_read_buf[20]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[20]&0xf)-(dut3.g_efuse_temp_buf[20]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////CC_CFG1_H Tolerance Compare/////////////////

			////////////CC_CFG2_L Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[21]&0xf0)>>4)>((efuse_read_buf[21]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[21]&0xf0)>>4)-((efuse_read_buf[21]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[21]&0xf0)>>4)-((dut3.g_efuse_temp_buf[21]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[21]&0xf)>(efuse_read_buf[21]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[21]&0xf)-(efuse_read_buf[21]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[21]&0xf)-(dut3.g_efuse_temp_buf[21]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////CC_CFG2_L Tolerance Compare/////////////////

			////////////CC_CFG2_H Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[22]&0xf0)>>4)>((efuse_read_buf[22]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[22]&0xf0)>>4)-((efuse_read_buf[22]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[22]&0xf0)>>4)-((dut3.g_efuse_temp_buf[22]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[22]&0xf)>(efuse_read_buf[22]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[22]&0xf)-(efuse_read_buf[22]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[22]&0xf)-(dut3.g_efuse_temp_buf[22]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////CC_CFG2_H Tolerance Compare/////////////////

			////////////CC_CFG3_L Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[23]&0xf0)>>4)>((efuse_read_buf[23]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[23]&0xf0)>>4)-((efuse_read_buf[23]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[23]&0xf0)>>4)-((dut3.g_efuse_temp_buf[23]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[23]&0xf)>(efuse_read_buf[23]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[23]&0xf)-(efuse_read_buf[23]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[23]&0xf)-(dut3.g_efuse_temp_buf[23]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////CC_CFG3_L Tolerance Compare/////////////////

			////////////CC_CFG3_H Tolerance Compare/////////////////
			if(((dut3.g_efuse_temp_buf[24]&0xf0)>>4)>((efuse_read_buf[24]&0xf0)>>4))
			{
				efuse_cfg0_l_tolerance=((dut3.g_efuse_temp_buf[24]&0xf0)>>4)-((efuse_read_buf[24]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=((efuse_read_buf[24]&0xf0)>>4)-((dut3.g_efuse_temp_buf[24]&0xf0)>>4);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}

			if((dut3.g_efuse_temp_buf[24]&0xf)>(efuse_read_buf[24]&0xf))
			{
				efuse_cfg0_l_tolerance=(dut3.g_efuse_temp_buf[24]&0xf)-(efuse_read_buf[24]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_cfg0_l_tolerance=(efuse_read_buf[24]&0xf)-(dut3.g_efuse_temp_buf[24]&0xf);
				if(efuse_cfg0_l_tolerance > CCDET_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////CC_CFG3_H Tolerance Compare/////////////////

			for(i=1; i<30; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
			//dut3.g_pattern_step++;
		}
		break;
	}

	//Tolerance Efuse data compare25~29
	case 0x0103:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut3.g_pattern_smbus_control_buf[2] = 0x19;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
		dut3.g_pattern_smbus_control_buf[4] = 0x05;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<5;i++)
			{
				efuse_read_buf[i+25]=dut3.g_pattern_smbus_control_buf[i+10];
			}
			for(i=0;i<5;i++)
			{
				xil_printf("dut3.efuse_read[i] =0x%x i =0x%x\r\n", efuse_read_buf[i+25],i+25);
			}
			//for(i=25;i<30;i++)
			//{
				//xil_printf("dut3.g_efuse_temp_buf[i] =%x %x\r\n", dut3.g_efuse_temp_buf[i],i);
			//}
/*
			////////////VBUSOCP Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[25]>efuse_read_buf[25])
			{
				efuse_vbusocp_3a_tolerance=dut3.g_efuse_temp_buf[25]-efuse_read_buf[25];
				if(efuse_vbusocp_3a_tolerance > VBUS_3A_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_vbusocp_3a_tolerance=efuse_read_buf[25]-dut3.g_efuse_temp_buf[25];
				if(efuse_vbusocp_3a_tolerance > VBUS_3A_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
*/
			if(dut3.g_efuse_temp_buf[26]>efuse_read_buf[26])
			{
				efuse_vbusocp_1p5a_tolerance=dut3.g_efuse_temp_buf[26]-efuse_read_buf[26];
				if(efuse_vbusocp_1p5a_tolerance > VBUS_1P5A_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_vbusocp_1p5a_tolerance=efuse_read_buf[26]-dut3.g_efuse_temp_buf[26];
				if(efuse_vbusocp_1p5a_tolerance > VBUS_1P5A_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
/*
			if(dut3.g_efuse_temp_buf[27]>efuse_read_buf[27])
			{
				efuse_vbusocp_0p9a_tolerance=dut3.g_efuse_temp_buf[27]-efuse_read_buf[27];
				if(efuse_vbusocp_0p9a_tolerance > VBUS_0P5A_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_vbusocp_0p9a_tolerance=efuse_read_buf[27]-dut3.g_efuse_temp_buf[27];
				if(efuse_vbusocp_0p9a_tolerance > VBUS_0P5A_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
*/
			////////////TPP_L Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[28]>efuse_read_buf[28])
			{
				efuse_tpp_l_tolerance=dut3.g_efuse_temp_buf[28]-efuse_read_buf[28];
				if(efuse_tpp_l_tolerance > RCClock_Tolerance_Compare_Data*2)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_tpp_l_tolerance=efuse_read_buf[28]-dut3.g_efuse_temp_buf[28];
				if(efuse_tpp_l_tolerance > RCClock_Tolerance_Compare_Data*2)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////TPP_L Tolerance Compare/////////////////

			////////////RC_Clock Tolerance Compare/////////////////
			if(dut3.g_efuse_temp_buf[29]>efuse_read_buf[29])
			{
				efuse_rc_clock_tolerance=dut3.g_efuse_temp_buf[29]-efuse_read_buf[29];
				if(efuse_rc_clock_tolerance > RCClock_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			else
			{
				efuse_rc_clock_tolerance=efuse_read_buf[29]-dut3.g_efuse_temp_buf[29];
				if(efuse_rc_clock_tolerance > RCClock_Tolerance_Compare_Data)
				{
					efuse_compare_bit = 1;
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
				else
				{

				}
			}
			////////////RC_Clock Tolerance Compare/////////////////

			for(i=1; i<30; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x0104:
	{
		dut3.g_pattern_step = 0;

		//dut3.g_dut_start_ready = 0x11;
		////////initial dut///////
		for(i=1; i<30; i++)
		{
			dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
		}
		dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

		////////initial dut///////
		for(i=1; i<30; i++)
		{
			dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
		}
		dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		dut3.g_smbus_status_buf[0] = 0x00;
		dut3.g_smbus_status_buf[1] = 0xff;
		dut3.g_smbus_style = 0x00;
		dut3.g_smbus_timer = 0xff;
		dut3.g_relay_control_timer = 0xff;
		dut3.g_smbus_getstatus_timer = 0x02;

		for(i=0;i<12;i++)
		{
			dut3.g_ccdet_calibration_data_buf[i] = 0x88;
		}
		dut3.g_efuse_status = 0;
		dut3.g_retest = 0;
		dut3.g_ccdet_step = 0;
		dut3.g_ccdet_retest_signbit = 0;

		for(i=0;i<8;i++)
		{
			dut3.g_dut_pattern_status_buf[i] = 0;
		}
		dut3.g_dut_start_ready = 0;

		//pattern_safe_state_dut3();	//add safe state

		usdelay(100);
		dut3.g_pattern_timer = 0xfff;
		dut3.g_uartPatternEnable = 0x00;

		if(Loop_run_enable ==1)
		{
			dut3.g_dut_start_ready = 0x11;
			dut3.g_uartPatternNum = 0x0;
			dut3.g_test_count++;
			xil_printf("dut3.g_test_count =%x\r\n", dut3.g_test_count);
		}
		else
		{
			dut3.g_uartPatternNum = 0x0e; //FT2 test pass
		}

		result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
		msdelay(10);

		//////xil_printf("adc_a = %x\r\n",adc_a);
		//////xil_printf("adc_b = %x\r\n",adc_b);
		//////xil_printf("adc_full_voltage = %x\r\n",adc_full_voltage);
		//xil_printf("dut3.g_uartPatternNum = %x\r\n",dut3.g_uartPatternNum);
		xil_printf("dut3_patternc_ft2_efuse_check_pass!\r\n\r\ndut3_ft2_pass!\r\n\r\n");
		xil_printf("dut3.g_uartPatternNum = %x\r\n\r\n",dut3.g_uartPatternNum);
		dut3.g_ft2_test_done = 0x00;
		pattern_safe_state_dut3();	//add safe state

		//////xil_printf("dut3.g_dut_start_ready = %x\r\n\r\n",dut3.g_dut_start_ready);
		//////xil_printf("dut3.g_result_fail = %x\r\n\r\n",dut3.g_result_fail);
		//////xil_printf("dut3.g_dut_pattern_status_buf[2] = %x\r\n\r\n",dut3.g_dut_pattern_status_buf[2]);
		//////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
		//////////////xil_printf("efuse_datavalid =%x\r\n", efuse_datavalid);
		////////////xil_printf("dut3_patternc_efuse_valid_read\r\n\r\n");
		break;
	}

	//write_efuse_package number
	case 0x0200:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writeefuse;
		   dut3.g_pattern_smbus_control_buf[2] = 0x00;
		   dut3.g_pattern_smbus_control_buf[3] = 0x00;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;
		   dut3.g_pattern_smbus_control_buf[5] = (QFN24_Package_Num<<4);  //0x20:RL6628 QFN20 package number

		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   //////xil_printf("dut3.g_pattern_step = %x\r\n\r\n",dut3.g_pattern_step);
			   ////xil_printf("dut3.g_pattern_smbus_control_buf[5] =%x\r\n", dut3.g_pattern_smbus_control_buf[5]);
			   dut3.g_pattern_step = 0;

			   ////////////reset flag///////////////////////////
					for(i=1; i<30; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

					////////initial dut///////
					for(i=1; i<30; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_smbus_status_buf[0] = 0x00;
					dut3.g_smbus_status_buf[1] = 0xff;
					dut3.g_smbus_style = 0x00;
					dut3.g_smbus_timer = 0xff;
					dut3.g_relay_control_timer = 0xff;
					dut3.g_smbus_getstatus_timer = 0x02;

					for(i=0;i<12;i++)
					{
						dut3.g_ccdet_calibration_data_buf[i] = 0x88;
					}
					dut3.g_efuse_status = 0;
					dut3.g_retest = 0;
					dut3.g_ccdet_step = 0;
					dut3.g_ccdet_retest_signbit = 0;

					for(i=0;i<8;i++)
					{
						dut3.g_dut_pattern_status_buf[i] = 0;
					}
					dut3.g_dut_start_ready = 0;

					usdelay(100);
		   	   	    dut3.g_pattern_timer = 0xfff;
				    dut3.g_uartPatternEnable = 0x00;
				    if(Loop_run_enable ==1)
				    {
						dut3.g_uartPatternNum = 0x0;
						dut3.g_dut_start_ready = 0x11;
						dut3.g_test_count++;
						xil_printf("dut3.g_test_count =%x\r\n", dut3.g_test_count);
				    }
				    else
				    	{
				    	dut3.g_uartPatternNum = 0x0d;
				    	}
					result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
					 ////////////reset flag///////////////////////////

					msdelay(10);

					xil_printf("dut3_patternc_ft1_efuse_check_pass!\r\n\r\n");
					xil_printf("dut3.g_uartPatternNum = %x\r\n\r\n",dut3.g_uartPatternNum);
		   }
		   //////////xil_printf("dut3_pattern0_write_efuse_data01\r\n");
	break;
	}


	//for loop run test





	}


	if(dut3.g_result_fail == 0x01)
	{
		dut3.g_ft2_test_done = 0x00;
		pattern_safe_state_dut3();	//add safe state
		xil_printf("dut3_patternc_fail! fail_step = %x\r\n\r\n",dut3.g_pattern_step);
	}
	//xil_printf("dut3_patternc_efuse_step = %x\r\n\r\n",dut3.g_pattern_step);
	//xil_printf("dut3.g_result_fail = %x\r\n\r\n",dut3.g_result_fail);
	////////xil_printf("dut3.g_uartPatternNum = %x\r\n\r\n",dut3.g_uartPatternNum);
	////////xil_printf("dut3.g_uartPatternEnable = %x\r\n\r\n",dut3.g_uartPatternEnable);
	return 0;
	}









