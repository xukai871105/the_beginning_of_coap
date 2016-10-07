#include <ArduinoJson.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  char json[] = "{\"red\":128, \"green\":127, \"blue\":32}";

	StaticJsonBuffer<64> jsonBuffer;

	JsonObject& root = jsonBuffer.parseObject(json);

  int red = root["red"];
  int green = root["green"];
  int blue= root["blue"];

  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
}

void loop() {
  // put your main code here, to run repeatedly:

}
