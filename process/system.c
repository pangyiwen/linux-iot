#include <stdlib.h>
#include <stdio.h>

#include "system.h"
#include "process.h"

#ifdef __cplusplus
extern "C" {
#endif


sys_info* g_sys_info;
int g_time_stamp = 0;

int init_sys()
{

    g_sys_info.resource_costed = (sys_resource*)malloc(sizeof(sys_resource));
    memset(g_sys_info.resource_costed,0,sizeof(sys_resource));

    g_sys_info.resource_costed.sys_cpu = 0;                      //100%
    g_sys_info.resource_costed.sys_ram = 1000;                   //MB--4G
    g_sys_info.resource_costed.sys_rom = 10000;                  //MB--50G

    g_sys_info.resource_remain = (sys_resource*)malloc(sizeof(sys_resource));
    memset(g_sys_info.resource_remain,0,sizeof(sys_resource));

    g_sys_info.resource_remain.sys_cpu = SYS_CPU_MAX;                      //100%
    g_sys_info.resource_remain.sys_ram = SYS_RAM_MAX - 1000;                   //MB--4G
    g_sys_info.resource_remain.sys_rom = SYS_ROM_MAX - 10000;                  //MB--50G

    return 0;
}

int running(sys_info* sys_info)
{
    return 0;
}

int main()
{
    int g_sys_on = 0;
    init_sys();

    g_sys_on = 1;

    while(g_sys_on){

        running(g_sys_info);

    }

}


#ifdef __cplusplus
}
#endif

