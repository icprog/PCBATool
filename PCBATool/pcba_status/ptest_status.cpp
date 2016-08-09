#include "stdafx.h"
#include "..\..\targetdevice\common\target.h"
#include "ptest_status.h"
#define PLATFORM_WATCH 1



int ptest_test_result_load(CTarget *dut,struct STATUS_DATA* pStatus)
{
#if PLATFORM_WATCH
    int values;
    if(0 == lord_data_from_nvm(dut,4,&values)) {
        if(values&ITEM_SIM_MASK) 
            pStatus->sim     = 1;

        if(values&ITEM_WIFI_MASK) 
            pStatus->wifi    = 1;

        if(values&ITEM_SENSOR_MASK) 
            pStatus->snesor  = 1;

        if(values&ITEM_BT_MASK) 
            pStatus->bt      = 1;
        
        if(values&ITEM_CODEC_MASK) 
            pStatus->codec   = 1;

        if(values&ITEM_GPS_MASK) 
            pStatus->gps     = 1;

        if(values&ITEM_CAMERA_MASK) 
            pStatus->camera  = 1;

        if(values&ITEM_VALID_MASK)
            pStatus->valid   = 1;
        pStatus->status = (values&ITEM_TEST_STATUS)>>STATUS_BIT_OFFSET;
    } else {

        return -1;
    }
#endif
    return 0;
}
int ptest_status_get(CTarget *dut,int *nstatus)
{
#if PLATFORM_WATCH
    int values;
    if(0 == lord_data_from_nvm(dut,4,&values)) {
        *nstatus = (values&ITEM_TEST_STATUS)>>STATUS_BIT_OFFSET;
        return 0;
    }
    return -1;
#else 


#endif

}

int store_data_to_nvm(CTarget *dut,int index,int values)
{
    if(dut->write_param(index,values)){
        return 0;
    }
    return -1;
}
int lord_data_from_nvm(CTarget *dut,int index,int *pvalues)
{
    unsigned short values;
    if(dut->read_param(index,values)){
        *pvalues = values;
        return 0;
    }
    return 0;

}

int flush_data_to_nvm(CTarget *dut) 
{
    return 0;
}















