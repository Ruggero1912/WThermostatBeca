#ifndef THERMOSTAT_paciosoft_H
#define	THERMOSTAT_paciosoft_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "WThermostat.h"

class WThermostat_paciosoft : public WThermostat {
public :
  WThermostat_paciosoft(WNetwork* network, WProperty* thermostatModel, WClock* wClock)
    : WThermostat(network, thermostatModel, wClock) {
    network->debug(F("WThermostat_paciosoft created"));
  }

  virtual void configureCommandBytes() {
    this->byteDeviceOn = 0x01;
    this->byteTemperatureActual = 0x03;
    this->byteTemperatureTarget = 0x02;
    this->byteTemperatureFloor = NOT_SUPPORTED; //forse è 12 
    this->temperatureFactor = 1.0f;
    this->byteSchedulesMode = 0x04;
    this->byteLocked = 0x06;
    this->byteSchedules = NOT_SUPPORTED;
    this->byteSchedulingPosHour = 0;
    this->byteSchedulingPosMinute = 1;
    this->byteSchedulingDays = 8;
    //custom parameters
    this->byteStatusMode = 105;
  }

  virtual void initializeProperties() {
    WThermostat::initializeProperties();
    this->schedulesMode->clearEnums();
    this->schedulesMode->addEnumString(SCHEDULES_MODE_OFF);
    this->schedulesMode->addEnumString(SCHEDULES_MODE_HOLIDAY);
    this->schedulesMode->addEnumString(SCHEDULES_MODE_ECO);
    this->schedulesMode->addEnumString(SCHEDULES_MODE_AUTO);
    this->schedulesMode->addEnumString(SCHEDULES_MODE_HOLD);
    //status mode
    this->statusMode = new WProperty("statusMode", "Status", STRING, TYPE_HEATING_COOLING_PROPERTY);
    this->statusMode->addEnumString(STATE_OFF);
    this->statusMode->addEnumString(STATE_HEATING);
    this->statusMode->setReadOnly(true);
    this->statusMode->setVisibility(MQTT);
    this->addProperty(statusMode);
  }

protected :

  virtual bool processStatusCommand(byte cByte, byte commandLength) {
		//Status report from MCU
		bool changed = false;
		bool knownCommand = WThermostat::processStatusCommand(cByte, commandLength);

		if (!knownCommand) {
      //const char* newS;
      /*if (cByte == this->byteXXX) {
        if (commandLength == 0xXX) {
          newS = systemMode->getEnumString(receivedCommand[10]);
          if (newS != nullptr) {
            changed = ((changed) || (systemMode->setString(newS)));
            knownCommand = true;
          }
        }
			}
    }*/
		if (changed) {
			notifyState();
		}
	  return knownCommand;
  }
  return false;
  } 
private :

WProperty* statusMode;
byte byteStatusMode;

};

#endif
