/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "iBeacon.h"

iBeacon::iBeacon():
data(0,0,0,0,0)
{
    
}
iBeacon::iBeacon(LocationUUID_t  uuid,
            uint16_t        majNum,
            uint16_t        minNum,
            uint8_t         txP    = 0xb3,
            uint16_t        compID = 0x004C) :
      data(uuid, majNum, minNum, txP, compID)
    {
                //Payload i = Payload(uuid, majNum, minNum, txP, compID);
    }
void iBeacon::setiBeacon(LocationUUID_t  uuid,
            uint16_t        majNum,
            uint16_t        minNum,
            uint8_t         txP    = 0xb3,
            uint16_t        compID = 0x004C)
    {
            
            data = Payload(uuid, majNum, minNum, txP, compID);

    }

void iBeacon::constructiBeacon(uint8_t *rawFrame)
{
    size_t index = 5;
        data.companyID=0x004C;
        data.majorNumber=3344;
      data.minorNumber=1122;
        data.txPower=0xb3;
    
/*      data.rawFrame [index] = 0x004C;
        data.rawFrame [index++] = 0x02;
        data.rawFrame [index++] = 0x15;
    */
       
        data.proximityUUID [index] = 0xf7;
        data.proximityUUID [index++] = 0x82;
        data.proximityUUID [index++] = 0x6d;
        data.proximityUUID [index++] = 0xa6;
        data.proximityUUID [index++] = 0x4f;
        data.proximityUUID [index++] = 0xa2;
        data.proximityUUID [index++] = 0x4e;
        data.proximityUUID [index++] = 0x98;
        data.proximityUUID [index++] = 0x80;
        data.proximityUUID [index++] = 0x24;
        data.proximityUUID [index++] = 0xbc;
        data.proximityUUID [index++] = 0x5b;
        data.proximityUUID [index++] = 0x71;
        data.proximityUUID [index++] = 0xe0;
        data.proximityUUID [index++] = 0x89;
        data.proximityUUID [index++] = 0x3e;
        
/*      data.rawFrame [index++] = 11;
        data.rawFrame [index++] = 22;
        data.rawFrame [index++] = 33;
        data.rawFrame [index++] = 44;
        data.rawFrame [index++] = 0xC8; 
        */
   /*     data.rawFrame [index] = 0x02;
        data.rawFrame [index++] = 0x01;
        data.rawFrame [index++] = 0x06;
        data.rawFrame [index++] = 0x1a;
        data.rawFrame [index++] =  0xff;
        data.rawFrame [index++] = 0x4c;
        data.rawFrame [index++] = 0x00;
        data.rawFrame [index++] = 0x02;
        data.rawFrame [index++] =  0x15;
        data.rawFrame [index++] = 0xf7;
        data.rawFrame [index++] = 0x82;
        data.rawFrame [index++] = 0x6d;
        data.rawFrame [index++] =  0xa6;
        data.rawFrame [index++] = 0x4f;
        data.rawFrame [index++] = 0xa2;
        data.rawFrame [index++] = 0x4e;
        data.rawFrame [index++] =  0x98;
        data.rawFrame [index++] = 0x80;
        data.rawFrame [index++] = 0x24;
        data.rawFrame [index++] = 0xbc;
        data.rawFrame [index++] = 0x5b ;
        data.rawFrame [index++] = 0x71;
        data.rawFrame [index++] = 0xe0;
        data.rawFrame [index++] = 0x89;
        data.rawFrame [index++] =  0x3e;
        data.rawFrame [index++] = 0x21;
        data.rawFrame [index++] = 0x2c;
        data.rawFrame [index++] = 0x0B;
        data.rawFrame [index++] =  0x16;
        data.rawFrame [index++] = 0xb3;**/
        
        memcpy(rawFrame, data.rawFrame,sizeof(data.rawFrame));
     /*   printf("data rawFrame 2=%d\n",data.rawFrame[10]);
        printf("data rawFrame 3=%d\n",data.rawFrame[11]);
        printf("data rawFrame 4=%d\n",data.rawFrame[12]);*/
}

size_t iBeacon::getRawFrameSize(void) const
{
    return FRAME_SIZE_IBEACON;
    // + EDDYSTONE_UUID_SIZE;
}
uint8_t* iBeacon::GetrawFrame(void)
{
    return data.rawFrame;
}
uint16_t iBeacon::GetmajorNumber(void)
{
    return data.majorNumber;
}
uint16_t iBeacon::GetminorNumber(void)
{
    return data.minorNumber;
}
uint8_t iBeacon::GettxPower(void)
{
    return data.txPower;
}
uint16_t iBeacon::GetcompanyID(void)
{
    return data.companyID;
}