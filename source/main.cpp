/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <events/mbed_events.h>

#include <mbed.h>
#include "ble/BLE.h"
#include "EddystoneService.h"
#include "BatteryService.h"
#include "AuthentificationService.h"
#include "PersistentStorageHelper/ConfigParamsPersistence.h"
#include "iBeacon.h"

EddystoneService *eddyServicePtr;

/* Duration after power-on that config service is available. */
static const int CONFIG_ADVERTISEMENT_TIMEOUT_SECONDS = 30;

/* Default UID frame data */
static const UIDNamespaceID_t uidNamespaceID = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
static const UIDInstanceID_t  uidInstanceID  = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

/* Default version in TLM frame */
static const uint8_t tlmVersion = 0x00;
static uint8_t temperature = 0;
static uint8_t batteryLevel = 0;
static BatteryService* batteryServicePtr;
   uint8_t uuid[] = {0xf7, 0x82, 0x6d, 0xa6, 0x4f, 0xa2, 0x4e, 0x98,
                            0x80, 0x24, 0xbc, 0x5b, 0x71, 0xe0, 0x89, 0x3e};
    uint16_t majorNumber = 1122;
    uint16_t minorNumber = 3344;
    uint8_t txPower     = 0xb3;
    uint16_t companyID = 0x004C;
    
static AuthentificationService* AuthentificationServicePtr;

/* Values for ADV packets related to firmware levels, calibrated based on measured values at 1m */
static const PowerLevels_t defaultAdvPowerLevels = {-47, -33, -21, -13};
/* Values for radio power levels, provided by manufacturer. */
static const PowerLevels_t radioPowerLevels      = {-30, -16, -4, 4};

static EventQueue eventQueue(/* event count */ 32 * EVENTS_EVENT_SIZE);

DigitalOut led(LED1, 1);
DigitalOut led2(LED2, 1);
DigitalOut led3(LED3, 1);
DigitalOut led4(LED4, 1);

/**
 * Callback triggered upon a disconnection event.
 */
static void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *cbParams)
{
     uint8_t L=0;
     AuthentificationServicePtr->resetcode(L);
     
    (void) cbParams;
    BLE::Instance().gap().startAdvertising();
}

/**
 * Callback triggered some time after application started to switch to beacon mode.
 */


static void timeout(void)
{
     BLE &ble = BLE::Instance();
    Gap::GapState_t state;
    state = BLE::Instance().gap().getState();
    if (!state.connected) { /* don't switch if we're in a connected state. */
			led2 = !led2;
        eddyServicePtr->startBeaconService();
        AuthentificationServicePtr= new AuthentificationService(ble);
        batteryServicePtr = new BatteryService(ble, batteryLevel);
        eddyServicePtr->startConfigServiceafterdeconnexion();
        EddystoneService::EddystoneParams_t params;
        eddyServicePtr->getEddystoneParams(params);
        saveEddystoneServiceConfigParams(&params);
    } else {
			led3 = !led3;
        eventQueue.call_in(1000, timeout);
    }
}
void updateSensorValue() 
{

uint8_t L = AuthentificationServicePtr->T;
    // Incrémentation de la batterie:.
                batteryLevel++;
    if (batteryLevel > 100) {
        batteryLevel = 20;
    }
// envoie de la nouvelle valeur de la batterie ainsi la valeur déterminante du bon et mauvais code. 
        
    batteryServicePtr->updateBatteryLevel(batteryLevel);
        batteryServicePtr->recuperationauth(L);
}

void updatetempSensorValue() 
{   
        uint8_t L = AuthentificationServicePtr->T;
        eddyServicePtr->authentificationcondition(L);   
      eddyServicePtr->updatetemperatureValues(temperature);
    
}

static void blinky(void)
{
    led = !led;
    BLE &ble = BLE::Instance();
/*
 * Appel de la fonction updatetempSensorValue chaque 500 ms.
 */ 
    
    if (ble.gap().getState().connected) {
            
            eventQueue.call(updatetempSensorValue);

    }
}

 static void onBleInitError(BLE::InitializationCompleteCallbackContext* initContext)
{
    /* Initialization error handling goes here... */
    (void) initContext;
}

static void initializeEddystoneToDefaults(BLE &ble)
{
    /* Set everything to defaults */
    AuthentificationServicePtr= new AuthentificationService(ble);
    eddyServicePtr = new EddystoneService(ble, defaultAdvPowerLevels, radioPowerLevels, eventQueue);
    batteryServicePtr = new BatteryService(ble, batteryLevel);
 
    /* Set default URL, UID and TLM frame data if not initialized through the config service */
    const char* url = YOTTA_CFG_EDDYSTONE_DEFAULT_URL;
    eddyServicePtr->setURLData(url);
    eddyServicePtr->setUIDData(uidNamespaceID, uidInstanceID);
    eddyServicePtr->setTLMData(tlmVersion);
    eddyServicePtr->setiBeacon(uuid, majorNumber, minorNumber ,txPower,companyID);    
}

static void bleInitComplete(BLE::InitializationCompleteCallbackContext* initContext)
{
	led4 = !led4;
    BLE         &ble  = initContext->ble;
    ble_error_t error = initContext->error;

    if (error != BLE_ERROR_NONE) {
        onBleInitError(initContext);
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);

    EddystoneService::EddystoneParams_t params;
  //  eddyServicePtr->setiBeacon(uuid, majorNumber, minorNumber ,txPower,companyID);  
    if (loadEddystoneServiceConfigParams(&params)) {
        AuthentificationServicePtr= new AuthentificationService(ble);
        eddyServicePtr = new EddystoneService(ble, params, radioPowerLevels, eventQueue);
        batteryServicePtr = new BatteryService(ble, batteryLevel); 
     } else {
        initializeEddystoneToDefaults(ble);
    }
    /* Start Eddystone in config mode */
   eddyServicePtr->startConfigService();
    
   eventQueue.call_in( 1000, timeout);
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}


int main()
{
    /* Tell standard C library to not allocate large buffers for these streams */
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);

    eventQueue.call_every(500, blinky);
    eventQueue.call_every(70,updateSensorValue );
    
    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();
    return 0;
}
