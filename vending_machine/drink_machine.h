#include "systemc.h"

enum dm_state 
	{
		IDLE, 
		FIVE, 
		TEN, 
		FIFTEEN, 
		TWENTY, 
		TWENTY_FIVE, 
		THIRTY, 
		THIRTY_FIVE, 
		FORTY, 
		FORTY_FIVE
	}; 

SC_MODULE(drink_machine) {
	sc_in<bool> nickel_in, 
		dime_in, 
		quarter_in, 
		reset; 
	sc_out<bool> nickel_out, 
		dime_out, 
		two_dime_out, 
		dispense;
	sc_in<sc_logic> clk;
	dm_state current_state;
	
	void dm_fsm();
	void reset_machine();

	SC_CTOR(drink_machine): 
		nickel_in	("nickel_in"), 
		dime_in		("dime_in"), 
		quarter_in	("quarter_in"), 
		clk		("clk"), 
		reset		("reset"), 
		nickel_out	("nickel_out"), 
		dime_out	("dime_out"), 
		two_dime_out	("two_dime_out"), 
		dispense	("dispense") 
	{
		
		SC_METHOD(reset_machine);
		sensitive << reset;

		SC_METHOD(dm_fsm);
		sensitive_pos << clk;

	}
};
		

