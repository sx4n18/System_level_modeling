#include "drink_machine.h"

void drink_machine::reset_machine() {
	current_state = IDLE;
}

void drink_machine::dm_fsm() {
	switch(current_state) {
		case IDLE:
			nickel_out.write(false);
			dime_out.write(false);
			two_dime_out.write(false);
			dispense.write(false);

			if( nickel_in){
				current_state = FIVE;
			}
			else if (dime_in){
				current_state = TEN;
			}
			else if (quarter_in){
				current_state = TWENTY_FIVE;
			}
			break;
		case FIVE:
			if(nickel_in){
				current_state = TEN;
			}
			else if (dime_in){
				current_state = FIFTEEN;
			}
			else if (quarter_in){
				current_state = THIRTY;
			}
			break;
		case TEN:
			if (nickel_in){
				current_state = FIFTEEN;
			}
			else if (dime_in){
				current_state = TWENTY;
			}
			else if (quarter_in){
				current_state = THIRTY_FIVE;
			}
			break;
		case FIFTEEN:
			if (nickel_in){
				current_state = TWENTY;
			}
			else if (dime_in){
				current_state = TWENTY_FIVE;
			}
			else if (quarter_in){
				current_state = FORTY;
			}
			break;
		case TWENTY:
                        if (nickel_in)
                                current_state = TWENTY_FIVE;
                        else if (dime_in)
                                current_state = THIRTY;
                        else if (quarter_in)
                                current_state = FORTY_FIVE;
			break;
                case TWENTY_FIVE:                    
                        if(nickel_in)
                                current_state = THIRTY;
                        else if (dime_in)
                                current_state = THIRTY_FIVE;
                        else if (quarter_in) { 
                                dispense.write(true);
                        	current_state = IDLE;
			}
			break;
		case THIRTY:
                        if (nickel_in)
                                current_state = THIRTY_FIVE;
                        else if (dime_in)
                                current_state = FORTY;
                        else if (quarter_in){
                                nickel_out.write(true);
                                dispense.write(true);
                                current_state = IDLE;
                        }
                        break;
                case THIRTY_FIVE:
                        if (nickel_in)
                                current_state = FORTY;
                        else if (dime_in)
                                current_state = FORTY_FIVE;
                        else if (quarter_in){
                                dispense.write(true);
                                dime_out.write(true);
                                current_state = IDLE;
                        }
                        break;
                case FORTY:
                        if (nickel_in)
                                current_state = FORTY_FIVE;
                        else if (dime_in){
                                dispense.write(true);
                                current_state = IDLE;
                        }
                        else if (quarter_in){
                                dispense.write(true);
                                nickel_out.write(true);
                                dime_out.write(true);
                                current_state = IDLE;
                        }
                        break;
                case FORTY_FIVE:
                        if (nickel_in){
                                dispense.write(true);
                                current_state = IDLE;
                        }
                        else if (dime_in){
                                nickel_out.write(true);
                                dispense.write(true);
                                current_state = IDLE;
                        }
                        else if (quarter_in){
                                current_state = IDLE;
                                dispense.write(true);
                                two_dime_out.write(true);
                        }
                        break;

		default:
			break;
	       }
} 
