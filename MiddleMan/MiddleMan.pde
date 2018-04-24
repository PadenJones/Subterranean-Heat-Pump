import processing.serial.*;

Serial arduinoSerial;
Serial arduinoDebug;
String arduinoSerialLabel = "COM5";
String arduinoDebugLabel = "COM10";
String hostWebsite = "http://localhost/";

void setup() {
  try {
    arduinoSerial = new Serial(this, arduinoSerialLabel, 57600);
    arduinoSerial.bufferUntil('\n');
    arduinoDebug = new Serial(this, arduinoDebugLabel, 57600);
    arduinoDebug.bufferUntil('\n');
  } catch(Exception e) {
    println("Error: failed to reserve serial ports.");
    exit();
  }
}

void draw() {
  delay(250);
  if (arduinoSerial.available() > 0) {
    String message = arduinoSerial.readString();
    
    try {
      apiRequest(parseJSONObject(message.trim()));
    } catch (Exception e) {
      arduinoDebug.write("Arduino:" + message);
    }
  }
}

void apiRequest(JSONObject request) {
  String data = request.toString().replaceAll("\\s+", "");
  String url = hostWebsite + "api.php/?data=" + data;

  println("MiddleMan  request: " + url);

  try {
    String[] response = loadStrings(url);
    if (response.length != 0) {
      try {
        println("MiddleMan response: " + response[0].trim());
        parseJSONObject(response[0].trim());
        arduinoSerial.write(response[0]);
      } catch (Exception e) {
        println("apiRequest() - response not JSON array: " + response[0]);
      }
    } else {
      println("apiRequest() - empty response array"); 
    }
  } catch (Exception e) {
    println("apiRequest() - URL request failed.");
  }  
}