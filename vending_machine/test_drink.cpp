#include "test_drink.h"

;void test_drink::clk_thread ( ) {
  		while (true) {
    		  clk.write(SC_LOGIC_0);
    		  wait(100, SC_NS);
    		  clk.write(SC_LOGIC_1);
    		  wait(100, SC_NS);
		}
}

void test_drink::enter_nickel(){
		nickel_in.write(true);
		cout << sc_simulation_time() << "\t entered nickel" << endl;
		wait();
		nickel_in.write(false);
}

void test_drink::enter_dime(){
		dime_in.write(true);
		cout << sc_simulation_time() << "\t entered dime" << endl;
		wait();
		dime_in.write(false);
}

void test_drink::enter_quarter(){
		quarter_in.write(true);
		cout << sc_simulation_time() << "\t entered quarter" << endl;
		wait();
		quarter_in.write(false);
}

void test_drink::messaging(){
	if (dispense.read() == true){
		cout << sc_simulation_time() << " -> drink dispensed" << endl;
		cout << "-----------------------" << endl;
	}
	if (nickel_out.read() == true){
                cout << sc_simulation_time() << "\t nickel changed" << endl;
                cout << "-----------------------" << endl;
        }
	if (dime_out.read() == true){
		cout << sc_simulation_time() << "\t dime changed" << endl;
		cout << "-----------------------" << endl;
	}
	if (two_dime_out.read() == true){
                cout << sc_simulation_time() << "\t two dimes changed" << endl;
                cout << "-----------------------" << endl;
        }
}

void test_drink::stop_machine() {
	if (dispense)  {
		cout << sc_simulation_time() << "\t stopping machine" << endl;
		cout << "-----------------------" << endl;
		sc_stop();
	}
}

void test_drink::initialize_machine() {
   	// write initial values
	nickel_in.write(false);
	dime_in.write(false);
	quarter_in.write(false);
	//load.write(false);
	reset.write(true);
	wait(100, SC_NS);
	reset.write(false);
	cans.write(10);
	nickels.write(4);
	dimes.write(8);
	wait(100, SC_NS);
	load.write(true);
	cout 	<< sc_time_stamp() 
		<< "\t loading machine with " 
		<< cans.read() 
		<< " cans, " 
		<< dimes.read() 
		<< " dimes, " 
		<< nickels.read() 
		<< " nickels" 
		<< endl;
	wait(100, SC_NS);
	load.write(false);
}
	
	
void test_drink::buy_drinks(){
		wait(buyDrinks);
		wait();
                enter_nickel();
                enter_dime();
                enter_quarter();
                enter_dime();
                enter_quarter();
		enter_nickel();
		enter_nickel();
                enter_dime();
                enter_quarter();
                enter_nickel();
                enter_dime();
                enter_quarter();
		enter_dime();
                enter_quarter();
                enter_nickel();
                enter_nickel();
		enter_dime();
		enter_dime();
                enter_quarter();
                enter_quarter();
                enter_nickel();
                enter_dime();
                enter_dime();
                enter_dime();
                enter_quarter();
		enter_dime();
		enter_dime();
                enter_dime();
                enter_quarter();
                enter_nickel();
                enter_nickel();
                enter_dime();
                enter_dime();
		enter_nickel();
		enter_nickel();
		enter_quarter();
		enter_dime();
		enter_quarter();

		enter_nickel();
		enter_quarter();
		enter_quarter();

		enter_quarter();
		enter_dime();
		enter_dime();
		enter_dime();
}

void test_drink::run_machine(){
	initialize_machine();
	notify(buyDrinks);
}			

NCSC_MODULE_EXPORT(test_drink)
