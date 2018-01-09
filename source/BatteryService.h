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

#ifndef __BLE_BATTERY_SERVICE_H__
#define __BLE_BATTERY_SERVICE_H__

#include "ble/BLE.h"
#include "SecurityManager.h"
#include "GattCharacteristic.h"
#include "custom_helper.h"
#include "GattCallbackParamTypes.h"
#include "GattServer.h"

#include "AuthentificationService.h"
/**
* @class BatteryService
* @brief BLE Battery Service. This service displays the battery level from 0% to 100%, represented as an 8bit number.
* Service:  https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.battery_service.xml
* Battery Level Char:  https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.battery_level.xml
*/
class BatteryService {
public:

/**
  * Cette fonction permet l'autorisation de la lecture dans les carachtéristiques des services.
**/
void basicAuthorizationCallback (GattReadAuthCallbackParams *Params)
{
    /**
  * Authorisation de la lecture de valeur celon la valeur du code
    * si le code est bon : AUTH_CALLBACK_REPLY_SUCCESS 
    * sinon AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED
    **/
        
    if ( Authentif == 12)
       { Params->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;}
    else {
    Params->authorizationReply =AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED;}
}

    /**
     * @param[in] _ble
     *               BLE object for the underlying controller.
     * @param[in] level
     *               8bit batterly level. Usually used to represent percentage of batterly charge remaining.
     */
                //requireSecurity(SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM);
    BatteryService(BLE &_ble, uint8_t level = 100) :
        ble(_ble),
        batteryLevel(level),
        batteryLevelCharacteristic(GattCharacteristic::UUID_BATTERY_LEVEL_CHAR, &batteryLevel){//, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY) {
                        
        batteryLevelCharacteristic.setReadAuthorizationCallback(this, &BatteryService::basicAuthorizationCallback);
                    
                GattCharacteristic *charTable[] = {&batteryLevelCharacteristic};
        GattService         batteryService(GattService::UUID_BATTERY_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
                ble.addService(batteryService);
            }
    /**
     * @brief Update the battery level with a new value. Valid values lie between 0 and 100,
     * anything outside this range will be ignored.
     *
     * @param newLevel
     *              Update to battery level.
     */
            
    void updateBatteryLevel(uint8_t newLevel) {
        batteryLevel = newLevel;
        ble.gattServer().write(batteryLevelCharacteristic.getValueHandle(), &batteryLevel, 1);
    }
        
        
/* 
 *  fonction qui permet de récupérer la valeur du Bon code introduite dans la fonction AuthentificationService.     
*/      
        void recuperationauth (uint8_t T)
        {
            Authentif = T;
        }

protected:
    /**
     * A reference to the underlying BLE instance that this object is attached to.
     * The services and characteristics will be registered in this BLE instance.
     */
    BLE &ble;

    /**
     * The current battery level represented as an integer from 0% to 100%.
         * Déclaration de la variable contenant la valeur retourné par le service Authentification.
     */
    uint8_t    batteryLevel;
        uint8_t    Authentif;
    /**
     * A ReadOnlyGattCharacteristic that allows access to the peer device to the
     * batteryLevel value through BLE.
     */
    ReadOnlyGattCharacteristic<uint8_t> batteryLevelCharacteristic;
};

#endif /* #ifndef __BLE_BATTERY_SERVICE_H__*/
