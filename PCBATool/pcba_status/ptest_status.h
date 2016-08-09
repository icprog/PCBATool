#ifndef _PTEST_STATUS_H_
#define _PTEST_STATUS_H_
struct STATUS_DATA
{
    unsigned int sim:        1;
    unsigned int wifi        :1;
    unsigned int snesor      :1;
    unsigned int bt          :1;
    unsigned int codec       :1;
    unsigned int gps         :1;
    unsigned int camera      :1;
    unsigned int reserved1   :1;
    unsigned int reserved2   :1;
    unsigned int reserved3   :1;
    unsigned int reserved4   :1;
    unsigned int reserved5   :1;
    unsigned int reserved6   :1;
    unsigned int valid       :1;
    unsigned int status      :2; /*0 idle 1 testing 2 done**/
};
#define ITEM_SIM_MASK           (0x1)
#define ITEM_WIFI_MASK          (0x2)
#define ITEM_SENSOR_MASK        (0x4)
#define ITEM_BT_MASK            (0x8)
#define ITEM_CODEC_MASK         (0x10)
#define ITEM_GPS_MASK           (0x20)
#define ITEM_CAMERA_MASK        (0x40)
#define ITEM_RESERVED_BIT1      (0x80)
#define ITEM_RESERVED_BIT2      (0x100)
#define ITEM_RESERVED_BIT3      (0x200)
#define ITEM_RESERVED_BIT4      (0x400)
#define ITEM_RESERVED_BIT5      (0x800)
#define ITEM_RESERVED_BIT6      (0x1000)
#define ITEM_VALID_MASK         (0x2000)
#define ITEM_TEST_STATUS        (0xC000)

#define STATUS_IDLE             0
#define STATUS_TESTING          1
#define STATUS_PASS             2
#define STATUS_DONE             3
#define STATUS_BIT_OFFSET       14

#define ST_IDLE                 0  
#define ST_TESTING              1
#define ST_PASS                 2
#define ST_DONE                 3   /*just test finish ,ok or fail dependes on users **/
#define ST_FAIL                 4

#define RESULT_INVALID          0
#define RESULT_VALID            1

/*mode variable **/
#define MODE_PTEST              0
#define MODE_NORMAL             1
#define MODE_AIRPLANE           2
#define MODE_MINIMAL            3
#define MODE_NONE               4

int     ptest_test_result_load  (CTarget *dut,struct STATUS_DATA* pStatus);
int     ptest_status_get        (CTarget *dut,int *nstatus);
int     store_data_to_nvm       (CTarget *dut,int index,int values);
int     lord_data_from_nvm      (CTarget *dut,int index,int *pvalues);
int     flush_data_to_nvm       (CTarget *dut);
#endif
