#include <stdio.h>
#include "platform.h"
#include <stdio.h>
#include <unistd.h>
#include "plic/plic_driver.h"

#include <stdlib.h>
#include <string.h>
#include "encoding.h"
#include "stdatomic.h"

#define CAP_TOUCH_IP	PIN_4_OFFSET
#define CAP_TOUCH_OP	PIN_7_OFFSET

#define BUF_SZ	124

static uint32_t start_cycles = 0;
static volatile uint32_t end_cycles = 0;
static volatile uint64_t * mtime       = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
plic_instance_t g_plic;

void set_output(int offset)
{
	GPIO_REG(GPIO_INPUT_EN) &= ~(1 << offset);
	GPIO_REG(GPIO_OUTPUT_EN) |= (1 << offset);
	GPIO_REG(GPIO_OUTPUT_VAL) &= ~(1 << offset);
}

void set_input(int offset, int pullup)
{
	GPIO_REG(GPIO_INPUT_EN) |= (1 << offset);
	GPIO_REG(GPIO_OUTPUT_EN) &= ~(1 << offset);
	if(pullup > 0)
		GPIO_REG(GPIO_PULLUP_EN) |= (1 << offset);
	else
		GPIO_REG(GPIO_PULLUP_EN) &= ~(1 << offset);
}

void set_output_val(int offset, int val)
{
	if(val > 0)
		GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << offset);
	else
		GPIO_REG(GPIO_OUTPUT_VAL) &= ~(1 << offset);
}

void handle_m_time_interrupt()
{
	const char* msg = "time interrupt\n";
	write(STDOUT_FILENO, msg, strlen(msg));
}

void handle_m_ext_interrupt()
{
	clear_csr(mie, MIP_MEIP);
	plic_source int_num  = PLIC_claim_interrupt(&g_plic);
	if ((int_num >=1 ) && (int_num < PLIC_NUM_INTERRUPTS)) {
		end_cycles  = read_csr(mcycle);
		GPIO_REG(GPIO_RISE_IP) = (1 << CAP_TOUCH_IP);
	}
	else {
		exit(1 + (uintptr_t) int_num);
	}
	PLIC_complete_interrupt(&g_plic, int_num);
	set_csr(mie, MIP_MEIP);
}

void setup_interrupt()
{
	clear_csr(mie, MIP_MEIP);
	clear_csr(mie, MIP_MTIP);
	uint32_t intno = INT_GPIO_BASE + PIN_4_OFFSET;

	PLIC_enable_interrupt(&g_plic, intno);
	PLIC_set_priority(&g_plic, intno, 1);
	GPIO_REG(GPIO_RISE_IE) |= (1 << CAP_TOUCH_IP);

	set_csr(mie, MIP_MEIP);
	set_csr(mstatus, MSTATUS_MIE);
}

int main(int argc, char* argv[])
{
	char buf[BUF_SZ];
	set_output(RED_LED_OFFSET);
	set_output_val(RED_LED_OFFSET, 1);

	set_output(GREEN_LED_OFFSET);
	set_output_val(GREEN_LED_OFFSET, 1);

	set_output(BLUE_LED_OFFSET);
	set_output_val(BLUE_LED_OFFSET, 1);

	PLIC_init(&g_plic, PLIC_CTRL_ADDR, PLIC_NUM_INTERRUPTS, PLIC_NUM_PRIORITIES);
	PLIC_set_threshold(&g_plic, 0);
	setup_interrupt();

	int threshold = 220;
	volatile unsigned int time_taken;
	while(1)
	{
		start_cycles = read_csr(mcycle);
		set_input(CAP_TOUCH_IP, 1);
		while(end_cycles == 0)
			;//wait until interrupt triggered
		time_taken = end_cycles-start_cycles;
		set_output(CAP_TOUCH_IP);
		set_output_val(CAP_TOUCH_IP, 0);
		//printf("end_cycles: %u, start_cycles: %u, time_taken: %u\n", end_cycles, start_cycles, time_taken);
		snprintf(buf, BUF_SZ, "%u\n", time_taken);
		if(time_taken > threshold)
			set_output_val(RED_LED_OFFSET, 0);
		else
			set_output_val(RED_LED_OFFSET, 1);
		write(STDOUT_FILENO, buf, strlen(buf));
		end_cycles = 0;
	}
	return 0;
}
