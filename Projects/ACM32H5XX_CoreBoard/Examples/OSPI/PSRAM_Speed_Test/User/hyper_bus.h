/******************************************
*   W958D8NBYA 256Mb HyperRAM
******************************************/
#ifndef __HYPER_BUS_H__
#define __HYPER_BUS_H__

#include  "hal.h" 

extern OSPI_HandleTypeDef HYPER_Handle;

#define CA_LEN                                          (6)

#define  HYPER_ID_REGISTER0_CA_47_40                    0xC0//0xE0  //CA[47:40]
#define  HYPER_ID_REGISTER0_CA_39_32                    0x00//CA[39:32]
#define  HYPER_ID_REGISTER0_CA_31_24                    0x00//CA[31:24]
#define  HYPER_ID_REGISTER0_CA_23_16                    0x00//CA[23:16]
#define  HYPER_ID_REGISTER0_CA_15_8                     0x00//CA[15:8]
#define  HYPER_ID_REGISTER0_CA_7_0                      0x00//CA[7:0]

#define  HYPER_ID_REGISTER1_CA_47_40                    0xC0//0xE0  //CA[47:40]
#define  HYPER_ID_REGISTER1_CA_39_32                    0x00//CA[39:32]
#define  HYPER_ID_REGISTER1_CA_31_24                    0x00//CA[31:24]
#define  HYPER_ID_REGISTER1_CA_23_16                    0x00//CA[23:16]
#define  HYPER_ID_REGISTER1_CA_15_8                     0x00//CA[15:8]
#define  HYPER_ID_REGISTER1_CA_7_0                      0x01//CA[7:0]

#define  HYPER_CONFIG_REGISTER0_READ_CA_47_40           0xC0//0xE0  //CA[47:40]
#define  HYPER_CONFIG_REGISTER0_READ_CA_39_32           0x00//CA[39:32]
#define  HYPER_CONFIG_REGISTER0_READ_CA_31_24           0x01//CA[31:24]
#define  HYPER_CONFIG_REGISTER0_READ_CA_23_16           0x00//CA[23:16]
#define  HYPER_CONFIG_REGISTER0_READ_CA_15_8            0x00//CA[15:8]
#define  HYPER_CONFIG_REGISTER0_READ_CA_7_0             0x00//CA[7:0]

#define  HYPER_CONFIG_REGISTER0_WRITE_CA_47_40          0x60///CA[47:40]
#define  HYPER_CONFIG_REGISTER0_WRITE_CA_39_32          0x00//CA[39:32]
#define  HYPER_CONFIG_REGISTER0_WRITE_CA_31_24          0x01//CA[31:24]
#define  HYPER_CONFIG_REGISTER0_WRITE_CA_23_16          0x00//CA[23:16]
#define  HYPER_CONFIG_REGISTER0_WRITE_CA_15_8           0x00//CA[15:8]
#define  HYPER_CONFIG_REGISTER0_WRITE_CA_7_0            0x00//CA[7:0]

#define  HYPER_CONFIG_REGISTER1_READ_CA_47_40           0xC0//0xE0  //CA[47:40]
#define  HYPER_CONFIG_REGISTER1_READ_CA_39_32           0x00//CA[39:32]
#define  HYPER_CONFIG_REGISTER1_READ_CA_31_24           0x01//CA[31:24]
#define  HYPER_CONFIG_REGISTER1_READ_CA_23_16           0x00//CA[23:16]
#define  HYPER_CONFIG_REGISTER1_READ_CA_15_8            0x00//CA[15:8]
#define  HYPER_CONFIG_REGISTER1_READ_CA_7_0             0x01//CA[7:0]

#define  HYPER_CONFIG_REGISTER1_WRITE_CA_47_40          0x60///CA[47:40]
#define  HYPER_CONFIG_REGISTER1_WRITE_CA_39_32          0x00//CA[39:32]
#define  HYPER_CONFIG_REGISTER1_WRITE_CA_31_24          0x01//CA[31:24]
#define  HYPER_CONFIG_REGISTER1_WRITE_CA_23_16          0x00//CA[23:16]
#define  HYPER_CONFIG_REGISTER1_WRITE_CA_15_8           0x00//CA[15:8]
#define  HYPER_CONFIG_REGISTER1_WRITE_CA_7_0            0x01//CA[7:0]


#define HYPER_MEMORY_WRITE                              (0x00)
#define HYPER_MEMORY_READ                               (0x80)

#define HYPER_WRAPPED_BURST                             (0x00)
#define HYPER_LINEAR_BURST                              (0x20)



void W958D8NBYA_OSPI_HyperBus_Test(void);


#endif








