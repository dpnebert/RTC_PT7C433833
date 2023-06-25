#include "RTC_PT7C433833.h"




/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***   RTC    **************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

typedef enum {
  rate_1,
  rate_4k096,
  rate_8k192,
  rate_32k768
} RATE_SELECT;


RTC_PT7C433833::RTC_PT7C433833() {
  
}
void RTC_PT7C433833::begin() {
  Wire.begin();  
  Wire.setClock((uint32_t)100000);
}

bool RTC_PT7C433833::start() {
  enableOscillator(true);
  startOscillator();
  delay(5);                                       // Wait for oscillator to stabilize
  for (uint8_t j = 0; j < 255; j++)            // Loop until changed or overflow
  {
    if(isOscillatorRunning()) return true;
    delay(5);           
  }
  return false;
}
bool RTC_PT7C433833::stop() {
  enableOscillator(false);
  for (uint8_t j = 0; j < 255; j++) {
    if( !isOscillatorRunning() ) return true;
    delay(5);
  }
  return false;
}
void RTC_PT7C433833::setDateTime(DateTime dt) {
  // rtc.adjust(DateTime(y, m, d, hh, mm, ss));
  setYear(dt.year());
  setMonth(dt.month());
  setDate(dt.day());
  setHour(dt.hour());
  setMinute(dt.minute());
  setSecond(dt.second());
}
DateTime RTC_PT7C433833::now() {
  

  // DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour=0, uint8_t min=0, uint8_t sec=0)
  return DateTime(getYear(), getMonth(), getDate(), getHour(), getMinute(), getSecond());
  
}









uint8_t RTC_PT7C433833::getSquareWavePinOutputLevel() {
  // It controls the output level of the SQW/OUT pin when the square wave output is disabled
	return readRegisterBit(PT7C_REG_CT, PT7C_BIT_OUT);
}
void RTC_PT7C433833::setSquareWavePinOutputLevel(uint8_t level) {
  // It controls the output level of the SQW/OUT pin when the square wave output is disabled
  if(level) setRegisterBit(PT7C_REG_CT, PT7C_BIT_OUT);
  else clearRegisterBit(PT7C_REG_CT, PT7C_BIT_OUT);
}





bool RTC_PT7C433833::isOscillatorEnabled() {
  if(readRegisterBit(PT7C_REG_SC, PT7C_BIT_EOSC)) return 1;
  else return 0;
}

void RTC_PT7C433833::enableOscillator(bool e) {
  if(e) clearRegisterBit(PT7C_REG_SC, PT7C_BIT_EOSC);
  else setRegisterBit(PT7C_REG_SC, PT7C_BIT_EOSC);
}


bool RTC_PT7C433833::isOscillatorRunning() {
  if(readRegisterBit(PT7C_REG_CT, PT7C_BIT_OSF)) return false;
  else return true; 
}

void RTC_PT7C433833::startOscillator() {
	clearRegisterBit(PT7C_REG_CT, PT7C_BIT_OSF);
}





bool RTC_PT7C433833::isSquareWaveEnabled() {
  if(readRegisterBit(PT7C_REG_CT, PT7C_BIT_SQWE)) return true;
  else return false;
}
void RTC_PT7C433833::enableSquareWave(bool e) {
  if(e)	setRegisterBit(PT7C_REG_CT, PT7C_BIT_SQWE);
  else clearRegisterBit(PT7C_REG_CT, PT7C_BIT_SQWE);
}










uint8_t RTC_PT7C433833::getRateSelect() {
	uint8_t value;
	readByte(PT7C_REG_CT, &value);
	value &= REGISTER_MASK_RS;
	return value;
}
void RTC_PT7C433833::setRateSelect(uint8_t value) {
	uint8_t rs = value & REGISTER_MASK_RS;
	uint8_t oldValue;
	readByte(PT7C_REG_CT, &oldValue);
	oldValue &= 0b10000000;
	writeByte(PT7C_REG_CT, oldValue & rs);
}





uint16_t RTC_PT7C433833::getYear() {
	uint8_t value;
	readByte(PT7C_REG_YR, &value);
	value &= REGISTER_MASK_YEARS;
	return bcd2int(value) + 2000;
}
void RTC_PT7C433833::setYear(uint16_t value) {
	uint8_t year = int2bcd(value - 2000) & REGISTER_MASK_YEARS;
	writeByte(PT7C_REG_YR, year);
}





uint8_t RTC_PT7C433833::getMonth() {
	uint8_t value;
	readByte(PT7C_REG_MO, &value);
	value &= REGISTER_MASK_MONTHS;
	return bcd2int(value);
}
void RTC_PT7C433833::setMonth(uint8_t value) {
	uint8_t month = int2bcd(value) & REGISTER_MASK_MONTHS;
	writeByte(PT7C_REG_MO, month);
}








uint8_t RTC_PT7C433833::getDate() {
	uint8_t value;
	readByte(PT7C_REG_DT, &value);
	value &= REGISTER_MASK_DATES;
	return bcd2int(value);
}
void RTC_PT7C433833::setDate(uint8_t value) {
	uint8_t date = int2bcd(value) & REGISTER_MASK_DATES;
	writeByte(PT7C_REG_DT, date);
}








uint8_t RTC_PT7C433833::getDaysOfTheWeek(){
	uint8_t value;
	readByte(PT7C_REG_DW, &value);
	value &= REGISTER_MASK_DOW;
	return value;
}
void RTC_PT7C433833::setDaysOfTheWeek(uint8_t value) {
	uint8_t dow = int2bcd(value) & REGISTER_MASK_DOW;
	writeByte(PT7C_REG_DW, dow);
}



uint8_t RTC_PT7C433833::getHour() {
	uint8_t value;
	readByte(PT7C_REG_HR, &value);
	//Serial.print("value:");Serial.println(value);
	//Serial.print("getClockSystem():");Serial.println(getClockSystem());
	if(getClockSystem()) value &= REGISTER_MASK_HOURS_12H;
	else value &= REGISTER_MASK_HOURS_24H;
	return bcd2int(value);
}
void RTC_PT7C433833::setHour(uint8_t value) {
	
	// value == 0000 0001
	uint8_t hour = int2bcd(value);
	//Serial.print("BCD Hour = ");Serial.println(hour, BIN);
	// hour == 0000 0001
	//if(getClockSystem()) hour &= REGISTER_MASK_HOURS_12H;
	//else hour &= REGISTER_MASK_HOURS_24H;
	// REGISTER_MASK_HOURS_24H == 111111
	
	// hour == 0000 0001
	uint8_t oldValue;
	readByte(PT7C_REG_HR, &oldValue);
	// oldValue == 0000 0000
  oldValue &= 0b1000000;
  oldValue |= hour;
	//Serial.print("oldValue = ");Serial.println(oldValue, BIN);
	// oldValue == 0000 0000
	writeByte(PT7C_REG_HR, oldValue | hour);
}


bool RTC_PT7C433833::isHourPM() {
  return readRegisterBit(PT7C_REG_HR, PT7C_BIT_PM);
}
void RTC_PT7C433833::setAM() {
  clearRegisterBit(PT7C_REG_HR, REGISTER_MASK_PM);
}

void RTC_PT7C433833::setPM() {
  setRegisterBit(PT7C_REG_HR, REGISTER_MASK_PM);
}

uint8_t RTC_PT7C433833::getMinute() {
	uint8_t value;
	readByte(PT7C_REG_MN, &value);
	value &= REGISTER_MASK_MINUTES;
	return bcd2int(value);
}
void RTC_PT7C433833::setMinute(uint8_t value) {
	uint8_t minute = int2bcd(value) & REGISTER_MASK_MINUTES;
	writeByte(PT7C_REG_MN, minute);
}




uint8_t RTC_PT7C433833::getSecond() {
	uint8_t value;
	readByte(PT7C_REG_SC, &value);
	value &= REGISTER_MASK_SECONDS;
	return bcd2int(value);
}
void RTC_PT7C433833::setSecond(uint8_t value) {
	uint8_t second = int2bcd(value) & REGISTER_MASK_SECONDS;
	uint8_t oldValue;
	readByte(PT7C_REG_SC, &oldValue);
  oldValue &= PT7C_BIT_EOSC;
	writeByte(PT7C_REG_SC, oldValue | second);
}





uint8_t RTC_PT7C433833::getClockSystem() {
	return readRegisterBit(PT7C_REG_HR, PT7C_BIT_12_24);
}
void RTC_PT7C433833::set12HourClockSystem() {
	setRegisterBit(PT7C_REG_HR, PT7C_BIT_12_24);
}
void RTC_PT7C433833::set24HourClockSystem() {
	//Serial.print(__func__);Serial.println(":");
	//Serial.print("PT7C_REG_HR: 0x");Serial.println((uint8_t)PT7C_REG_HR, HEX);
	//Serial.print("PT7C_BIT_12_24:");Serial.println((uint8_t)PT7C_BIT_12_24, BIN);
	clearRegisterBit(PT7C_REG_HR, PT7C_BIT_12_24);
}





uint8_t RTC_PT7C433833::bcd2int(uint8_t bcd) {
  return ((bcd / 16 * 10) + (bcd % 16));
}  // of method bcd2int

uint8_t RTC_PT7C433833::int2bcd(uint8_t dec) {
  return ((dec / 10 * 16) + (dec % 10));
}  // of method int2bcd






void RTC_PT7C433833::clearRegisterBit(uint8_t reg, uint8_t b) {
	uint8_t value;
	uint8_t rc = readByte(reg, &value);
	//Serial.print(__func__);Serial.println((uint8_t)value, BIN);
	
	if(rc == 0) {
		value &= ~(b);
		//Serial.print(__func__);Serial.println((uint8_t)value, BIN);
	
		writeByte(reg, value);
	}
}

void RTC_PT7C433833::setRegisterBit(uint8_t reg, uint8_t b) {
	uint8_t value;
	uint8_t rc = readByte(reg, &value);
	if(rc == 0) {
		value |= b;
		writeByte(reg, value);
	}
}

uint8_t RTC_PT7C433833::readRegisterBit(uint8_t reg, uint8_t b) {
	uint8_t value;
	uint8_t rc = readByte(reg, &value);
	//Serial.print("value=");Serial.println(value, BIN);
	//Serial.print("b=");Serial.println(b, BIN);
	if(value & b) return 1;
	return 0;
}









uint8_t RTC_PT7C433833::writeByte(uint8_t reg, uint8_t data) {
	return I2C_write((uint8_t)PT7C_I2C_ADDR, reg, data);
}	



uint8_t RTC_PT7C433833::readByte(uint8_t reg, uint8_t* data) {
	return I2C_read((uint8_t)PT7C_I2C_ADDR, reg, data);
}



uint8_t RTC_PT7C433833::I2C_write(uint8_t busAddress, uint8_t startAddress, uint8_t data) {
	Wire.beginTransmission(busAddress);
	Wire.write(startAddress);
	Wire.write(data);
	return Wire.endTransmission();
}




uint8_t RTC_PT7C433833::I2C_read(uint8_t busAddress, uint8_t startAddress, uint8_t* data) {
	Wire.beginTransmission(busAddress);
	Wire.write(startAddress);	
	int rc = Wire.endTransmission();
	uint8_t count = 1;
	if(rc == 0) {
		Wire.requestFrom(busAddress, count);
		*data = Wire.read();
	}	
	return rc;
}
