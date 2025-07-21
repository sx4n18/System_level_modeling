#include "systemc.h"
#include "can_counter.h"
#include "coin_counter.h"
#include "drink_machine.h"

SC_MODULE(drink_machine_top){
	//ports
	sc_in<bool> load, 
		nickel_in,  
		dime_in, 
		quarter_in, 
		reset;
	sc_in<sc_uint<8> > nickels, 
		dimes, 
		cans;
	sc_out<bool> dispense, 
		nickel_out, 
		dime_out, 
		two_dime_out, 
		empty; 
	sc_in<sc_logic> clk;

	// instances
	coin_counter coins;
	can_counter drinks;
	drink_machine vending;
   
	SC_CTOR(drink_machine_top): 
		load		("load"), 
		nickel_in	("nickel_in"), 
		dime_in		("dime_in"), 
		quarter_in	("quarter_in"), 
		clk		("clk"), 
		reset		("reset"), 
		nickels		("nickels"), 
		dimes		("dimes"), 
		cans		("cans"), 
		dispense	("dispense"), 
		nickel_out	("nickel_out"), 
		dime_out	("dime_out"), 
		two_dime_out	("two_dime_out"), 
		empty		("empty"), 
		coins		("coins"), 
		drinks		("drinks"), 
		vending		("vending")
	{
	coins.dimes(dimes);
	coins.dime_out(dime_out);
	coins.load(load);
	coins.dispense(dispense);
	coins.nickels(nickels);
	coins.nickel_out(nickel_out);
	coins.two_dime_out(two_dime_out);
	coins.empty(empty);
	coins.clk(clk);

	drinks.load(load);
	drinks.count(cans);
	drinks.dispense(dispense);
	drinks.empty(empty);
	drinks.clk(clk);

	vending.nickel_in(nickel_in);
	vending.dime_in(dime_in);
	vending.quarter_in(quarter_in);
	vending.reset(reset);
	vending.nickel_out(nickel_out);
	vending.dime_out(dime_out);
	vending.two_dime_out(two_dime_out);
	vending.dispense(dispense);
	vending.clk(clk);
	}
};
