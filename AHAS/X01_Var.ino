/**********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/

/** Data Pool **/
/* Used for data exchange between webserver and sensors */

struct{
  #ifdef USE_DHT11
  char* DHT11_Status;
  float DHT11_Temperature;
  float DHT11_Humidity;
  float DHT11_DewPoint;
  #endif
  
  #ifdef USE_BMP085
  float BMP085_Temperature;
  float BMP085_Pressure;
  #endif
  
  #ifdef USE_MS5611
  float MS5611_Temperature;
  float MS5611_Pressure;
  float MS5611_AbsAltitude;
  float MS5611_RelAltitude;
  #endif
  
  #ifdef USE_DS1302
  word DS1302_Year;
  byte DS1302_Month;
  byte DS1302_Day;
  byte DS1302_Hour;
  byte DS1302_Minute;
  byte DS1302_Second;
  byte DS1302_DayOfWeek;
  char* DS1302_SyncStatus;
  #endif
  
  #ifdef USE_NTP
  word NTP_Year;
  byte NTP_Month;
  byte NTP_Day;
  byte NTP_Hour;
  byte NTP_Minute;
  byte NTP_Second;
  char* NTP_Status;
  #endif

  #ifdef USE_RGB
  byte RGB_Red;
  byte RGB_Green;
  byte RGB_Blue;
  #endif

  #ifdef USE_PIR
  byte PIR_State;
  #endif

  #ifdef USE_SOUND_DETECT
  byte SOUND_State;
  #endif
}DataPool;


typedef struct
{
  unsigned int act;
  unsigned int max;
  unsigned int min;
  float avg;
  float avgNZ;
  unsigned long int count;
  unsigned long int countNZ;
  
}RunTime_Type;

struct{
  unsigned long TimeStampStart = 0;
  RunTime_Type Task_Init = {0,0,65535,0.0,0.0,0,0};
  RunTime_Type Task_Acquisition = {0,0,65535,0.0,0.0,0,0};
  RunTime_Type Task_Display = {0,0,65535,0.0,0.0,0,0};
  RunTime_Type Task_Webduino = {0,0,65535,0.0,0.0,0,0};
  RunTime_Type Task_Log = {0,0,65535,0.0,0.0,0,0};
  RunTime_Type Task_RenewDHCP = {0,0,65535,0.0,0.0,0,0};
}RunTime;

void init_RuntimeMeasurement()
{
  //zero the variables, initialize min max values;
}

void inline startRuntimeMeasurement()
{
  /* Get the timestamp */
  RunTime.TimeStampStart = millis();
}

void endRuntimeMeasurement(RunTime_Type* module)
{
  /* Increment the counter */
  module->count++;
  /* Calculate the runtime */
  module->act = (unsigned int)(millis() - RunTime.TimeStampStart);
  /* Check the maximum */
  module->act>module->max?module->max=module->act:module->max=module->max;
  /* Check the minimum */
  module->act<module->min?module->min=module->act:module->min=module->min;
  /* Calculate the average */
  module->avg = (float)(module->avg)*((module->count-1)/(float)module->count) + (module->act)/(float)module->count; 

  /* When runtime is not zero */
  if(module->act > 1)
  {
    /* Increment the counter */
    module->countNZ++; 
    /* Calculate the average when runtime is not zero*/
    module->avgNZ = (float)(module->avgNZ)*((module->countNZ-1)/(float)module->countNZ) + (module->act)/(float)module->countNZ;
  }
  
}

void printRuntTime(RunTime_Type* module)
{
  Serial.print(F("Act:"));Serial.println(module->act);
  Serial.print(F("Max:"));Serial.println(module->max);
  Serial.print(F("Min:"));Serial.println(module->min);
  Serial.print(F("Avg:"));Serial.println(module->avg);
  Serial.print(F("AvgNZ:"));Serial.println(module->avgNZ);
  Serial.print(F("Count:"));Serial.println(module->count);
}

