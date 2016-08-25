/**********************************************************************************************************/
/*** Function Declarations ***/
/***********************************************************************************************************/
void Task_Init_Callback();
void Task_Acquisition_Callback();
void Task_Display_Callback();
void Task_Webduino_Callback();
void Task_Log_Callback();
void Task_RenewDHCP_Callback();

/***********************************************************************************************************/
/*** Global Variable definition  ***/
/***********************************************************************************************************/

/** Task Definitions **/
Task Task_Init(TASK_IMMEDIATE, TASK_ONCE, &Task_Init_Callback, &TaskScheduler, true);
Task Task_Acquisition(TASK_SECOND, TASK_FOREVER, &Task_Acquisition_Callback, &TaskScheduler, true);
Task Task_Display(2*TASK_SECOND, TASK_FOREVER, &Task_Display_Callback, &TaskScheduler, false);
Task Task_Webduino(TASK_SECOND/2, TASK_FOREVER, &Task_Acquisition_Callback, &TaskScheduler, false);
Task Task_Log(TASK_MINUTE, TASK_FOREVER, &Task_Log_Callback, &TaskScheduler, false);
Task Task_RenewDHCP(TASK_HOUR, TASK_FOREVER, &Task_RenewDHCP_Callback, &TaskScheduler, false);

/** Data Pool **/
/* Used for data exchange between webserver and sensors */

struct{
  #ifdef USE_DHT11
  char* DHT11_Status;
  int DHT11_Temperature;
  int DHT11_Humidity;
  int DHT11_DewPoint;
  #endif
  
  #ifdef USE_BMP085
  int BMP085_Temperature;
  long BMP085_Pressure;
  #endif
  
  #ifdef USE_MS5611
  double MS5611_Temperature;
  long MS5611_Pressure;
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
  unsigned long int count;
}RunTime_Type;

struct{
  unsigned long TimeStampStart = 0;
  RunTime_Type Task_Init = {0,0,65535,0.0,0};
  RunTime_Type Task_Acquisition = {0,0,65535,0.0,0};
  RunTime_Type Task_Display = {0,0,65535,0.0,0};
  RunTime_Type Task_Webduino = {0,0,65535,0.0,0};
  RunTime_Type Task_Log = {0,0,65535,0.0,0};
  RunTime_Type Task_RenewDHCP = {0,0,65535,0.0,0};
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
  /* Calculate the runtime */
  module->act = (unsigned int)(millis() - RunTime.TimeStampStart);
  /* Check the maximum */
  module->act>module->max?module->max=module->act:module->max=module->max;
  /* Check the minimum */
  module->act<module->min?module->min=module->act:module->min=module->min;
  /* Calculate the average */
  module->avg = (float)(module->avg)*((module->count-1)/(float)module->count) + (module->act)/(float)module->count; 
}

void printRuntTime(RunTime_Type* module)
{
  Serial.print("Act:");Serial.println(module->act);
  Serial.print("Max:");Serial.println(module->max);
  Serial.print("Min:");Serial.println(module->min);
  Serial.print("Avg:");Serial.println(module->avg);
  Serial.print("Count:");Serial.println(module->count);
}

