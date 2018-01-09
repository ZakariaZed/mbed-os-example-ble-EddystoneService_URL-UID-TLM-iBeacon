/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef __BLE_AUTHENTIFICATION_SERVICE_H__
#define __BLE_AUTHENTIFICATION_SERVICE_H__

#include <mbed.h>
#include "ble/BLE.h"
#include "SecurityManager.h"
#include "GattCharacteristic.h"
#include "custom_helper.h"
#include <iostream>
#include <vector>

/**
  * Déclaration et l'utilisation d'une trace Série pour l'affichage des données.
**/

Serial pc(USBTX, USBRX);



/**
* @class BatteryService
* @brief BLE Battery Service. This service displays the battery level from 0% to 100%, represented as an 8bit number.
* Service:  https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.battery_service.xml
* Battery Level Char:  https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.battery_level.xml
*/
class AuthentificationService {
public:
    
/** 
* Déclaration de variables global: 
    * vecteur contenant la valeur du bon code. 
**/
    std::vector<uint8_t> recupvalue;    
        uint8_t                                 c;
        uint8_t                                 T;
/*
 * Fonction du callback afin de récuperer la valeur envoyer par le client.
 */
void onDataWrittenCallback(const GattWriteCallbackParams *writeParams)
{
    c=0;
  if(writeParams->handle == WChar.getValueHandle())
        {
                WValue.clear();
                recupvalue.clear();
            
        /**
            * Etablir le code = 1234.
        **/         
                    recupvalue.push_back(1);
                    recupvalue.push_back(2);
                    recupvalue.push_back(3);
                    recupvalue.push_back(4);
                    
                for ( int i=0; i< 4;i++)
                    {
                        WValue.push_back(writeParams->data[i]);
                        if (WValue[i]==recupvalue[i])
                        {c++;}
                    }
            for ( int x=0; x< 4;x++)
                {
                    if (WValue[x]==recupvalue[x+1])
                    {c++;}
                }
            if ( c==4)
            {
                /**
                * Valeur indiquant le bon code. T=12.
                **/
                T=12;
            }
            else {
                /**
                * Forcer la déconnexion une fois le code est mauvais.
                **/
                Gap::DisconnectionReason_t res=Gap::LOCAL_HOST_TERMINATED_CONNECTION;
                ble.gap().disconnect(res);
                }
    }
}
/**
* Création du service Authentification en mode écriture. 
**/


 
    AuthentificationService(BLE &_ble) :
        ble(_ble),
         WChar(UUID_AUTHENTIFICATION_PASS_CHAR, &WValue){ 
                    

        GattCharacteristic *charTable[] = {&WChar}; 
        GattService         authentificationService(UUID_AUTHENTIFICATION_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
                T=0;
                ble.gattServer().onDataWritten(this, &AuthentificationService::onDataWrittenCallback);
        ble.addService(authentificationService);
                
                
    }

void resetcode (uint8_t L)
{
    T=L;
}
protected:
   
/**
* Déclaration du vecteur introduit par le client pour mettre le code.
* Déclaration du service authentification en mode écriture.
**/      
    BLE &ble;
     std::vector<uint8_t> WValue;
    const  uint16_t UUID_AUTHENTIFICATION_PASS_CHAR  = 0x2A9A;
     WriteOnlyArrayGattCharacteristic<std::vector<uint8_t>, sizeof(WValue)> WChar;
  //   const uint8_t  UUID_AUTHENTIFICATION_PASS_CHAR[] = {0xee, 0x0c, 0x2A, 0x9A, 0x87, 0x86, 0x40, 0xba, 0xab, 0x96, 0x99, 0xb9, 0x1a, 0xc9, 0x81, 0xd8};
  //   const uint8_t UUID_AUTHENTIFICATION_SERVICE []= {0x00, 0x00, 0x17, 0x04, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};
    
  const   uint16_t UUID_AUTHENTIFICATION_SERVICE  = 0x1704;
    
        
};

#endif /* #ifndef __BLE_AUTHENTIFICATION_SERVICE_H__*/
