#include "systemc.h"

SC_MODULE(can_counter) {

	sc_in<bool> load;
	sc_in<bool> dispense; 
	sc_in<sc_uint<8> > count;
	sc_out<bool> empty;
	sc_in<sc_logic> clk;

	sc_uint<8> left;
	void loader();
	void dispenser();

	SC_CTOR(can_counter): 
		load	("load"), 
		dispense("dispense"), 
		count	("count"), 
		empty	("empty"), 
		clk	("clk") 
	{
		SC_METHOD(loader);
		sensitive << load;

		SC_METHOD(dispenser);
		sensitive_pos << dispense;
	}
};
		
	
