#include <stdio.h>
#include <stdlib.h>

#include "state_machine_elevator.h"

#define FAILURE -1
#define SUCCESS  0
typedef unsigned int unint32;


typedef enum
{
    STATE_RUN = 1,
    STATE_STOP,
    STATE_CLOSE,
    STATE_OPEN,

    STATE_BUTT
}STATE_E,OPRERATE_E;

typedef struct elevator_state_table_s
{
    unint32 cru_state;       //open or  close
    unint32 next_state;      //run  or  stop
    //int (*elevator_operation)(elevator* elevator,OPRERATE_E operate);
}elevator_state_table;

typedef struct elevator_s
{
    elevator_state_table_s *elevator_state_table;
    unint32 cru_state;      //open  or  close
}elevator;

/*TODO: Reserved for complex need
int elevator_working(elevator* elevator,OPRERATE_E operate)
{
    elevator->cru_state = STATE_RUN;
    elevator.elevator_state_table->cru_state = STATE_CLOSE;
    elevator.elevator_state_table->next_state = STATE_RUN;

}
*/

int elevator_operation(elevator* elevator,OPRERATE_E operate)
{
    if(elevator == NULL) {
        printf("[error] the pts is null\n")
    }
     switch(operate) {
        case STATE_RUN:
             if ((elevator.elevator_state_table->cru_state == STATE_CLOSE) &&
                (elevator->next_state == STATE_STOP)) {
                    elevator->cru_state = STATE_RUN;
                    elevator.elevator_state_table->cru_state = STATE_CLOSE;
                    elevator.elevator_state_table->next_state = STATE_RUN;
                    printf("[OK] run SUCCESS\n");
                    return SUCCESS;
                } else {
                    printf("[ERROR] run failed\n");
                    return FAILURE;
                }

        case STATE_STOP:
            if (elevator->next_state == STATE_RUN) {
                    elevator->cru_state = STATE_STOP;
                    elevator.elevator_state_table->next_state = STATE_STOP;
                    printf("[OK] stop SUCCESS\n");
                    return SUCCESS;
                } else {
                    printf("[ERROR] stop failed\n");
                    return FAILURE;
                }

        case STATE_CLOSE:
            if ((elevator.elevator_state_table->cru_state == STATE_OPEN) &&
                (elevator->next_state == STATE_STOP)) {
                    elevator.elevator_state_table->cru_state = STATE_CLOSE;
                    printf("[OK] open SUCCESS\n");
                    return SUCCESS;
                } else {
                    printf("[ERROR] open failed\n");
                    return FAILURE;
                }

        case STATE_OPEN:
            if ((elevator.elevator_state_table->cru_state == STATE_CLOSE) &&
                (elevator->next_state == STATE_STOP)) {
                    elevator.elevator_state_table->cru_state = STATE_OPEN;
                    printf("[OK] open SUCCESS\n");
                    return SUCCESS;
                } else {
                    printf("[ERROR] open failed\n");
                    return FAILURE;
                }

        default:
            return 0;
     }
}

int elevator_init(elevator* elevator)
{
    elevator->cru_state = STATE_CLOSE;
    elevator.elevator_state_table->cru_state = STATE_CLOSE;
    elevator.elevator_state_table->next_state = STATE_STOP;

    return SUCCESS;
}

