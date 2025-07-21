#include "can_counter.h"

void can_counter::loader() {
	left = count.read();
	empty.write(false); 
}

void can_counter::dispenser(){
	left = left - 1;
	if (left == 0) {
		cout << sc_simulation_time() << "\t **out of drinks" << endl;
		empty.write(true);
	}	
}
