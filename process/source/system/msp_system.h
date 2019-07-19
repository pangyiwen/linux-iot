#ifndef  __MSP_SYSTEM_H_
#define  __MSP_SYSTEM_H_

#define SYS_CPU_MAX   100
#define SYS_ROM_MAX   50000
#define SYS_RAM_MAX   4000

typedef struct  sys_resource{
    int sys_cpu;
    int sys_rom;
    int sys_ram;
}sys_resource;

typedef enum{
    STATE_FINISHED = 0,
    STATE_READY,
    STATE_BLOCKED,
    STATE_RUNNING
}process_state_e;

typedef struct process_info{
    sys_resource* process_cost_resource;
    int process_priority;
    process_state_e process_state;
    int time_piece;

    int blocked_time;
    int waiting_time;

    process_info* next;
}process_info;

typedef struct sys_process_info{
    process_info* list_running;
    process_info* list_ready;
    process_info* list_blocked;
    process_info* list_finished;
}sys_process_info;

typedef struct sys_info{
    sys_resource*  resource_costed;
    sys_resource*  resource_remain;

    sys_process_info* process_info;
}sys_info;

int init_sys();

#endif
