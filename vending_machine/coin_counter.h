#include "systemc.h"

SC_MODULE(coin_counter) {
	
	sc_in<bool> load, 
		    dispense, 
		    nickel_out, 
		    dime_out, 
		    two_dime_out;

	sc_out<bool>    empty;

	sc_in<sc_uint<8> > nickels,
			   dimes;
	sc_in<sc_logic> clk;
	
	sc_int<8> nickel_count, 
		  dime_count;
	//ncsc_viewable< sc_int<8> > count;

	void coins();
	void loader();

	SC_CTOR(coin_counter): 
		dimes		("dimes"), 
		dime_out	("dime_out"), 
		load		("load"), 
		dispense	("dispense"),
		nickels		("nickels"), 
		nickel_out	("nickel_out"), 
		two_dime_out	("two_dime_out"), 
	//	count		("nickel_count", nickel_count),
		empty		("empty"),
		clk		("clk")
	{
		SC_METHOD(loader);
		sensitive << load;

		SC_METHOD(coins);
		sensitive_pos << dispense;
	}
};
