#include "platform.h"
#include "Pattern3_RP.h"

u8 _by_Pattern3_RPcalibration_dut2()
{
	u8 i;
	u16 voltage_ldo3v3;
	u16 rp12k_ref;
	u16 rp12k_min;
	u16 rp12k_max;
	u16 rp36k_min;
	u16 rp36k_max;
	u16 rp4p7k_max;
	u16 rp4p7k_min;

	u16 cc1_rp_voltage;
	u16 cc2_rp_voltage;
	u16 cc_rp_voltage;

	u8 cc1_rp12k_offset;
	u8 Buff_dut2_XGPIO_0[8];

	switch(dut2.g_pattern_step)
	{
	case 0x0000:
	{
		//gpio17: low bit, gpio20: high bit
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,17, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,18, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,19, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,20, 0);
		msdelay(5);

		//enable ADG1604
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,13, 1);	//for CC1
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,14, 1);	//for CC2
		msdelay(5);
		//select ADG1604 S1 channel
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,9, 0);	//for CC1
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,10, 0);

		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,24, 0);	//for CC2
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,27, 0);
		msdelay(5);

		dut2.g_pattern_step = 0x100;
	}

	//smbus_write_mem 0xC1AE 1 0x00
	//disconnect CC from BMC PHY//
	case 0x0100:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut2.g_pattern_smbus_control_buf[2] = 0xAE;
		   dut2.g_pattern_smbus_control_buf[3] = 0xC1;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   dut2.g_pattern_smbus_control_buf[5] = 0x00;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   //////////////xil_printf("\r\n dut2_pattern2_0xC1AE_be_writen\r\n");
		   //////xil_printf("dut2_pattern3_step = %x\r\n", dut2.g_pattern_step);
		   break;
	}

	//smbus_write_mem 0xC108 1 0x88
	case 0x0101:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut2.g_pattern_smbus_control_buf[2] = 0x08;
		   dut2.g_pattern_smbus_control_buf[3] = 0xC1;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   dut2.g_pattern_smbus_control_buf[5] = 0x88;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   //////////////xil_printf("dut2_pattern2_0xC108_be_writen\r\n");
		   //////xil_printf("dut2_pattern3_step = %x\r\n", dut2.g_pattern_step);
		   break;
	}

	//smbus_write_mem 0x1022/1023 1 0x46
	case 0x0102:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut2.g_pattern_smbus_control_buf[2] = 0x22;
		   dut2.g_pattern_smbus_control_buf[3] = 0x10;
		   dut2.g_pattern_smbus_control_buf[4] = 0x02;
		   dut2.g_pattern_smbus_control_buf[5] = 0x46;
		   dut2.g_pattern_smbus_control_buf[6] = 0x46;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   //////xil_printf("dut2_pattern3_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern2_0x1022/1023_be_writen\r\n\r\n");
		   break;
	}

	//smbus_write_mem 0x1117 data:0x1b
	case 0x0103:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x17;
		dut2.g_pattern_smbus_control_buf[3] = 0x11;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x1b;
		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step = 0x0200;
		}
		////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		//////xil_printf("dut2_pattern2_write_0x1131_with_0xc0\r\n");
		break;
	}
	   /****************************************
	    *RP_12K Calibration start!
	    *
	    *
	    *
	    *AD7994:
	    * D7-4:0001   CH0   CC1
	    * D7-4:0010   CH1   CC2
	    * D7-4:0100   CH2   3V3/2
	    * D7-4:1000   CH3   1V2
	    ******************************************/



	//Read LDO3V3 Voltage from AD7994 CH3
	case 0x0200:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x05, 5);

    	voltage_ldo3v3 = dut2.g_as7994Cvsdata *2;     //6628 ldo3v3 NO output pin
    	//voltage_ldo3v3 = 0x0a8e *2;       //6628 define Ldo3v3 data,just for Rp test
    	xil_printf("dut2_voltage_ldo3v3 = %x\r\n", voltage_ldo3v3);
    	rp12k_ref  = 51/((120*1.000+51))*voltage_ldo3v3;
    	rp12k_min  = 51/(120*1.05+51)*voltage_ldo3v3;
    	rp12k_max  = 51/(120*0.95+51)*voltage_ldo3v3;
    	rp4p7k_min = 51/(47*1.05+51)*voltage_ldo3v3;
    	rp4p7k_max = 51/(47*0.95+51)*voltage_ldo3v3;
    	rp36k_min  = 51/(360*1.085+51)*voltage_ldo3v3;
    	rp36k_max  = 51/(360*0.915+51)*voltage_ldo3v3;

    	dut2.g_dut_pattern_status_buf[9] = (voltage_ldo3v3 & 0xFF);
    	dut2.g_dut_pattern_status_buf[10] = (voltage_ldo3v3 & 0xFF00)>>8;

    	dut2.g_dut_pattern_status_buf[11] = (rp12k_ref & 0xFF);
    	dut2.g_dut_pattern_status_buf[12] = (rp12k_ref & 0xFF00)>>8;
    	dut2.g_dut_pattern_status_buf[13] = (rp12k_min & 0xFF);
    	dut2.g_dut_pattern_status_buf[14] = (rp12k_min & 0xFF00)>>8;
    	dut2.g_dut_pattern_status_buf[15] = (rp12k_max & 0xFF);
    	dut2.g_dut_pattern_status_buf[16] = (rp12k_max & 0xFF00)>>8;

    	dut2.g_dut_pattern_status_buf[17] = (rp4p7k_min & 0xFF);
    	dut2.g_dut_pattern_status_buf[18] = (rp4p7k_min & 0xFF00)>>8;
    	dut2.g_dut_pattern_status_buf[19] = (rp4p7k_max & 0xFF);
    	dut2.g_dut_pattern_status_buf[20] = (rp4p7k_max & 0xFF00)>>8;

    	dut2.g_dut_pattern_status_buf[21] = (rp36k_min & 0xFF);
    	dut2.g_dut_pattern_status_buf[22] = (rp36k_min & 0xFF00)>>8;
    	dut2.g_dut_pattern_status_buf[23] = (rp36k_max & 0xFF);
    	dut2.g_dut_pattern_status_buf[24] = (rp36k_max & 0xFF00)>>8;

    	dut2.g_pattern_step++;
		//////////////xil_printf("dut2_pattern2_read_LDO3V3\r\n\r\n");
    	//////xil_printf("dut2_pattern3_step = %x\r\n", dut2.g_pattern_step);
		break;
	}

	case 0x0201:
	{
    	dut2.g_dut_pattern_status_buf[30] = 0x00;
    	dut2.g_dut_pattern_status_buf[31] = 0x00;

    	//write to phy 0x106 default data

		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0c;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   dut2.g_pattern_smbus_control_buf[5] = 0xef;
		   dut2.g_pattern_smbus_control_buf[6] = 0x01;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			   		   dut2.g_pattern_step++;
		   }
		   //////xil_printf("dut2_pattern3_step = %x\r\n", dut2.g_pattern_step);
			//////////////xil_printf("dut2_pattern2_write_0x106_default_data\r\n");
		break;
	}

	case 0x0202:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x01, 5);

    	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
    	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;

    	cc1_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	rp12k_ref = (u16)(dut2.g_dut_pattern_status_buf[12]<<8 | dut2.g_dut_pattern_status_buf[11]);
    	rp12k_min = (u16)(dut2.g_dut_pattern_status_buf[14]<<8 | dut2.g_dut_pattern_status_buf[13]);
    	rp12k_max = (u16)(dut2.g_dut_pattern_status_buf[16]<<8 | dut2.g_dut_pattern_status_buf[15]);

    	   //////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
		   //////////////xil_printf("cc1_rp12k_voltage = %d\r\n", cc1_rp_voltage);
	       //////////////xil_printf("rp12k_ref = %d\r\n",rp12k_ref );
	       //////////////xil_printf("rp12k_min = %d\r\n",rp12k_min );
	       //////////////xil_printf("rp12k_max = %d\r\n",rp12k_max );
		   //////////////xil_printf("dut2.g_dut_pattern_status_buf[30] =%d\r\n", dut2.g_dut_pattern_status_buf[30]);
		   //////////////xil_printf("dut2.g_dut_pattern_status_buf[31] =%d\r\n", dut2.g_dut_pattern_status_buf[31]);

	       if((dut2.g_dut_pattern_status_buf[31]  >= 0) &&(dut2.g_dut_pattern_status_buf[31] <= 0x0F))
        	{
        		if(cc1_rp_voltage > rp12k_ref){
        			dut2.g_dut_pattern_status_buf[30] = 0x00;
        			dut2.g_dut_pattern_status_buf[31]++;
        		}
        		else if(cc1_rp_voltage < rp12k_ref){
        			dut2.g_dut_pattern_status_buf[30] = 0x01;
        			dut2.g_dut_pattern_status_buf[31]++;
        		}
       	}
	       dut2.g_pattern_step++;
		   //////xil_printf("dut2_pattern3_step = %x\r\n", dut2.g_pattern_step);
	       //////////////xil_printf("dut2_pattern2_read_cc1_rp12k_default_voltage\r\n");
	break;
	}

	case 0x0203:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		dut2.g_pattern_smbus_control_buf[2] = 0x0c;
		dut2.g_pattern_smbus_control_buf[3] = 0x01;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		if(dut2.g_dut_pattern_status_buf[30] == 0)
		{
			dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
			dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		}
		else if(dut2.g_dut_pattern_status_buf[30] == 1)
		{
			dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
			dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		   //////xil_printf("dut2_pattern3_step = %x\r\n", dut2.g_pattern_step);
		//////////////xil_printf("dut2_pattern2_write_cc1_rp12k_0x106\r\n");
		break;
	}

	case 0x0204:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x01, 5);

    	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
    	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;


    	cc1_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	rp12k_ref = (u16)(dut2.g_dut_pattern_status_buf[12]<<8 | dut2.g_dut_pattern_status_buf[11]);
    	rp12k_min = (u16)(dut2.g_dut_pattern_status_buf[14]<<8 | dut2.g_dut_pattern_status_buf[13]);
    	rp12k_max = (u16)(dut2.g_dut_pattern_status_buf[16]<<8 | dut2.g_dut_pattern_status_buf[15]);

    	   ////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
		   ////xil_printf("cc1_rp12k_voltage = %d\r\n", cc1_rp_voltage);
	       ////xil_printf("rp12k_ref = %d\r\n",rp12k_ref );
	       ////xil_printf("rp12k_min = %d\r\n",rp12k_min );
	       ////xil_printf("rp12k_max = %d\r\n",rp12k_max );

	   //rp12k calibration
    	if((dut2.g_dut_pattern_status_buf[31] >= 0) && (dut2.g_dut_pattern_status_buf[31]<= 0x0F))
    	{
    		if(cc1_rp_voltage > rp12k_ref)
    		{

    			if(dut2.g_dut_pattern_status_buf[30] == 0x00){

    			dut2.g_dut_pattern_status_buf[31]++;
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;

				dut2.g_pattern_step++;
    			}
    			else if(dut2.g_dut_pattern_status_buf[30] == 0x01)
    			{
    				if((cc1_rp_voltage > rp12k_min) && (cc1_rp_voltage < rp12k_max)){
    					dut2.g_pattern_step++;
    					dut2.g_pattern_step++;
    			}
			   		   else
			   		   {
			   			   dut2.g_result_fail = 0x01;
			   			dut2.g_result_fail_tmrcount = 0xffff;
			   		   }
			    break;
    			}
    			}

    		else if(cc1_rp_voltage < rp12k_ref)
    		{

    			if(dut2.g_dut_pattern_status_buf[30] == 0x01)
    			{

    			dut2.g_dut_pattern_status_buf[31]++;
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
				dut2.g_pattern_step++;
    			}
    			else if(dut2.g_dut_pattern_status_buf[30] == 0x00){
    				if((cc1_rp_voltage > rp12k_min) && (cc1_rp_voltage < rp12k_max)){
    					dut2.g_pattern_step++;
    					dut2.g_pattern_step++;
    			}
			   		   else{
			   			   dut2.g_result_fail = 0x01;
			   			dut2.g_result_fail_tmrcount = 0xffff;
			   		   }

    				//////////////xil_printf("dut2_pattern2_read_cc1_rp12k_calibration_voltage\r\n");
			    break;
    			}

    			}
    		}

    		//calibration has done,output register data

    	else{
    		dut2.g_result_fail = 0x01;
    		dut2.g_result_fail_tmrcount = 0xffff;
    		break;
    	}



    	////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
	break;
	}

	case 0x0205:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0c;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut2.g_dut_pattern_status_buf[30] == 0){

				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		   else if(dut2.g_dut_pattern_status_buf[30] == 1){
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

				       dut2.g_pattern_step--;
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern2_write_cc1_rp12k_calibration_0x106\r\n");
	break;
	}


	//read 0x106 data and save to efuse buf
	case 0x0206:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readphy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0c;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   //dut2.g_pattern_smbus_control_buf[5] = 0x00;
		   //dut2.g_pattern_smbus_control_buf[6] = 0x00;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
		       cc1_rp12k_offset = (((dut2.g_pattern_smbus_control_buf[11] << 8) | (dut2.g_pattern_smbus_control_buf[10]))>>5)&0x1f;
		       if(cc1_rp12k_offset >= 0x0f)
		       {
		    	   dut2.g_dut_pattern_status_buf[30] = 0;
		    	   dut2.g_dut_pattern_status_buf[31] = cc1_rp12k_offset - 0x0f;
		    	   dut2.g_efuse_data_buf[4] = 1;
		    	   dut2.g_efuse_data_buf[5] = dut2.g_dut_pattern_status_buf[30];
		    	   dut2.g_efuse_data_buf[6] = dut2.g_dut_pattern_status_buf[31];
		       }
		       else if(cc1_rp12k_offset < 0x0f)
		       {
		    	   dut2.g_dut_pattern_status_buf[30] = 1;
		    	   dut2.g_dut_pattern_status_buf[31] = 0x0f- cc1_rp12k_offset;
		    	   dut2.g_efuse_data_buf[4] = 1;
		    	   dut2.g_efuse_data_buf[5] = dut2.g_dut_pattern_status_buf[30];
		    	   dut2.g_efuse_data_buf[6] = dut2.g_dut_pattern_status_buf[31];
		       }

				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

				       dut2.g_pattern_step = 0x0300;
					//xil_printf("dut2_cc1_rp12k_offset =%x\r\n", cc1_rp12k_offset);
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
	       //////////////xil_printf("cc1_rp12k_offset =%x\r\n", cc1_rp12k_offset);
		   //////////////xil_printf("dut2.g_pattern_smbus_control_buf[10] =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
		   //////////////xil_printf("dut2.g_pattern_smbus_control_buf[11] =%x\r\n", dut2.g_pattern_smbus_control_buf[11]);
		   //////////////xil_printf("dut2.g_dut_pattern_status_buf[30] =%x\r\n", dut2.g_dut_pattern_status_buf[30]);
		   //////////////xil_printf("dut2.g_dut_pattern_status_buf[31] =%x\r\n", dut2.g_dut_pattern_status_buf[31]);
		   //////////////xil_printf("cc1_rp12k_voltage =%d\r\n", cc1_rp_voltage);
		   //////////xil_printf("dut2.g_efuse_data_buf[5] =%x\r\n", dut2.g_efuse_data_buf[5]);
		   //////////xil_printf("dut2.g_efuse_data_buf[6] =%x\r\n", dut2.g_efuse_data_buf[6]);
		   //////////////xil_printf("dut2_pattern2_read_cc1_rp12k_calibration_0x106\r\n");
	break;
	}

	//CC2 RP12K test
	case 0x0300:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0c;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut2.g_dut_pattern_status_buf[30] == 0){

				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;

		   }
		   else if(dut2.g_dut_pattern_status_buf[30] == 1){
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		  smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern2_write_cc2_rp12k_calibration_0x106\r\n");
	break;
	}

	case 0x0301:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x03, 5);

    	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
    	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;

    	cc2_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	rp12k_ref = (u16)(dut2.g_dut_pattern_status_buf[12]<<8 | dut2.g_dut_pattern_status_buf[11]);
    	rp12k_min = (u16)(dut2.g_dut_pattern_status_buf[14]<<8 | dut2.g_dut_pattern_status_buf[13]);
    	rp12k_max = (u16)(dut2.g_dut_pattern_status_buf[16]<<8 | dut2.g_dut_pattern_status_buf[15]);

    	   //////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
		   //////////////xil_printf("cc2_rp12k_voltage = %d\r\n", cc2_rp_voltage);
	       //////////////xil_printf("rp12k_ref = %d\r\n",rp12k_ref );
	       //////////////xil_printf("rp12k_min = %d\r\n",rp12k_min );
	       //////////////xil_printf("rp12k_max = %d\r\n",rp12k_max );

			if((cc2_rp_voltage > rp12k_min) && (cc2_rp_voltage < rp12k_max)){
	   		   dut2.g_pattern_step = 0x0400;
		}

	   		   else{
	   			   dut2.g_result_fail = 0x01;
	   			dut2.g_result_fail_tmrcount = 0xffff;
	   		   }
			////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
			//////////////xil_printf("dut2_pattern2_read_cc2_rp12k_voltage\r\n");
	break;
	}


	//CC2 RP36K test
	case 0x0400:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut2.g_pattern_smbus_control_buf[2] = 0x22;
		   dut2.g_pattern_smbus_control_buf[3] = 0x10;
		   dut2.g_pattern_smbus_control_buf[4] = 0x02;
		   dut2.g_pattern_smbus_control_buf[5] = 0x26;
		   dut2.g_pattern_smbus_control_buf[6] = 0x26;


		  smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern2_write_cc2_rp12k_calibration_0x1022\r\n");
	break;
	}

	case 0x0401:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0d;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut2.g_dut_pattern_status_buf[30] == 0){

				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;

		   }
		   else if(dut2.g_dut_pattern_status_buf[30] == 1){
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		  smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern2_write_cc1_rp36k_calibration_0x107\r\n");
	break;
	}

	case 0x0402:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x01, 5);
      	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
      	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;

      	cc1_rp_voltage = (u16)dut2.g_as7994Cvsdata;
      	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;

      	rp36k_min = (u16)(dut2.g_dut_pattern_status_buf[22]<<8 | dut2.g_dut_pattern_status_buf[21]);
      	rp36k_max = (u16)(dut2.g_dut_pattern_status_buf[24]<<8 | dut2.g_dut_pattern_status_buf[23]);

      	//////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
      	//////////////xil_printf("cc1_rp36k_voltage = %d\r\n", cc1_rp_voltage);

      	//////////////xil_printf("rp36k_min = %d\r\n",rp36k_min );
      	//////////////xil_printf("rp36k_max = %d\r\n",rp36k_max );

      	if((cc1_rp_voltage > rp36k_min) && (cc1_rp_voltage < rp36k_max)){
      		dut2.g_pattern_step++;
			}
      	else{
      		dut2.g_result_fail = 0x01;
      		dut2.g_result_fail_tmrcount = 0xffff;
      	}
      	////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
      	//////////////xil_printf("dut2_pattern2_read_cc1_rp36k_calibration_voltage\r\n");
	break;
	}

	case 0x0403:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x03, 5);
      	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
      	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;

      	cc2_rp_voltage = (u16)dut2.g_as7994Cvsdata;
      	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;

      	rp36k_min = (u16)(dut2.g_dut_pattern_status_buf[22]<<8 | dut2.g_dut_pattern_status_buf[21]);
      	rp36k_max = (u16)(dut2.g_dut_pattern_status_buf[24]<<8 | dut2.g_dut_pattern_status_buf[23]);

      	//////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
      	//////////////xil_printf("cc2_rp36k_voltage = %d\r\n", cc2_rp_voltage);

      	//////////////xil_printf("rp36k_min = %d\r\n",rp36k_min );
      	//////////////xil_printf("rp36k_max = %d\r\n",rp36k_max );

      	if((cc2_rp_voltage > rp36k_min) && (cc2_rp_voltage < rp36k_max)){
      		dut2.g_pattern_step = 0x0500;
			}
      	else{
      		dut2.g_result_fail = 0x01;
      		dut2.g_result_fail_tmrcount = 0xffff;
      	}
      	////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
      	//////////////xil_printf("dut2_pattern2_read_cc2_rp36k_calibration_voltage\r\n");
	break;
	}

	 //rp4.7k test
	case 0x0500:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut2.g_pattern_smbus_control_buf[2] = 0x22;
		   dut2.g_pattern_smbus_control_buf[3] = 0x10;
		   dut2.g_pattern_smbus_control_buf[4] = 0x02;
		   dut2.g_pattern_smbus_control_buf[5] = 0x16;
		   dut2.g_pattern_smbus_control_buf[6] = 0x16;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern2_write_cc2_rp4.7k_calibration_0x1022\r\n");
		   break;
	}

	case 0x0501:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0e;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut2.g_dut_pattern_status_buf[30] == 0){

				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		   else if(dut2.g_dut_pattern_status_buf[30] == 1){
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   //xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern2_write_cc1_rp36k_calibration_0x108\r\n");
	break;
	}

	case 0x0502:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x01, 5);
      	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
      	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;


      	cc1_rp_voltage = (u16)dut2.g_as7994Cvsdata;
      	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;

      	rp4p7k_min = (u16)(dut2.g_dut_pattern_status_buf[18]<<8 | dut2.g_dut_pattern_status_buf[17]);
      	rp4p7k_max = (u16)(dut2.g_dut_pattern_status_buf[20]<<8 | dut2.g_dut_pattern_status_buf[19]);

      	//////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
	    //////////////xil_printf("cc1_rp4p7k_voltage = %d\r\n", cc1_rp_voltage);

		//////////////xil_printf("rp4p7k_min = %d\r\n",rp4p7k_min );
		//////////////xil_printf("rp4p7k_max = %d\r\n",rp4p7k_max );

		if((cc1_rp_voltage > rp4p7k_min) && (cc1_rp_voltage < rp4p7k_max))
		{
			dut2.g_pattern_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			dut2.g_result_fail_tmrcount = 0xffff;
		}
		//xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		//////////////xil_printf("dut2_pattern2_read_cc1_rp4.7k_calibration_voltage\r\n");
		break;
	}
	case 0x0503:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x03, 5);
      	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
      	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;


      	cc2_rp_voltage = (u16)dut2.g_as7994Cvsdata;
      	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;

      	rp4p7k_min = (u16)(dut2.g_dut_pattern_status_buf[18]<<8 | dut2.g_dut_pattern_status_buf[17]);
      	rp4p7k_max = (u16)(dut2.g_dut_pattern_status_buf[20]<<8 | dut2.g_dut_pattern_status_buf[19]);

      	//////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
	    //////xil_printf("cc2_rp4p7k_voltage = %d\r\n", cc2_rp_voltage);

		//////xil_printf("rp4p7k_min = %d\r\n",rp4p7k_min );
		//////xil_printf("rp4p7k_max = %d\r\n",rp4p7k_max );

		if((cc2_rp_voltage > rp4p7k_min) && (cc2_rp_voltage < rp4p7k_max))
		{
	   	    dut2.g_pattern_timer = 0xfff;
			dut2.g_pattern_step = 0;
			dut2.g_dut_pattern_status_buf[2]++;
		    dut2.g_uartPatternEnable = 0x00;
		    dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
		    dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
		    result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
		    //xil_printf("dut2.g_uartPatternNum = %x\r\n\r\n",dut2.g_uartPatternNum);
			xil_printf("dut2_pattern3_rp_test_pass!    rp12k_signbit= 0x%x  rp12k_offset=0x%x\r\n\r\n",dut2.g_efuse_data_buf[5],dut2.g_efuse_data_buf[6]);
			//xil_printf("dut2.g_result_fail = %x\r\n",dut2.g_result_fail);
		}
		else
		{
			dut2.g_result_fail = 0x01;
			dut2.g_result_fail_tmrcount = 0xffff;
			xil_printf("dut2.g_result_fail = %x\r\n",dut2.g_result_fail);
		}
		//xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		//////////xil_printf("dut2_pattern2_read_cc2_rp4.7k_calibration_voltage\r\n\r\n");
		//////xil_printf("dut2.g_dut_pattern_status_buf[2] = %x\r\n\r\n",dut2.g_dut_pattern_status_buf[2]);
		//////xil_printf("dut2.g_uartPatternNum = %x\r\n",dut2.g_uartPatternNum);

		break;
	}

	}

	if(dut2.g_result_fail == 0x01)
	{
		dut2.g_ft2_test_done = 0x00;
		pattern_safe_state_dut2();	//add safe state
		xil_printf("dut2_pattern3_fail! fail_step = %x\r\n",dut2.g_pattern_step);
	    xil_printf("rp12k_min = %x rp12k_max = %x\r\n",rp12k_min, rp12k_max);
	    xil_printf("rp36k_min = %x rp36k_max = %x\r\n",rp36k_min, rp36k_max);
	    xil_printf("rp4p7k_min = %x rp4p7k_max = %x\r\n",rp4p7k_min, rp4p7k_max);
	    xil_printf("dut2_cc_rp_voltage = %x\r\n\r\n",cc_rp_voltage);
	}

	//xil_printf("dut2_pattern3_rp_step = 0x%x\r\n", dut2.g_pattern_step);
	return 0;
}


