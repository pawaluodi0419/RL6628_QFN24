#include "platform.h"
#include "ccdet_calibration_dut.h"

u8 _ccdet_calibration_dut1()
{
	u8 i;
	switch(dut1.g_ccdet_process_bit)
	{
	case ccdet_process_waiting:
	{
		_by_mcp4822_output_control_dut1(dut1.g_dac_data_buf);
		msdelay(10);
    	dut1.g_pattern_step=0;
    	dut1.g_ccdet_process_bit = ccdet_process_doing;
    	//xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
    	//xil_printf("dut1.g_ccdet_process_bit = %x\r\n",dut1.g_ccdet_process_bit);
    	xil_printf("dut1_dac_writen =%x,%x,%x\r\n",dut1.g_dac_data_buf[0],dut1.g_dac_data_buf[1],dut1.g_dac_data_buf[2]);
		break;
	}
	case ccdet_process_doing:
	{
		switch(dut1.g_pattern_step)
	    {
		case 0x0000:
		{
			_by_mcp4822_output_control_dut1(dut1.g_dac_data_buf);
			msdelay(10);
	    	dut1.g_pattern_step++;
	    	dut1.g_ccdet_process_bit = ccdet_process_doing;
	    	////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
	    	////xil_printf("dut1_dac_writen =%x,%x,%x\r\n",dut1.g_dac_data_buf[0],dut1.g_dac_data_buf[1],dut1.g_dac_data_buf[2]);
			break;
		}
	    //select cc voltage
	    //smbus write to mem 0x1020
		case 0x0001:
		{
	       dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut1.g_pattern_smbus_control_buf[2] = 0x20;
		   dut1.g_pattern_smbus_control_buf[3] = 0x10;
		   dut1.g_pattern_smbus_control_buf[4] = 0x01;
		   dut1.g_pattern_smbus_control_buf[5] = dut1.g_ccdet_voltage_sel;

		   ////////xil_printf("dut1.g_pattern_smbus_control_buf[5] = %x\r\n",dut1.g_pattern_smbus_control_buf[5]);

		   smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		   if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<30; i++)
				   {
					   dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut1.g_pattern_step++;
		   }
		   ////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);



		break;
	}

	//write CC1/CC2 Calibration phy with default data
	case 0x0002:
	{
    	dut1.g_calibrate_signbit = 0x00;
    	dut1.g_calibration_offset = 0x00;

    	//write to phy 0x10f/0x110/0x111/0x112 with default data

		   dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut1.g_pattern_smbus_control_buf[2] = dut1.g_ccdet_phy_calibration_addr_buf[0];
		   dut1.g_pattern_smbus_control_buf[3] = dut1.g_ccdet_phy_calibration_addr_buf[1];
		   dut1.g_pattern_smbus_control_buf[4] = 0x01;
		   dut1.g_pattern_smbus_control_buf[5] = dut1.g_ccdet_calibration_data_buf[0];
		   dut1.g_pattern_smbus_control_buf[6] = dut1.g_ccdet_calibration_data_buf[1];


		   smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);

		   if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<30; i++)
				   {
					   dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			   		   dut1.g_pattern_step++;
		   }

			//////////xil_printf("dut1_pattern5_write_0x10f/0x110/0x111/0x112_default_dataL= %x\r\n",dut1.g_ccdet_calibration_data_buf[0]);
			//////////xil_printf("dut1_pattern5_write_0x10f/0x110/0x111/0x112_default_dataH= %x\r\n",dut1.g_ccdet_calibration_data_buf[1]);
			////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		    break;
	}

	   /****************************************
	    *CC_Det Calibration start!
	    *
	    *REG_CC1_REF_SELx/REG_CC2_REF_SELx control
	    *smbus write mem 0xc1a0 0x00
	    *CC1:0.66V/1.23V
	    *CC2:0.66V/1.23V
	    *
	    *
	    *
	    *
	    ******************************************/




	//read 0x1029 data and set calibration register signbit
	case 0x0003:
	{
		   dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		   dut1.g_pattern_smbus_control_buf[2] = 0x29;
		   dut1.g_pattern_smbus_control_buf[3] = 0x10;
		   dut1.g_pattern_smbus_control_buf[4] = 0x01;

		   smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		   if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {

			   ////////xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
			   if(((dut1.g_pattern_smbus_control_buf[10] >> dut1.g_ccdet_ana_result_bit) & 0x01) == 1)
			   {
				   dut1.g_calibrate_signbit = 0;
				   dut1.g_calibration_offset = 1;
				   dut1.g_pattern_step++;
			   }
			   else
			   {
				   dut1.g_calibrate_signbit = 1;
				   dut1.g_calibration_offset = 1;
				   dut1.g_pattern_step++;
			   }

			   for(i=0; i<30; i++)
			   {
				   dut1.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			   }
			       dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

		   }
		   ////////xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
		   ////////xil_printf("dut1.g_calibrate_signbit =%x\r\n", dut1.g_calibrate_signbit);
		   ////////xil_printf("dut1.g_calibration_offset =%x\r\n", dut1.g_calibration_offset);
	       ////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		   ////////xil_printf("dut1_pattern5_read_cc1_0xc1a9 = %x\r\n",dut1.g_pattern_smbus_control_buf[10]);
	break;
	}

    //write to calibration phy
	case 0x0004:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		dut1.g_pattern_smbus_control_buf[2] = dut1.g_ccdet_phy_calibration_addr_buf[0];
		dut1.g_pattern_smbus_control_buf[3] = dut1.g_ccdet_phy_calibration_addr_buf[1];
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		if(dut1.g_calibrate_signbit == 0)
		{
       			if((dut1.g_calibration_offset >=0) && (dut1.g_calibration_offset <= 7))
			{
				if((dut1.g_ccdet_phy_calibration_bit >= 0) && (dut1.g_ccdet_phy_calibration_bit < 2))
				{
			        dut1.g_pattern_smbus_control_buf[5] = (dut1.g_ccdet_calibration_data_buf[0]+(dut1.g_calibration_offset<<4*dut1.g_ccdet_phy_calibration_bit));
			        dut1.g_pattern_smbus_control_buf[6] = dut1.g_ccdet_calibration_data_buf[1];
				}
				else if((dut1.g_ccdet_phy_calibration_bit >= 2) && (dut1.g_ccdet_phy_calibration_bit < 4))
				{
					dut1.g_pattern_smbus_control_buf[5] = dut1.g_ccdet_calibration_data_buf[0];
					dut1.g_pattern_smbus_control_buf[6] = (dut1.g_ccdet_calibration_data_buf[1]+(dut1.g_calibration_offset<<4*(dut1.g_ccdet_phy_calibration_bit-2)));
				}
				    //////xil_printf("\r\ndut1.g_ccdet_calibration_data_buf[0] =%x\r\n", dut1.g_ccdet_calibration_data_buf[0]);
				    //////xil_printf("dut1.g_ccdet_calibration_data_buf[1] =%x\r\n", dut1.g_ccdet_calibration_data_buf[1]);
				    //////xil_printf("dut1.g_pattern_smbus_control_buf[5] = %x\r\n", dut1.g_pattern_smbus_control_buf[5]);
				    //////xil_printf("dut1.g_pattern_smbus_control_buf[6] = %x\r\n", dut1.g_pattern_smbus_control_buf[6]);
		}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else if(dut1.g_calibrate_signbit == 1)
		{
			if((dut1.g_calibration_offset >=0) && (dut1.g_calibration_offset <= 8))
			{
				if((dut1.g_ccdet_phy_calibration_bit >= 0) && (dut1.g_ccdet_phy_calibration_bit < 2))
				{
			        dut1.g_pattern_smbus_control_buf[5] = (dut1.g_ccdet_calibration_data_buf[0]-(dut1.g_calibration_offset<<(4*dut1.g_ccdet_phy_calibration_bit)));
			        dut1.g_pattern_smbus_control_buf[6] = dut1.g_ccdet_calibration_data_buf[1];
				}
				else if((dut1.g_ccdet_phy_calibration_bit >= 2) && (dut1.g_ccdet_phy_calibration_bit < 4))
				{
					dut1.g_pattern_smbus_control_buf[5] = dut1.g_ccdet_calibration_data_buf[0];
					dut1.g_pattern_smbus_control_buf[6] = (dut1.g_ccdet_calibration_data_buf[1]-(dut1.g_calibration_offset<<(4*(dut1.g_ccdet_phy_calibration_bit-2))));;
				}
			       //////xil_printf("\r\ndut1.g_ccdet_calibration_data_buf[0] =%x\r\n", dut1.g_ccdet_calibration_data_buf[0]);
			       //////xil_printf("dut1.g_ccdet_calibration_data_buf[1] =%x\r\n", dut1.g_ccdet_calibration_data_buf[1]);
			       //////xil_printf("dut1.g_pattern_smbus_control_buf[5] =%x\r\n", dut1.g_pattern_smbus_control_buf[5]);
			       //////xil_printf("dut1.g_pattern_smbus_control_buf[6] =%x\r\n\r\n", dut1.g_pattern_smbus_control_buf[6]);
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{

		}
		else
		{
			for(i=1; i<30; i++)
			{
				dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}
		   ////////xil_printf("dut1.g_ccdet_retest_signbit =%x\r\n", dut1.g_ccdet_retest_signbit);
		   //////////xil_printf("dut1.g_ccdet_calibration_data_buf[0] =%x\r\n", dut1.g_ccdet_calibration_data_buf[0]);
		   //////////xil_printf("dut1.g_ccdet_calibration_data_buf[1] =%x\r\n", dut1.g_ccdet_calibration_data_buf[1]);
		   //////////xil_printf("dut1.g_pattern_smbus_control_buf[5] =%x\r\n", dut1.g_pattern_smbus_control_buf[5]);
		   //////////xil_printf("dut1.g_pattern_smbus_control_buf[6] =%x\r\n", dut1.g_pattern_smbus_control_buf[6]);
		   ////////xil_printf("dut1.g_calibrate_signbit =%x\r\n", dut1.g_calibrate_signbit);
		   ////////xil_printf("dut1.g_calibration_offset =%x\r\n", dut1.g_calibration_offset);
	       ////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		   ////////xil_printf("dut1_pattern5_cc_write_to_0x109/a/b/c\r\n");



		break;
	}

	//read 0xc1a9 data and calibration
	case 0x0005:
	{
		   dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		   dut1.g_pattern_smbus_control_buf[2] = 0x29;
		   dut1.g_pattern_smbus_control_buf[3] = 0x10;
		   dut1.g_pattern_smbus_control_buf[4] = 0x01;

		   smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		   ////////xil_printf("\r\n dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
		   ////////xil_printf("\r\n dut1.g_pattern_smbus_control_buf[11] =%x\r\n", dut1.g_pattern_smbus_control_buf[11]);
		   if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {

			   if(((dut1.g_pattern_smbus_control_buf[10] >> dut1.g_ccdet_ana_result_bit) & 0x01) == 1)
			   {
				  if(dut1.g_calibrate_signbit == 0)
				  {
					  if((dut1.g_calibration_offset >=0) && (dut1.g_calibration_offset < 7))
					  {
						  dut1.g_calibration_offset++;
						  dut1.g_pattern_step--;
					  }
					  else if((dut1.g_calibration_offset == 7) && (dut1.g_ccdet_retest_signbit == 0))
					  {
						  dut1.g_ccdet_retest_signbit = lower_voltage_retest;
						  dut1.g_dac_data_buf[1] = dut1.g_dac_data_buf[3];
						  dut1.g_dac_data_buf[2] = dut1.g_dac_data_buf[4];
					      dut1.g_pattern_step = 0;

					  }
					  else
					  {
						  dut1.g_result_fail = 0x01;
						  dut1.g_result_fail_tmrcount = 0xffff;
					  }
				  }

				  else if(dut1.g_calibrate_signbit == 1)
				  {
					  if((dut1.g_calibration_offset >=0) && (dut1.g_calibration_offset <= 8))
					  {
					  dut1.g_pattern_step++;

					  }
					  else
					  {
						dut1.g_result_fail = 0x01;
						dut1.g_result_fail_tmrcount = 0xffff;
					  }
				  }
			   }


			   else if(((dut1.g_pattern_smbus_control_buf[10] >> dut1.g_ccdet_ana_result_bit) & 0x01) == 0)
			   {
				  if(dut1.g_calibrate_signbit == 1)
				  {
					  if((dut1.g_calibration_offset >=0) && (dut1.g_calibration_offset < 8))
					  {
						  dut1.g_calibration_offset++;
						  dut1.g_pattern_step--;
					  }
					  else if((dut1.g_calibration_offset == 8)&& (dut1.g_ccdet_retest_signbit == 0))
					  {
						  dut1.g_ccdet_retest_signbit = higer_voltage_retest;
						  dut1.g_dac_data_buf[1] = dut1.g_dac_data_buf[5];
						  dut1.g_dac_data_buf[2] = dut1.g_dac_data_buf[6];
					      dut1.g_pattern_step = 0;

					  }
					  else
					  {
						  dut1.g_result_fail = 0x01;
						  dut1.g_result_fail_tmrcount = 0xffff;
					  }
				  }

				  else if(dut1.g_calibrate_signbit == 0)
				  {
					  if((dut1.g_calibration_offset >=0) && (dut1.g_calibration_offset <= 7))
					  {
					  dut1.g_pattern_step++;

					  }
					  else
					  {
						dut1.g_result_fail = 0x01;
						dut1.g_result_fail_tmrcount = 0xffff;
					  }
				  }
			   }

			   for(i=1; i<30; i++)
			   {
				   dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }
			       dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

		   }


		   ////////xil_printf("\r\n dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
		   ////////xil_printf("\r\n dut1.g_pattern_smbus_control_buf[11] =%x\r\n", dut1.g_pattern_smbus_control_buf[11]);
		   ////////xil_printf("dut1.g_calibrate_signbit =%x\r\n", dut1.g_calibrate_signbit);
		   ////xil_printf("dut1.g_calibration_offset =%x\r\n", dut1.g_calibration_offset);
		   ////xil_printf("dut1.g_ccdet_retest_signbit =%x\r\n", dut1.g_ccdet_retest_signbit);
		   ////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		   ////////xil_printf("dut1.g_pattern_smbus_control_buf[0] =%x\r\n", dut1.g_pattern_smbus_control_buf[0]);
		   ////////xil_printf("dut1_pattern5_ccdet_read 0xc1a9 data and calibration\r\n");
	break;
	}


	//read 0x109 data and save to efuse buf
	case 0x0006:
	{
		   dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readphy;
		   dut1.g_pattern_smbus_control_buf[2] = dut1.g_ccdet_phy_calibration_addr_buf[0];;
		   dut1.g_pattern_smbus_control_buf[3] = dut1.g_ccdet_phy_calibration_addr_buf[1];;
		   dut1.g_pattern_smbus_control_buf[4] = 0x01;

		   smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		   if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   if(dut1.g_ccdet_retest_signbit == 0)
			   {
			   dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10];
			   dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11];
			   }


			   if(dut1.g_ccdet_retest_signbit == lower_voltage_retest)
			   {
				   if((dut1.g_ccdet_phy_calibration_bit >= 0) && (dut1.g_ccdet_phy_calibration_bit < 2))
				   {
					   //dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10]|(0x0f<<(4*dut1.g_ccdet_phy_calibration_bit));
					   //dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11];
					   dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10];
					   dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11];
				   }
				   if((dut1.g_ccdet_phy_calibration_bit >= 2) && (dut1.g_ccdet_phy_calibration_bit < 4))
				   {
					  // dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10];
					   //dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11]|(0x0f<<(4*(dut1.g_ccdet_phy_calibration_bit-2)));
					   dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10];
					   dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11];
				   }
			   }

			   if(dut1.g_ccdet_retest_signbit == higer_voltage_retest)
			   {
				   if((dut1.g_ccdet_phy_calibration_bit >= 0) && (dut1.g_ccdet_phy_calibration_bit < 2))
				   {
					   //dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10]& ~(0x0f<<(4*dut1.g_ccdet_phy_calibration_bit));
					   //dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11];
					   dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10];
					   dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11];
				   }
				   if((dut1.g_ccdet_phy_calibration_bit >= 2) && (dut1.g_ccdet_phy_calibration_bit < 4))
				   {
					   //dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10];
					   //dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11]& ~(0x0f<<(4*(dut1.g_ccdet_phy_calibration_bit-2)));
					   dut1.g_ccdet_calibration_data_buf[2] = dut1.g_pattern_smbus_control_buf[10];
					   dut1.g_ccdet_calibration_data_buf[3] = dut1.g_pattern_smbus_control_buf[11];
				   }

			       //xil_printf("dut1.g_ccdet_retest_signbit =%x\r\n", dut1.g_ccdet_retest_signbit);
			       //xil_printf("dut1.g_ccdet_phy_calibration_bit =%x\r\n", dut1.g_ccdet_phy_calibration_bit);

				   //xil_printf("dut1.g_pattern_smbus_control_buf[10] =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
				   //xil_printf("dut1.g_pattern_smbus_control_buf[11] =%x\r\n", dut1.g_pattern_smbus_control_buf[11]);

				   //xil_printf("dut1.g_ccdet_calibration_data_buf[2] =%x\r\n", dut1.g_ccdet_calibration_data_buf[2]);
				   //xil_printf("dut1.g_ccdet_calibration_data_buf[3] =%x\r\n", dut1.g_ccdet_calibration_data_buf[3]);
			   }


			   dut1.g_ccdet_process_bit = ccdet_process_done_pass;
			   dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			   dut1.g_pattern_step = 0;

		   }


		   //////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		   //////////xil_printf("dut1_pattern2_read_cc_calibration_data_save_to_temp_efuse\r\n\r\n");

	break;
	}

	}

	break;
	}

	}

	return 0;
}

















