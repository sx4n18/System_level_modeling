#include "coin_counter.h"

void coin_counter::loader() {
	nickel_count = nickels.read();
	dime_count = dimes.read();
}

void coin_counter::coins() {
	if (nickel_out) {
		if (nickel_count != 0)
			nickel_count = nickel_count - 1;
		else {
			cout << sc_simulation_time() << "\t ** out of nickels" << endl;
			empty.write(true);
		}
	}
	else if (dime_out) {
		if (dime_count != 0)
			dime_count = dime_count - 1;
		else if (nickel_count != 0) 
			nickel_count = nickel_count - 2;
		else {
			cout << sc_simulation_time() << "\t ** out of coins" << endl;
			empty.write(true);
		}
	}
	else if (two_dime_out) {
		if (dime_count != 0)
			dime_count = dime_count - 2;
		else if (nickel_count != 0)
			nickel_count = nickel_count - 4;
		else {
			cout << sc_simulation_time() << "\t ** out of coins" << endl;
			empty.write(true);
		}
	}
}
