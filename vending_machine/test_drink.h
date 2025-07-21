#include "systemc.h"
#include "drink_machine_top.h"

SC_MODULE(test_drink){
	
	// signals
	sc_signal<bool> load, 
			nickel_in, 
			dime_in, 
			quarter_in, 
			reset, 
			dispense, 
			nickel_out, 
			dime_out, 
			two_dime_out, 
			empty;
	sc_signal<sc_logic> clk;
	sc_signal<sc_uint<8> > nickels, 
				cans, 
				dimes;

	// instantiate top
	drink_machine_top top;
     
	// events 
        sc_event buyDrinks;

	// functions/processes
	void clk_thread();
        void initialize_machine();
	void run_machine();
        void buy_drinks();
        void enter_nickel();
        void enter_dime();
        void enter_quarter();
        void messaging();
	void stop_machine();

	SC_CTOR(test_drink) : 
		load		("load"), 
		nickel_in	("nickel_in"), 
		dime_in		("dime_in"), 
		quarter_in	("quarter_in"), 
		clk		("clk"), 
		reset		("reset"), 
		dispense	("dispense"), 
		nickel_out	("nickel_out"), 
		dime_out	("dime_out"), 
		two_dime_out	("two_dime_out"), 
		empty		("empty"), 
		nickels		("nickels"), 
		cans		("cans"), 
		dimes		("dimes"), 
		top		("top") 
	{
		// top connections
		top.load(load);
		top.nickel_in(nickel_in);
		top.dime_in(dime_in);
		top.quarter_in(quarter_in);
		top.clk(clk);
		top.reset(reset);
		top.dispense(dispense);
		top.nickel_out(nickel_out);
		top.dime_out(dime_out);
		top.two_dime_out(two_dime_out);
		top.empty(empty);
		top.nickels(nickels);
		top.cans(cans);
		top.dimes(dimes);

		SC_THREAD(clk_thread);

		SC_THREAD(run_machine);

                SC_THREAD(buy_drinks);
		sensitive_neg << clk;	

                SC_METHOD(messaging);
                sensitive << dispense;

		SC_METHOD(stop_machine);
		sensitive_pos << empty;
	}
}
