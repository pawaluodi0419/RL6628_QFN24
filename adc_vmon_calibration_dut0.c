#include "platform.h"
#include "adc_vmon_calibration.h"

u8 _vmon_calibration_dut0()
{
	u8 i;
	switch(dut0.g_vmoncompare_process_bit)
	{
		//REG_VBUS_COMP_CFG2 reg 0x1047 bit3(AIF:REG_POW1)??1'b1;
	    case vmon_process_waiting:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut0.g_pattern_smbus_control_buf[2] = 0x47;
		    dut0.g_pattern_smbus_control_buf[3] = 0x10;
		    dut0.g_pattern_smbus_control_buf[4] = 0x01;
		    dut0.g_pattern_smbus_control_buf[5] = ((dut0.g_vmon_calib_level &0x07) |(dut0.g_vmon_temp_voltage[2]&0xF8)|(1<<3));

		    ////////xil_printf("dut0.g_pattern_smbus_control_buf[5] = %x\r\n",dut0.g_pattern_smbus_control_buf[5]);
		   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
			           dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut0.g_vmoncompare_process_bit = vmon_process_doing;
				       dut0.g_vmon_compare_step++;
		   }
		   ////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);

		break;
		}
	   case vmon_process_doing:
	{

		switch(dut0.g_vmon_compare_step)
	    {

		case 0x0000:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut0.g_pattern_smbus_control_buf[2] = 0x47;
		    dut0.g_pattern_smbus_control_buf[3] = 0x10;
		    dut0.g_pattern_smbus_control_buf[4] = 0x01;
		    dut0.g_pattern_smbus_control_buf[5] = ((dut0.g_vmon_calib_level &0x07) |(dut0.g_vmon_temp_voltage[2]&0xF8)|(1<<3));

		    ////////xil_printf("dut0.g_pattern_smbus_control_buf[5] = %x\r\n",dut0.g_pattern_smbus_control_buf[5]);
		   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				   dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut0.g_vmoncompare_process_bit = vmon_process_doing;
				   dut0.g_vmon_compare_step++;
		   }
		   ////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);

		break;
		}

		case 0x0001:
		{
			////xil_printf("dut0_pattern2_patternstep3_vmonstep1_start\r\n");

			i2c_max5825_output(AD7994_DEV0_ADDR,MAX5825_DEV0_SLAVE_ADDR,0,dut0.g_vmon_calib_voltage/7.98);

			usdelay(10);
	    	dut0.g_vmon_compare_step++;
	    	dut0.g_vmoncompare_process_bit = vmon_process_doing;
	    	////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);
	    	////xil_printf("dut0_dac_writen =%x,%x,%x\r\n",dut0.g_dac_data_buf[0],dut0.g_dac_data_buf[1],dut0.g_dac_data_buf[2]);
			break;
		}

		//QFN20/12:write REG_VBUS_COMP_CFG4 reg 0x1049 bit[4:0] (AIF:REG_VTH2_TUNE) with 0x0f
		//QFN24:write REG_VBUS_COMP_CFG4 reg 0x1048 bit[5:0] (AIF:REG_VTH2_TUNE) with 0x1f
		case 0x0002:
		{

			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut0.g_pattern_smbus_control_buf[2] = 0x48;
		    dut0.g_pattern_smbus_control_buf[3] = 0x10;
		    dut0.g_pattern_smbus_control_buf[4] = 0x01;
		    dut0.g_pattern_smbus_control_buf[5] = 0x1f;

		    ////////xil_printf("dut0.g_pattern_smbus_control_buf[5] = %x\r\n",dut0.g_pattern_smbus_control_buf[5]);
		   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
		   dut0.g_vmon_calibration_data_buf[0] = 0x1f;
				   for(i=1; i<31; i++)
				   {
					   dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				   dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut0.g_vmoncompare_process_bit = vmon_process_doing;
				   dut0.g_vmon_compare_step++;
		   }
		   ////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);
		   ////xil_printf("dut0.g_pattern_smbus_control_buf[0] = %x\r\n",dut0.g_pattern_smbus_control_buf[0]);
		break;
		}

		//read REG_VBUS_COMP_DBC2 reg 0x1045 bit5 and set calibration register signbit

		case 0x0003:
		{
			   dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			   dut0.g_pattern_smbus_control_buf[2] = 0x45;
			   dut0.g_pattern_smbus_control_buf[3] = 0x10;
			   dut0.g_pattern_smbus_control_buf[4] = 0x01;

			   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
			   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			   {
				   break;
			   }
			   else
			   {

				   //xil_printf("dut0.g_pattern_smbus_control_buf[10] =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
				   if(((dut0.g_pattern_smbus_control_buf[10] >>4) & 0x01) == 1)
				   {
					   dut0.g_calibrate_signbit = 0;
					   dut0.g_calibration_offset = 1;
					   dut0.g_vmon_compare_step++;
				   }
				   else
				   {
					   dut0.g_calibrate_signbit = 1;
					   dut0.g_calibration_offset = 1;
					   dut0.g_vmon_compare_step++;
				   }

				   for(i=0; i<31; i++)
				   {
					   dut0.g_pattern_smbus_control_buf[i+1] = CLEAR_;
				   }
				   dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut0.g_vmoncompare_process_bit = vmon_process_doing;

				   //xil_printf("dut0.g_calibrate_signbit =%x\r\n", dut0.g_calibrate_signbit);
				   //xil_printf("dut0.g_calibration_offset =%x\r\n", dut0.g_calibration_offset);

			   }

		       ////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);

		break;
		}

		  //write to calibration mem
			case 0x0004:
			{
				dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
				dut0.g_pattern_smbus_control_buf[2] = 0x48;
				dut0.g_pattern_smbus_control_buf[3] = 0x10;
				dut0.g_pattern_smbus_control_buf[4] = 0x01;

				if(dut0.g_calibrate_signbit == 0)
				{
		       			if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x20))
					{
					        dut0.g_pattern_smbus_control_buf[5] = (dut0.g_vmon_calibration_data_buf[0]+dut0.g_calibration_offset);
						    //////xil_printf("\r\ndut0.g_vmon_calibration_data_buf[0] =%x\r\n", dut0.g_vmon_calibration_data_buf[0]);
						    //////xil_printf("dut0.g_vmon_calibration_data_buf[1] =%x\r\n", dut0.g_vmon_calibration_data_buf[1]);
						    //////xil_printf("dut0.g_pattern_smbus_control_buf[5] = %x\r\n", dut0.g_pattern_smbus_control_buf[5]);
						    //////xil_printf("dut0.g_pattern_smbus_control_buf[6] = %x\r\n", dut0.g_pattern_smbus_control_buf[6]);
				}
					else
					{
						dut0.g_result_fail = 0x01;
						dut0.g_result_fail_tmrcount = 0xffff;
					}
				}
				else if(dut0.g_calibrate_signbit == 1)
				{
					if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x1f))
					{
				        dut0.g_pattern_smbus_control_buf[5] = (dut0.g_vmon_calibration_data_buf[0]-dut0.g_calibration_offset);
						}
					else
					{
						dut0.g_result_fail = 0x01;
						dut0.g_result_fail_tmrcount = 0xffff;
					}
					       //////xil_printf("\r\ndut0.g_vmon_calibration_data_buf[0] =%x\r\n", dut0.g_vmon_calibration_data_buf[0]);
					       //////xil_printf("dut0.g_vmon_calibration_data_buf[1] =%x\r\n", dut0.g_vmon_calibration_data_buf[1]);
					       //////xil_printf("dut0.g_pattern_smbus_control_buf[5] =%x\r\n", dut0.g_pattern_smbus_control_buf[5]);
					       //////xil_printf("dut0.g_pattern_smbus_control_buf[6] =%x\r\n\r\n", dut0.g_pattern_smbus_control_buf[6]);
					}
					else
					{
						dut0.g_result_fail = 0x01;
						dut0.g_result_fail_tmrcount = 0xffff;
					}
				//xil_printf("dut0.g_pattern_smbus_control_buf[5] =%x\r\n", dut0.g_pattern_smbus_control_buf[5]);
				//xil_printf("dut0.g_pattern_smbus_control_buf[6] =%x\r\n", dut0.g_pattern_smbus_control_buf[6]);

				smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
				if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
				{

				}
				else
				{
					for(i=1; i<31; i++)
					{
						dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut0.g_vmoncompare_process_bit = vmon_process_doing;
					dut0.g_vmon_compare_step++;

					   ////////xil_printf("dut0.g_vmon_retest_signbit =%x\r\n", dut0.g_vmon_retest_signbit);
					   //xil_printf("dut0.g_vmon_calibration_data_buf[0] =%x\r\n", dut0.g_vmon_calibration_data_buf[0]);
					   //xil_printf("dut0.g_vmon_calibration_data_buf[1] =%x\r\n", dut0.g_vmon_calibration_data_buf[1]);

					   ////////xil_printf("dut0.g_calibrate_signbit =%x\r\n", dut0.g_calibrate_signbit);
					   ////////xil_printf("dut0.g_calibration_offset =%x\r\n", dut0.g_calibration_offset);
				       ////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);
				}



				break;
			}

		//read 0x1045 data and calibration
		case 0x0005:
		{
			   dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			   dut0.g_pattern_smbus_control_buf[2] = 0x45;
			   dut0.g_pattern_smbus_control_buf[3] = 0x10;
			   dut0.g_pattern_smbus_control_buf[4] = 0x01;

			   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
			   ////////xil_printf("\r\n dut0.g_pattern_smbus_control_buf[10] =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
			   ////////xil_printf("\r\n dut0.g_pattern_smbus_control_buf[11] =%x\r\n", dut0.g_pattern_smbus_control_buf[11]);
			   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			   {
				   break;
			   }
			   else
			   {

				   if(((dut0.g_pattern_smbus_control_buf[10] >> 4) & 0x01) == 0)
				   {
					  if(dut0.g_calibrate_signbit == 1)
					  {
						  if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset < 0x1f))
						  {
							  dut0.g_calibration_offset++;
							  dut0.g_vmon_compare_step--;
						  }

						  else
						  {
							  dut0.g_result_fail = 0x01;
							  dut0.g_result_fail_tmrcount = 0xffff;
						  }
					  }

					  else if(dut0.g_calibrate_signbit == 0)
					  {
						  if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x20))
						  {
						  dut0.g_vmon_compare_step++;

						  }
						  else
						  {
							dut0.g_result_fail = 0x01;
							dut0.g_result_fail_tmrcount = 0xffff;
						  }
					  }
				   }


				   else if(((dut0.g_pattern_smbus_control_buf[10] >> 4) & 0x01) == 1)
				   {
					  if(dut0.g_calibrate_signbit == 0)
					  {
						  if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset < 0x20))
						  {
							  dut0.g_calibration_offset++;
							  dut0.g_vmon_compare_step--;
						  }

						  else
						  {
							  dut0.g_result_fail = 0x01;
							  dut0.g_result_fail_tmrcount = 0xffff;
						  }
					  }

					  else if(dut0.g_calibrate_signbit == 1)
					  {
						  if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x1f))
						  {
						  dut0.g_vmon_compare_step++;

						  }
						  else
						  {
							dut0.g_result_fail = 0x01;
							dut0.g_result_fail_tmrcount = 0xffff;
						  }
					  }
				   }
				   //xil_printf("\r\n dut0.g_pattern_smbus_control_buf[10] =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
				   //xil_printf("\r\n dut0.g_pattern_smbus_control_buf[11] =%x\r\n", dut0.g_pattern_smbus_control_buf[11]);

				   for(i=1; i<31; i++)
				   {
					   dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				   dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;


				   ////////xil_printf("dut0.g_calibrate_signbit =%x\r\n", dut0.g_calibrate_signbit);
				   ////////xil_printf("dut0.g_calibration_offset =%x\r\n", dut0.g_calibration_offset);
				   ////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);
				   ////////xil_printf("dut0.g_pattern_smbus_control_buf[0] =%x\r\n", dut0.g_pattern_smbus_control_buf[0]);
				   ////////xil_printf("dut0_pattern5_ccdet_read 0xc1a9 data and calibration\r\n");
			   }



		break;
		}

		//qfn20/12 read 0x1049 data and save to efuse buf
		//qfn24 read 0x1048 data and save to efuse buf
		case 0x0006:
		{
			   dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			   dut0.g_pattern_smbus_control_buf[2] = 0x48;
			   dut0.g_pattern_smbus_control_buf[3] = 0x10;
			   dut0.g_pattern_smbus_control_buf[4] = 0x01;

			   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
			   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			   {
				   break;
			   }
			   else
			   {
				   dut0.g_vmon_calibration_data_buf[2] = dut0.g_pattern_smbus_control_buf[10];
				   dut0.g_vmon_calibration_data_buf[3] = dut0.g_pattern_smbus_control_buf[11];

				   dut0.g_vmoncompare_process_bit = vmon_process_done_pass;
				   dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut0.g_vmon_compare_step = 0;
			   }
		       //////////xil_printf("dut0.g_vmon_retest_signbit =%x\r\n", dut0.g_vmon_retest_signbit);
			   ////xil_printf("dut0.g_pattern_smbus_control_buf[10] =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
			   ////////xil_printf("dut0.g_pattern_smbus_control_buf[11] =%x\r\n", dut0.g_pattern_smbus_control_buf[11]);

			   //////////xil_printf("dut0.g_vmon_calibration_data_buf[2] =%x\r\n", dut0.g_vmon_calibration_data_buf[2]);
			   //////////xil_printf("dut0.g_vmon_calibration_data_buf[3] =%x\r\n", dut0.g_vmon_calibration_data_buf[3]);

			   //////////xil_printf("dut0.g_vmon_compare_step = %x\r\n",dut0.g_vmon_compare_step);


		break;
		}





	break;
	}

	}
	}
	////xil_printf("dut0.g_result_fail = %x\r\n",dut0.g_result_fail);
	//xil_printf("dut0.g_vmon_compare_step = %x\r\n\r\n",dut0.g_vmon_compare_step);

	return 0;
}

















