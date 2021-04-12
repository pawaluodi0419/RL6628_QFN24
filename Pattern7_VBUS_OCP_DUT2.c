#include "platform.h"
#include "Pattern7_VBUS_OCP.h"
#include "vbus_ocp_calibration.h"

u8 _by_Pattern7_VBUS_OCP_dut2()
{
	u8 i;
	u16 adc_code;
	u16 vbusout_voltage_code;
	u16 vbusout_low_spec;
	u16 vbusout_high_spec;

	u8 Buff_dut2_XGPIO_0[8];

	switch(dut2.g_pattern_step)
	{
	case 0x0000:
	{
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,5, 0);	//S2-VMON_TRIM_EN
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,23, 0);	//VBUS_OCP EN control
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,26, 0);	//1.5A_EN control
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,25, 0);	//3A_EN control
		msdelay(5);

		_byAD7994ReadCvsData_dut2(AD7994_ADC2_SLAVE_ADDR,0x07, 5);
		vbusout_voltage_code = dut2.g_as7994Cvsdata *(90.9+47)/47;
		vbusout_low_spec = 0.5/2.5*4095;
		//xil_printf("dut2.g_as7994Cvsdata = %x\r\n", dut2.g_as7994Cvsdata);
		xil_printf("dut2_vbusout_voltage_code = %x\r\n", vbusout_voltage_code);
		//xil_printf("vbusout_low_spec = %x\r\n", vbusout_low_spec);
		if(vbusout_voltage_code<vbusout_low_spec)
		{
	    	dut2.g_pattern_step++;
	    	//dut2.g_pattern_step++;
		}
		else
		{
			msdelay(300);
			_byAD7994ReadCvsData_dut2(AD7994_ADC2_SLAVE_ADDR,0x07, 5);
			vbusout_voltage_code = dut2.g_as7994Cvsdata *(90.9+47)/47;
			vbusout_low_spec = 0.5/2.5*4095;
			xil_printf("dut2_reread_vbusout_voltage_code = %x\r\n", vbusout_voltage_code);
			if(vbusout_voltage_code<vbusout_low_spec)
			{
		    	dut2.g_pattern_step++;
		    	//dut2.g_pattern_step++;
			}
			else
			{
			dut2.g_result_fail = 0x01;
			xil_printf("dut2_vbus_out_voltage_read,vbus_output_over_low_spec!\r\n");
			}
		}

		break;
	}

	//0.9A vbus_ocp trimmng
	case 0x0001:
	{
		dut2.g_vbusocp_board_en = 1;
		dut2.g_vbusocp_1p5a_nmos_control = 0;
		dut2.g_vbusocp_3a_nmos_control = 0;
		_vbus_ocp_calibration_dut2();

		if(dut2.g_vbusocp_process_bit != vbusocp_process_done_pass)
		{

		}
		else
		{
			dut2.g_vbusocp_writeefuse_0p9a = dut2.g_vbus_ocp_calibrationdata_buf[1];
			dut2.g_vbusocp_process_bit = vbusocp_process_waiting;
			dut2.g_vbusocp_step=0;
			if((dut2.g_vbusocp_writeefuse_0p9a>VBUSOCP_Efuse_0p9a_Min)&&(dut2.g_vbusocp_writeefuse_0p9a<VBUSOCP_Efuse_0p9a_Max))
			{
				dut2.g_pattern_step++;
				xil_printf(">>>>>>>>>> dut2_pattern7_vbus_ocp_0p9a_test_pass!\r\n\r\n");
			}
			else
			{
				dut2.g_result_fail = 0x01;
				xil_printf("dut2.g_vbusocp_writeefuse_0p9a = %x VBUSOCP_Efuse_0p9a_Min=%x VBUSOCP_Efuse_0p9a_Max=%x\r\n",dut2.g_vbusocp_writeefuse_0p9a,VBUSOCP_Efuse_0p9a_Min,VBUSOCP_Efuse_0p9a_Max);
				xil_printf("dut2_g_vbusocp_writeefuse_0p9a fail,not fit spec!\r\n");
			}
		}
		break;
	}

	//1.5A vbus_ocp trimmng
	case 0x0002:
	{
		dut2.g_vbusocp_board_en = 1;
		dut2.g_vbusocp_1p5a_nmos_control = 1;
		dut2.g_vbusocp_3a_nmos_control = 0;
		_vbus_ocp_calibration_dut2();

		if(dut2.g_vbusocp_process_bit != vbusocp_process_done_pass)
		{
			if((dut2.g_vbusocp_step==0x09)&&(dut2.g_vbusocp_readadc_ready == 1))
			{
				_byAD7994ReadCvsData_dut2(AD7994_ADC2_SLAVE_ADDR,0x07, 5);

				vbusout_voltage_code = dut2.g_as7994Cvsdata *(90.9+47)/47;
				vbusout_high_spec = 4.0/2.5*4095;
				//xil_printf("dut2_vbusout_voltage_code = %x\r\n", vbusout_voltage_code);
				//xil_printf("dut2_vbusout_high_spec = %x\r\n", vbusout_high_spec);
				if(vbusout_voltage_code>vbusout_high_spec)
				{
			    	//dut2.g_pattern_step++;
				}
				else
				{
					dut2.g_result_fail = 0x01;
					xil_printf("dut2_vbus_out_voltage_read,vbus_output_over_high_spec!\r\n");
				}
			}
		}
		else
		{
			dut2.g_vbusocp_writeefuse_1p5a = dut2.g_vbus_ocp_calibrationdata_buf[1];
			dut2.g_vbusocp_process_bit = vbusocp_process_waiting;
			dut2.g_vbusocp_step=0;
			if((dut2.g_vbusocp_writeefuse_1p5a>VBUSOCP_Efuse_1p5a_Min)&&(dut2.g_vbusocp_writeefuse_1p5a<VBUSOCP_Efuse_1p5a_Max))
			{
				dut2.g_pattern_step++;
				dut2.g_pattern_step++;
				xil_printf(">>>>>>>>>> dut2_pattern7_vbus_ocp_1p5a_test_pass!\r\n\r\n");
			}
			else
			{
				dut2.g_result_fail = 0x01;
				xil_printf("dut2.g_vbusocp_writeefuse_1p5a = %x VBUSOCP_Efuse_1p5a_Min=%x VBUSOCP_Efuse_1p5a_Max=%x\r\n",dut2.g_vbusocp_writeefuse_1p5a,VBUSOCP_Efuse_1p5a_Min,VBUSOCP_Efuse_1p5a_Max);
				xil_printf("dut2_g_vbusocp_writeefuse_1p5a fail,not fit spec!\r\n");
			}
		}
		break;
	}

	//3A vbus_ocp trimmng
	case 0x0003:
	{
		dut2.g_vbusocp_board_en = 1;
		dut2.g_vbusocp_1p5a_nmos_control = 1;
		dut2.g_vbusocp_3a_nmos_control = 1;
		_vbus_ocp_calibration_dut2();

		if(dut2.g_vbusocp_process_bit != vbusocp_process_done_pass)
		{

		}
		else
		{
			dut2.g_vbusocp_writeefuse_3a = dut2.g_vbus_ocp_calibrationdata_buf[1];
			dut2.g_vbusocp_process_bit = vbusocp_process_waiting;
			dut2.g_vbusocp_step=0;
			if((dut2.g_vbusocp_writeefuse_3a>VBUSOCP_Efuse_3a_Min)&&(dut2.g_vbusocp_writeefuse_3a<VBUSOCP_Efuse_3a_Max))
			{
				//dut2.g_vbusocp_3a_done = 0x01;
				dut2.g_pattern_step++;
				xil_printf(">>>>>>>>>> dut2_pattern7_vbus_ocp_3a_test_pass!\r\n\r\n");
			}
			else
			{
				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
				xil_printf("dut2.g_vbusocp_writeefuse_3a = %x VBUSOCP_Efuse_3a_Min = %x VBUSOCP_Efuse_3a_Max = %x\r\n",dut2.g_vbusocp_writeefuse_3a,VBUSOCP_Efuse_3a_Min,VBUSOCP_Efuse_3a_Max);
				xil_printf("dut2_g_vbusocp_writeefuse_3a fail,not fit spec!\r\n");
			}
		}
		break;
	}

	case 0x0004:
	{
		for(i=1; i<60; i++)
		{
			dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
		}
		dut2.g_pattern_timer = 0xfff;
		dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		dut2.g_pattern_step = 0x00;
		dut2.g_dut_pattern_status_buf[2]++;
		dut2.g_uartPatternEnable = 0x00;

		//this is control vbus board,base addr is dutx
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,23, 0);//VBUS_OCP_EN Control
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,26, 0);//1.5A_EN Control
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,25, 0);//3A_EN Control
		if((dut2.g_vbusocp_3a_nmos_control ==1) &&(dut2.g_vbusocp_3a_en_status==1))
		{
			g_vbusocp_3a_status=0;
		}

		xil_printf("dut2_pattern7_vbusocp_calibration_pass!\r\n");
		xil_printf("dut2_efuse_0p9a=0x%x,efuse_1p5a=0x%x\r\n\r\n",dut2.g_vbusocp_writeefuse_0p9a,dut2.g_vbusocp_writeefuse_1p5a);
		dut2.g_uartPatternNum = 0x0c;
		result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
		//xil_printf("dut2.g_uartPatternNum = %x\r\n\r\n",dut2.g_uartPatternNum);
		break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		dut2.g_ft2_test_done = 0x00;
		pattern_safe_state_dut2();	//add safe state

		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,23, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,26, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,25, 0);
		usdelay(5);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,23, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,26, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,25, 0);

		msdelay(2);
		if((dut2.g_vbusocp_3a_nmos_control ==1) &&(dut2.g_vbusocp_3a_en_status==1))
		{
			g_vbusocp_3a_status=0;
		}
		xil_printf("dut2_pattern7_fail! fail_pattern_step = %x    g_vbusocp_step=%x\r\n\r\n",dut2.g_pattern_step,dut2.g_vbusocp_step);
	}

	return 0;
}


