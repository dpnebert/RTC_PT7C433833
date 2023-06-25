


#ifndef RTC_PT7C433833_H_
#define RTC_PT7C433833_H_


#include <Arduino.h>  // Arduino data type definitions
#include <Wire.h>     // Standard I2C "Wire" library
#include <RTClib.h>
#include <iostream>
#include <vector>

/* Register map */
/* rtc section */

#define PT7C_BIT_EOSC       128

#define PT7C_REG_SC         0x00
#define PT7C_REG_MN         0x01
#define PT7C_REG_HR         0x02
#define PT7C_REG_DW         0x03
#define PT7C_REG_DT         0x04
#define PT7C_REG_MO         0x05
#define PT7C_REG_YR         0x06
#define PT7C_REG_CT         0x07
/* i2c configuration */
#define PT7C_I2C_ADDR       0x68

#define RTC_SECTION_LEN     0x07

#define PT7C_BIT_12_24      64
#define PT7C_BIT_PM         32
#define PT7C_BIT_H10        16
#define PT7C_BIT_H8         8
#define PT7C_BIT_H4         4
#define PT7C_BIT_H2         2
#define PT7C_BIT_H1         1

#define PT7C_BIT_OUT     	128
#define PT7C_BIT_OSF        32
#define PT7C_BIT_SQWE       16

#define SECONDS_FROM_1970_TO_2000 946684800 ///< Unixtime for 2000-01-01 00:00:00, useful for initialization
#define SECS_1970_TO_2000         946684800 ///< Unixtime for 2000-01-01 00:00:00, useful for initialization

#define REGISTER_MASK_SECONDS	((uint8_t)0b1111111)
#define REGISTER_MASK_MINUTES	((uint8_t)0b1111111)
#define REGISTER_MASK_HOURS_12H	((uint8_t)0b11111)
#define REGISTER_MASK_HOURS_24H	((uint8_t)0b111111)
#define REGISTER_MASK_DOW		((uint8_t)0b111)
#define REGISTER_MASK_DATES		((uint8_t)0b111111)
#define REGISTER_MASK_MONTHS	((uint8_t)0b11111)
#define REGISTER_MASK_YEARS		((uint8_t)0b11111111)

#define REGISTER_MASK_RS    ((uint8_t)0b11)
#define REGISTER_MASK_PM   ((uint8_t)0b100000)


#if !defined(BUFFER_LENGTH)  // The ESP32 Wire library doesn't currently define BUFFER_LENGTH
/** @brief If the "Wire.h" library doesn't define the buffer, do so here */
#define BUFFER_LENGTH 32
#endif






class RTC_PT7C433833 {
    /*!
    @class   RTC_PT7C433833
    @brief   Class for interfacing with the RTC_PT7C433833 RTC
    */
public:
  RTC_PT7C433833();
	void begin();
 
  bool start();
  bool stop();
  void setDateTime(DateTime dt);
  DateTime now();

	uint8_t getSquareWavePinOutputLevel();
  void setSquareWavePinOutputLevel(uint8_t level);

	bool isOscillatorRunning();
	void startOscillator();

	bool isSquareWaveEnabled();
	void enableSquareWave(bool e);
 
  bool isOscillatorEnabled();
  void enableOscillator(bool e);

  uint8_t getClockSystem();
  void set12HourClockSystem();
  void set24HourClockSystem();
  

	uint8_t getRateSelect();
	void setRateSelect(uint8_t value);




	uint16_t getYear();
	void setYear(uint16_t value);

	uint8_t getMonth();
	void setMonth(uint8_t value);

	uint8_t getDate();
	void setDate(uint8_t value);

	uint8_t getDaysOfTheWeek();
	void setDaysOfTheWeek(uint8_t value);

	uint8_t getHour();
	void setHour(uint8_t value);

  bool isHourPM();
  void setAM();
  void setPM();
	
	uint8_t getMinute();
	void setMinute(uint8_t value);
	
	uint8_t getSecond();
	void setSecond(uint8_t value);
	
	
	
    uint8_t bcd2int(uint8_t bcd);
    uint8_t int2bcd(uint8_t dec);

    void clearRegisterBit(uint8_t reg, uint8_t b);
    void setRegisterBit(uint8_t reg, uint8_t b);
	  uint8_t readRegisterBit(uint8_t reg, uint8_t b);
	
	  uint8_t writeByte(uint8_t reg, uint8_t data);	
    uint8_t readByte(uint8_t reg, uint8_t* data);


	uint8_t I2C_write(uint8_t busAddress, uint8_t startAddress, uint8_t data);
	uint8_t I2C_read(uint8_t busAddress, uint8_t startAddress, uint8_t* data);

private:


}; // of class RTC_PT7C433833 definition

#endif
