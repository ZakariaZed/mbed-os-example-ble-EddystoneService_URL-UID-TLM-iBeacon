
#ifndef __BLE_IBEACON_H__
#define __BLE_IBEACON_H__

#include "cmsis_compiler.h"
#include <string.h>
#include "EddystoneTypes.h"
#include "ble/BLE.h"
#include <iostream>
#include "stdio.h"

class iBeacon
{
public:
    typedef  uint8_t LocationUUID_t[16];
typedef union Payload {
        uint8_t rawFrame[25];
        struct { 
            uint16_t companyID;
            uint8_t len;
            uint8_t ID;      
            uint8_t proximityUUID[16];
            uint16_t majorNumber;
            uint16_t minorNumber;
            uint8_t txPower;
        };

        Payload(LocationUUID_t uuid, uint16_t majNum, uint16_t minNum, uint8_t transmitPower, uint16_t companyIDIn) :
              companyID(companyIDIn),len(0x15), ID(0x02), majorNumber(__REV16(majNum)), minorNumber(__REV16(minNum)), txPower(transmitPower)
        {
            memcpy(proximityUUID, uuid, sizeof(LocationUUID_t));
        }
    } Payload;
        iBeacon();
        iBeacon(LocationUUID_t  uuid,
            uint16_t        majNum,
            uint16_t        minNum,
            uint8_t         txP,
            uint16_t        compID);  
        
void setiBeacon(LocationUUID_t  uuid,
            uint16_t        majNum,
            uint16_t        minNum,
            uint8_t         txP,
            uint16_t        compID);


        void constructiBeacon(uint8_t *rawFrame);
        uint8_t* GetrawFrame(void);
        uint16_t GetcompanyID(void);
        uint16_t GetmajorNumber(void);
        uint16_t GetminorNumber(void);
        uint8_t GettxPower(void);

        size_t getRawFrameSize(void) const;

    /*  uint8_t getcompanyIDSize(void) ;
        uint8_t getmajorNumberSize(void) ;
        uint8_t getminorNumber(void) ;
        uint8_t gettxPower(void) ;*/
        Payload  data;

 protected :

    static const uint8_t FRAME_TYPE_IBEACON = 0x30;

    static const uint8_t FRAME_SIZE_IBEACON = 25;
        
        
};

typedef iBeacon iBeaconService; 

   

#endif //__BLE_IBEACON_H__
