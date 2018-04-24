#ifndef WEB_H
#define WEB_H

#include <ArduinoJson.h>

class Web {
  public:
    String lastMessage;
    String lastResponse;
  
    Web();
    bool apiRequest(JsonObject& data, String& response, long waitPeriod);
    bool saveStateToWeb(double nodeB, double nodeC, int zone);
    bool getInstructions(int& instruction);
};

Web::Web() {
  lastMessage = "";
  lastResponse = "";
}

bool Web::apiRequest(JsonObject& data, String& response, long waitPeriod)
{
  data.printTo(Serial);

  bool timeout = false;
  long start = millis();
  while (!timeout)
  {
    if (Serial.available() > 0)
    {
      response = Serial.readString();
      lastResponse = "Last Response: " + response;
      return true;
    }

    if (millis() - start > waitPeriod)
    {
      timeout = true;
    }
  }

  return !timeout;
}

bool Web::getInstructions(int& instruction)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data["method"] = "getInstructions";
  data["apiKey"] = 89406;

  String response = "";
  if (apiRequest(data, response, 1000))
  {
    lastMessage = "Web::getInstructions() Response: " + response;

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject(response);
    instruction = data["targetZone"];

    return true;
  }
  else
  {
    lastMessage = "Web::getInstructions() Error: API request timed out...";
    return false;
  }
}

bool Web::saveStateToWeb(double nodeB, double nodeC, int zone)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data["method"] = "saveState";
  data["apiKey"] = 89406;
  data["zone1"] = nodeB;
  data["zone2"] = nodeC;
  data["zone"] = zone;

  String response = "";
  if (apiRequest(data, response, 1000))
  {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject(response);

    String message = data["message"];
    if (data["status"])
    {
      lastMessage = "Web::saveStateToWeb() Success: " + message;
      return true;
    }
    else
    {
      lastMessage = "Web::saveStateToWeb() Failed: " + message;
      return false;
    }
  }
  else
  {
    lastMessage = "Web::saveStateToWeb() Error: API request timed out...";
    return false;
  }
}

#endif
