/** ThingSpeak **/

#include "AHAS_Config.h"
#include "App_Var.h"
#include "App_SD.h"
#include "App_IO.h"
#include "ThingSpeak.h"

#ifdef USE_THINGSPEAK
#include "App_ThingSpeak.h"
#include "Ethernet.h"

EthernetClient EthThingSpeakClient;

unsigned long myChannelNumber = 184750;
const char * myWriteAPIKey = "AV1UNTYPGCCMDVCU";

void init_ThingSpeak()
{
    ThingSpeak.begin(EthThingSpeakClient);

    MONITOR_LOG_LN(F("#INIT: ThingSpeak => DONE"));
}

void updateThingSpeak()
{
    ThingSpeak.setField(1, DataPool.DHT11_Temperature);
    ThingSpeak.setField(2, DataPool.DHT11_Humidity);
    ThingSpeak.setField(3, DataPool.BMP085_Temperature);
    ThingSpeak.setField(4, DataPool.BMP085_Pressure / (float)100.0);
    ThingSpeak.setField(5, DataPool.MS5611_Temperature);
    ThingSpeak.setField(6, DataPool.MS5611_Pressure / (float)100.0);
    ThingSpeak.setField(7, DataPool.SOUND_State);
    ThingSpeak.setField(8, DataPool.PIR_State);

    // Write the fields that you've set all at once.
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    MONITOR_LOG_LN(F("#ThingSpeak Updated"));
}
#endif


