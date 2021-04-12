#include "platform.h"
#include "Pattern5_LDO3V3.h"

//dut1 Pattern 5��LDO3V3 Calibration and test
u8 _by_Pattern5_LDO3V3calibration_dut1()
{
	u8 i;
	u16 voltage_ldo3v3;
	u16 ldo3v3_ref;
	u16 ldo3v3_min;
	u16 ldo3v3_max;
	u16 voltage_delta;
	u16 voltage_delta_temp;

	switch(dut1.g_pattern_step)
	{
	//smbus_write_mem 0x1117 data:0x1b
	case 0x0000:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut1.g_pattern_smbus_control_buf[2] = 0x17;
		dut1.g_pattern_smbus_control_buf[3] = 0x11;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;
		dut1.g_pattern_smbus_control_buf[5] = 0x1b;
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
		////xil_printf("dut1_rp_g_pattern_step = %x\r\n", dut1.g_pattern_step);
		//////xil_printf("dut1_pattern2_write_0x1131_with_0xc0\r\n");
		break;
	}




	//write to LDO3V3 phy 0x11a bit[10:8] with medium data 3b100
	case 0x0001:
	{
    	dut1.g_dut_pattern_status_buf[30] = 0x00;
    	dut1.g_dut_pattern_status_buf[31] = 0x00;

		   dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut1.g_pattern_smbus_control_buf[2] = 0x1a;
		   dut1.g_pattern_smbus_control_buf[3] = 0x01;
		   dut1.g_pattern_smbus_control_buf[4] = 0x01;
		   dut1.g_pattern_smbus_control_buf[5] = 0x00;
		   dut1.g_pattern_smbus_control_buf[6] = 0x18;

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
				       //xil_printf("dut1.ldo3v3_g_pattern_step = %x\r\n\r\n", dut1.g_pattern_step);
			   		   dut1.g_pattern_step++;
		   }

			//////xil_printf("dut1_pattern4_write_0x11a_default_data\r\n");
		break;
	}





	   /******************************************
	    *LDO3V3 Calibration start!
	    *
	    *
	    *
	    *AD7994:
	    * D7-4:0001   CH0   CC1
	    * D7-4:0010   CH1   CC2
	    * D7-4:0100   CH2   3V3/2
	    * D7-4:1000   CH3   1V2
	    ******************************************/
	   /******************************************
	    *RL6628 LDO3V3�ֲ��������ŵ�λ�������Ա仯
	    *����Ϊ�ο�ֵ
	    *100: 3170mv
	    *101: 3215mv
	    *110: 3260mv
	    *111: 3305mv
	    *000��3350mv��default��
	    *001: 3395mv
	    *010: 3440mv
	    *011: 3485mv

	    *
	    ******************************************/


	//Read LDO3V3 Voltage from AD7994 CH3
	case 0x0002:
	{
		_byAD7994ReadCvsData_dut1(AD7994_ADC1_SLAVE_ADDR,0x05, 10);

    	voltage_ldo3v3 = dut1.g_as7994Cvsdata *2;

    	ldo3v3_ref  = (u16)4095*3.30/2.5;
    	ldo3v3_min  = (u16)4095*3.15/2.5;
    	ldo3v3_max  = (u16)4095*3.45/2.5;


    	dut1.g_dut_pattern_status_buf[9] = (voltage_ldo3v3 & 0xFF);
    	dut1.g_dut_pattern_status_buf[10] = (voltage_ldo3v3 & 0xFF00)>>8;
    	dut1.g_dut_pattern_status_buf[11] = (ldo3v3_ref & 0xFF);
    	dut1.g_dut_pattern_status_buf[12] = (ldo3v3_ref & 0xFF00)>>8;
    	dut1.g_dut_pattern_status_buf[13] = (ldo3v3_min & 0xFF);
    	dut1.g_dut_pattern_status_buf[14] = (ldo3v3_min & 0xFF00)>>8;
    	dut1.g_dut_pattern_status_buf[15] = (ldo3v3_max & 0xFF);
    	dut1.g_dut_pattern_status_buf[16] = (ldo3v3_max & 0xFF00)>>8;


 	    //xil_printf("dut1.g_as7994Cvsdata = %x\r\n", dut1.g_as7994Cvsdata);
		//xil_printf("voltage_ldo3v3 = %x\r\n", voltage_ldo3v3);
	    //xil_printf("ldo3v3_ref = %x\r\n",ldo3v3_ref );
	    //xil_printf("ldo3v3_min = %x\r\n",ldo3v3_min );
	    //xil_printf("ldo3v3_max = %x\r\n",ldo3v3_max );

	    dut1.g_dut_pattern_status_buf[30] = 0x00;
	    dut1.g_dut_pattern_status_buf[31] = 0x00;



	    if(voltage_ldo3v3 < ldo3v3_ref)
	    {
	    	voltage_delta = ldo3v3_ref - voltage_ldo3v3;
	    	dut1.g_dut_pattern_status_buf[17] = (voltage_delta & 0xFF);
	    	dut1.g_dut_pattern_status_buf[18] = ((voltage_delta & 0xFF00)>>8);

		    dut1.g_dut_pattern_status_buf[30] = 0x00;
		    dut1.g_dut_pattern_status_buf[31]++;
	    }
	    else
	    {
	    	voltage_delta = voltage_ldo3v3 - ldo3v3_ref;
	    	dut1.g_dut_pattern_status_buf[17] = (voltage_delta & 0xFF);
	    	dut1.g_dut_pattern_status_buf[18] = ((voltage_delta & 0xFF00)>>8);

		    dut1.g_dut_pattern_status_buf[30] = 0x01;
		    dut1.g_dut_pattern_status_buf[31]++;
	    }
	    //xil_printf("dut1.g_dut_pattern_status_buf[30] = %x\r\n",dut1.g_dut_pattern_status_buf[30] );
	    //xil_printf("dut1.g_dut_pattern_status_buf[31] = %x\r\n",dut1.g_dut_pattern_status_buf[31] );
	    //xil_printf("dut1.ldo3v3_g_pattern_step = %x\r\n\r\n", dut1.g_pattern_step);
		//////xil_printf("dut1_pattern4_read_LDO3V3\r\n");

    	dut1.g_pattern_step++;
		break;
	}


	case 0x0003:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		dut1.g_pattern_smbus_control_buf[2] = 0x1a;
		dut1.g_pattern_smbus_control_buf[3] = 0x01;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;
		dut1.g_pattern_smbus_control_buf[5] = 0x00;

		if((dut1.g_dut_pattern_status_buf[30] == 0) && (dut1.g_dut_pattern_status_buf[31] <= 3))
		{
			//ƫ�Ƶ�����Ϊ4bit����ʼֵΪ1000�����ȡ����3bit��������Ӧ��ƫ��ֵ�仯������
			dut1.g_pattern_smbus_control_buf[6] = (0x18|((0x08+dut1.g_dut_pattern_status_buf[31])&0x07));
		}
		else if((dut1.g_dut_pattern_status_buf[30] == 1) && (dut1.g_dut_pattern_status_buf[31] <= 4))
		{

			dut1.g_pattern_smbus_control_buf[6] = (0x18|((0x08-dut1.g_dut_pattern_status_buf[31])&0x07));
		}
		else
		{
			dut1.g_result_fail = 0x01;
			dut1.g_result_fail_tmrcount = 0xffff;
		}

	    //xil_printf("dut1.g_pattern_smbus_control_buf[6] = %x\r\n",dut1.g_pattern_smbus_control_buf[6] );
	    //xil_printf("dut1.g_dut_pattern_status_buf[30] = %x\r\n",dut1.g_dut_pattern_status_buf[30] );
	    //xil_printf("dut1.g_dut_pattern_status_buf[31] = %x\r\n",dut1.g_dut_pattern_status_buf[31] );

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
			//xil_printf("dut1_pattern4_write_ldo3v3_0x119\r\n");
			//xil_printf("dut1.ldo3v3_g_pattern_step = %x\r\n\r\n", dut1.g_pattern_step);

			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}


		break;
	}


	case 0x0004:
	{
		_byAD7994ReadCvsData_dut1(AD7994_ADC1_SLAVE_ADDR,0x05, 10);

    	dut1.g_dut_pattern_status_buf[9] = (dut1.g_as7994Cvsdata *2) & 0xFF;
    	dut1.g_dut_pattern_status_buf[10] = ((dut1.g_as7994Cvsdata *2) & 0xFF00)>>8;

    	voltage_ldo3v3 = (u16)dut1.g_as7994Cvsdata *2;
    	ldo3v3_ref = (u16)(dut1.g_dut_pattern_status_buf[12]<<8 | dut1.g_dut_pattern_status_buf[11]);
    	ldo3v3_min = (u16)(dut1.g_dut_pattern_status_buf[14]<<8 | dut1.g_dut_pattern_status_buf[13]);
    	ldo3v3_max = (u16)(dut1.g_dut_pattern_status_buf[16]<<8 | dut1.g_dut_pattern_status_buf[15]);

    	   //xil_printf("dut1.g_as7994Cvsdata = %x\r\n", dut1.g_as7994Cvsdata);
		   //xil_printf("voltage_ldo3v3 = %x\r\n", voltage_ldo3v3);
	       //xil_printf("ldo3v3_ref = %x\r\n",ldo3v3_ref );
	       //xil_printf("ldo3v3_min = %x\r\n",ldo3v3_min );
	       //xil_printf("ldo3v3_max = %x\r\n",ldo3v3_max );
	       //xil_printf("dut1.g_dut_pattern_status_buf[18] = %x\r\n",dut1.g_dut_pattern_status_buf[18] );
	       //xil_printf("dut1.g_dut_pattern_status_buf[17] = %x\r\n",dut1.g_dut_pattern_status_buf[17] );

	   //ldo3v3 calibration

	    if(voltage_ldo3v3 > ldo3v3_ref)
	    {
	    	voltage_delta = (u16)(dut1.g_dut_pattern_status_buf[18]<<8 | dut1.g_dut_pattern_status_buf[17]);
	    	voltage_delta_temp = voltage_ldo3v3 - ldo3v3_ref;
	    	dut1.g_dut_pattern_status_buf[17] = (voltage_delta_temp & 0xFF);
	    	dut1.g_dut_pattern_status_buf[18] = (voltage_delta_temp & 0xFF00)>>8;
	    	if(dut1.g_dut_pattern_status_buf[30] == 0x01)
	    	{
	    		if((dut1.g_dut_pattern_status_buf[31] >= 0) && (dut1.g_dut_pattern_status_buf[31]<4))
	    		{

	    			dut1.g_dut_pattern_status_buf[31]++;
	    			dut1.g_pattern_step--;
	    		}
	    		else if(dut1.g_dut_pattern_status_buf[31] == 4)
	    		{
	    			if((voltage_ldo3v3 > ldo3v3_min) && (voltage_ldo3v3 < ldo3v3_max))
	    			{
	    				dut1.g_efuse_valid_ldo3v3 = 1;
	    				dut1.g_efuse_data_ldo3v3 = 0x04;
	    				dut1.g_pattern_step++;
	    				//xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3);
	    			}
	    			else
	    			{
	    				dut1.g_result_fail = 0x01;
	    				dut1.g_result_fail_tmrcount = 0xffff;
	    			}

	    		}
	    		else
	    		{
	    			   dut1.g_result_fail = 0x01;
	    			   dut1.g_result_fail_tmrcount = 0xffff;
	    		}


	    	}
	    	if(dut1.g_dut_pattern_status_buf[30] == 0x00)
	    	{
	    		if((dut1.g_dut_pattern_status_buf[31] > 0) && (dut1.g_dut_pattern_status_buf[31] <= 3))
		    {

	    			if(voltage_delta_temp > voltage_delta)
	    			{
	    				dut1.g_efuse_valid_ldo3v3 = 0x01;
	    				dut1.g_efuse_data_ldo3v3 = ((0x08 + dut1.g_dut_pattern_status_buf[31]-1)&0x07);
	    				dut1.g_pattern_step++;
	    			    //xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3);
	    			}
	    			else
	    			{
	    				dut1.g_efuse_valid_ldo3v3 = 0x01;
	    				dut1.g_efuse_data_ldo3v3 = ((0x08 + dut1.g_dut_pattern_status_buf[31])&0x07);
	    				dut1.g_pattern_step++;
	    			    //xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3);
	    			}

		    }
	    	else
	    	{
	    				dut1.g_result_fail = 0x01;
	    				dut1.g_result_fail_tmrcount = 0xffff;
	    	}


	    	}
	    }



	    if(voltage_ldo3v3 <= ldo3v3_ref)
	    {
    		voltage_delta = (u16)(dut1.g_dut_pattern_status_buf[18]<<8 | dut1.g_dut_pattern_status_buf[17]);
	    	voltage_delta_temp = ldo3v3_ref - voltage_ldo3v3;
	    	dut1.g_dut_pattern_status_buf[17] = (voltage_delta_temp & 0xFF);
	    	dut1.g_dut_pattern_status_buf[18] = (voltage_delta_temp & 0xFF00)>>8;
	    	if(dut1.g_dut_pattern_status_buf[30] == 0x00)
	    	{
	    		if((dut1.g_dut_pattern_status_buf[31] >= 0) && (dut1.g_dut_pattern_status_buf[31]<3))
	    		{
	    			dut1.g_dut_pattern_status_buf[31]++;
	    			dut1.g_pattern_step--;
	    		}
	    		else if(dut1.g_dut_pattern_status_buf[31] == 3)
	    		{
	    			if((voltage_ldo3v3 > ldo3v3_min) && (voltage_ldo3v3 < ldo3v3_max))
	    			{
	    				dut1.g_efuse_valid_ldo3v3 = 0x01;
	    				dut1.g_efuse_data_ldo3v3 = 0x03;
	    				dut1.g_pattern_step++;
	    			    //xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3);
	    			}
	    			else
	    			{
	    				dut1.g_result_fail = 0x01;
	    				dut1.g_result_fail_tmrcount = 0xffff;
	    			}

	    		}
	    		else
	    		{
	    			   dut1.g_result_fail = 0x01;
	    			   dut1.g_result_fail_tmrcount = 0xffff;
	    		}


	    	}
	    	if(dut1.g_dut_pattern_status_buf[30] == 0x01)
	    	{
	    		if((dut1.g_dut_pattern_status_buf[31] > 0) && (dut1.g_dut_pattern_status_buf[31] <= 4))
		    {

	    			if(voltage_delta_temp > voltage_delta)
	    			{
	    				dut1.g_efuse_valid_ldo3v3 = 0x01;
	    				dut1.g_efuse_data_ldo3v3 = ((0x08 -(dut1.g_dut_pattern_status_buf[31]-1))&0x07);
	    				dut1.g_pattern_step++;
	    			    //xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3);
	    			}
	    			else
	    			{
	    				dut1.g_efuse_valid_ldo3v3 = 0x01;
	    				dut1.g_efuse_data_ldo3v3 = ((0x08 - dut1.g_dut_pattern_status_buf[31])&0x07);
	    				dut1.g_pattern_step++;
	    			    //xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3);
	    			}
    			    //xil_printf("dut1.g_uartPatternNum = %x\r\n\r\n",dut1.g_uartPatternNum);
		    }
	    	else
	    	{
	    				dut1.g_result_fail = 0x01;
	    				dut1.g_result_fail_tmrcount = 0xffff;
	    	}


	    	}
	    }

	    break;
	}

	case 0x0005:
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

			    xil_printf("dut1_pattern5_ldo3v3_calibration_pass! ldo3v3_calibration_data= 0x%x\r\n\r\n",dut1.g_efuse_data_ldo3v3);

			    ////xil_printf("voltage_delta_temp = %x\r\n",voltage_delta_temp );
			    ////xil_printf("voltage_delta = %x\r\n",voltage_delta );
			    ////xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3 );
			    ////xil_printf("dut1.g_result_fail = %x\r\n",dut1.g_result_fail );
			    ////xil_printf("dut1_pattern5_ldo3v3_step = %x\r\n\r\n", dut1.g_pattern_step);
			    //////xil_printf("dut1.g_efuse_valid_ldo3v3 = %x\r\n",dut1.g_efuse_valid_ldo3v3 );
			    //////xil_printf("dut1.g_efuse_data_buf[15] = %x\r\n",dut1.g_efuse_data_buf[15] );
			    ////xil_printf("dut1_pattern4_ldo3v3_calibration\r\n\r\n");
			    //////xil_printf("dut1.g_dut_pattern_status_buf[2] = %x\r\n", dut1.g_dut_pattern_status_buf[2]);
		       ////xil_printf("dut1.g_uartPatternNum = %x\r\n\r\n",dut1.g_uartPatternNum);
		       ////xil_printf("dut1.g_efuse_data_ldo3v3 = %x\r\n",dut1.g_efuse_data_ldo3v3 );
		       //////xil_printf("dut1_pattern4_write_0x11a_default_data\r\n");
		       break;
	}




	}
	if(dut1.g_result_fail == 0x01)
	{
		dut1.g_ft2_test_done = 0x00;
		pattern_safe_state_dut1();	//add safe state
		xil_printf("dut1_pattern5_fail! fail_step = %x\r\n\r\n",dut1.g_pattern_step);
	}
			//xil_printf("dut1_pattern5_ldo3v3_step = %x\r\n\r\n",dut1.g_pattern_step);
			return 0;
}

















