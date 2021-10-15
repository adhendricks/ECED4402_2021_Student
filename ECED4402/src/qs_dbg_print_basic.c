/**
 * \file
 *
 * \brief Debug print basic quick start guide code
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>

uint32_t main_counter;
char main_string[] = "Main task iteration: 0x00000000\r\n";

static void PrintTaskID(void*params);
static void main_task(void *params);

static void main_task(void *params)
{
	char ibuff[50];
	int taskcount = 0;
	TaskHandle_t xHandle[50];
	
	size_t val;
	do {
		dbg_print_str("Main task loop executing ");

		// Update hexadecimal 32-bit integer in string, and print it
		dbg_sprint_hexint(&main_string[23], main_counter++);
		sprintf(main_string,"%ld \r\n",main_counter);
		dbg_print_str(main_string);

		if(port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE){
			if (xTaskCreate(&PrintTaskID,(const char*)"subtask",configMINIMAL_STACK_SIZE+100,(void *) taskcount,tskIDLE_PRIORITY+2,&xHandle[taskcount])!=pdPASS){
				sprintf(ibuff,"Failed creating new task %d \r\n",taskcount);
				dbg_print_str(ibuff);
				}else{
				sprintf(ibuff,"Created a new task %d \r\n",taskcount++);
				dbg_print_str(ibuff);
			}
			}else if(port_pin_get_input_level(PIN_PA16) == BUTTON_0_ACTIVE && taskcount >0){
			taskcount--;
			vTaskDelete(xHandle[taskcount]);
			sprintf(ibuff,"Deleted task new task %d \r\n",taskcount);
			dbg_print_str(ibuff);
		}
		val = xPortGetFreeHeapSize() ;
		sprintf(ibuff,"Size = %d Bytes\r\n",val);
		dbg_print_str(ibuff);

		vTaskDelay(1000 / portTICK_RATE_MS);
	} while(1);
}

static void PrintTaskID(void*params){
	char ibuff[50];
	do{
		sprintf(ibuff,"Task %d printing \r\n",(int) params);
		dbg_print_str(ibuff);
		
		vTaskDelay(1000);
	}while(1);
}

int main (void)
{
	//! [init_calls]
	system_init();
	dbg_init();
	
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	//Configure button 2
	/* Set buttons as inputs */
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(PIN_PA16, &pin_conf);
	
	xTaskCreate(&main_task,
	(const char *)"Main task",
	configMINIMAL_STACK_SIZE + 100,
	NULL,
	tskIDLE_PRIORITY + 2,
	NULL);

	vTaskStartScheduler();
}
