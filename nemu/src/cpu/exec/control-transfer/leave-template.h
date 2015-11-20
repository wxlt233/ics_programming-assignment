
#define instr leave

static void leave() {
	cpu.esp=cpu.ebp;
	cpu.ebp=swaddr_read(cpu.esp,4);
	print_asm_template1();
}





