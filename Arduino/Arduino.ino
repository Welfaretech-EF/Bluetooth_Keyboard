/*

Created by: Simon Kjems Jørgensen (2020-08-08)
Modified by: Mikkel Damgaard Justiniano (2023-07-19)
Organsation: The Elsass Foundation

This is a Bluetooth keyboard that is configurable
to send most normal and frequently used keys.

It uses a webserver to be configured with
a simple webinterface. 

The esp32 must be configured to use huge 
partition (3mb no OTA / 1mb SPIFFS)
*/

//Includes 
//Wifi used to configurer the BleKeyboard
#include <WiFi.h>

//Handle the bluetooth low energy keyboard
#include "BleKeyboard.h"
#include "BleMouse.h"

#include <Preferences.h>

struct Button {
    const uint8_t PIN;
    uint32_t iKey;
    uint32_t lastChange;
    bool isPressed;
};

Button buttons[] = {
  {5,104,0,false},
  {18,106,0,false},
  {19,105,0,false},
  {21,107,0,false},
  {0,0,0,false},
  {4,96,0,false},
  {16,97,0,false},
  {17,98,0,false}
};

uint8_t nButtons;

int nAsciis;
int nKeys;
int nMediaKeys;

//Set the name of the bluetooth keyboard
BleKeyboard bleKeyboard("Elsass Keyboard");
//BleMouse bleMouse("Elsass Mouse");

//Webserver config
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//Create webserver on port 80
WiFiServer server(80);

//Setup preferences
Preferences preferences;

bool isDK = true;
String wifiName;
String blName;
void loadPreferences(){
  Serial.println("Loading previous settings");
  preferences.begin("Elsass", false);
  isDK = preferences.getBool("isDK", true);
  wifiName = preferences.getString("wifiName", "Elsass Keyboard");
  blName = preferences.getString("blName", "Elsass Keyboard");
  bleKeyboard.setName(blName.c_str());
  if(isDK){
    Serial.println("Language: DK");
  }else{
    Serial.println("Language: EN");
  }
  
  Serial.println("Wifi Name: " + wifiName);
  Serial.println("Bluetooth Name: " + blName);
  
  for(int i = 0; i < nButtons; i++){
    String iStr = String(i);
    int key = preferences.getUInt(iStr.c_str(), -1);
    if(key != -1){
      buttons[i].iKey = key;
    }
    Serial.println("Button" + String(i) + " = " + String(buttons[i].iKey));
  }
  preferences.end();
}

String sendHTML(){
  String html = R"HTML(<!DOCTYPE html><html><head><meta charset="utf-8" name="viewport" http-equiv="Content-Type" content="width=device-width, initial-scale=1.0"> 
<title>Elsass</title><link rel="icon" href="data:,">
<style>
select{
  margin-top: 4px;
  width: 50%;
  margin-left:25%;
}
.select-selected {
  background-color: DodgerBlue;
}
div {
  width: 50%;
  margin: 0 auto;
  position: relative;
} 
.hover:hover {
    cursor: pointer;
}
form {  
  border-radius:5px
  border-color: gray;  
  border-width: 5px;  
  border-style: double;  
}
</style></head>
<body>
<form action="Change">Keyboard layout<br/>)HTML";
if(isDK){
  html+= R"HTML(<input style="background:green;color:white;" type="submit" name="language" value="DK" disabled><input class="hover" style="background:white;color:black" type="submit" name="language" value="EN">)HTML";
}else{
  html+= R"HTML(<input class="hover" style="background:white;color:black" type="submit" name="language" value="DK"><input style="background:green;color:white" type="submit" name="language" value="EN" disabled>)HTML";
}
html+="</form>";
html+= R"HTML(
  <br/>)HTML";
html+= "<form action=\"Reset\" onsubmit=\"alert('The device will now restart and you might need to reconnect')\">";
html+="<input class=\"hover\" style=\"width: 100%;background-color:#4CAF50;color:white;padding:14px 20px;margin: 8px 0;border: none;border-radius: 4px\" type=\"submit\" value=\"Reset (Will restart the device)\"></form><br/>";
html+= "<form action=\"Rename\" onsubmit=\"alert('The device will now restart and you might need to reconnect')\"><label for=\"wifi\">Wifi Name:</label><br/><input type=\"text\" id=\"wifi\" name=\"wifi\" value=\"" + wifiName + "\"><br/>";
html+= "<label for=\"bluetooth\">Bluetooth Name:</label><br/><input type=\"text\" id=\"bluetooth\" name=\"bluetooth\" value=\"" + blName + "\"><br/>";
html+= "<input class=\"hover\" style=\"width: 100%;background-color:#4CAF50;color:white;padding:14px 20px;margin: 8px 0;border: none;border-radius: 4px\" type=\"submit\" value=\"Save Names (Will restart the device)\"></form>";
html+= R"HTML(
    <br/><form action="Change"><div id = "content"></div>
    <input class="hover" style="width: 100%;background-color:#4CAF50;color:white;padding:14px 20px;margin: 8px 0;border: none;border-radius: 4px" type="submit" value="Save Buttons"></form>
  </form>
</body>
)HTML";

html += "<script>var def = ";
html += "[";
for(int i = 0; i < nButtons; i++){
  if(i != 0){
    html += ",";
  }
  html += buttons[i].iKey;
}
html += "];";
html += "\r\nvar str = [";
for(int i = 0; i < nAsciis;i++){
  if(i != 0){
    html += ",";
  }
  if(isDK){
    html += "\"" + asciis[i].str + "\"";
  }
  else{
    html += "\"" + asciisEN[i].str + "\"";
  }
}
for(int i = 0; i < nKeys;i++){
  html += ",\"" + keys[i].str + "\"";
}
for(int i = 0; i < nMediaKeys;i++){
  html += ",\"" + MediaKeyReports[i].str + "\"";
}

html += "];";
// Create HTML code to swithc keyboard layout (DK or EN)
html += R"HTML(
var text = "";
for (var i = 0; i < def.length; i++){
  text += "#" + (i+1) + "<select name=\"button";
  text += i;
  text += "\">";
  for (var j = 0; j < str.length; j++) {
    text += "<option value=\"";
    text += j;
    if(def[i] == j){
      text += "\" selected=>";
    }else{
      text += "\">";
    }
    text += str[j];
    text += "</option>";
  }
  text += "</select><br/>";
}
document.getElementById("content").innerHTML = text;
</script>
</html>)HTML";
  return html;
}
void press(uint32_t iKey){
  if (iKey < nAsciis){
    if(isDK){
      bleKeyboard.press(asciis[iKey].code);
    }else{
      bleKeyboard.press(asciisEN[iKey].code);
    }
  }
  else if(iKey < nAsciis+nKeys){
    bleKeyboard.press(keys[iKey-nAsciis].code);
  }
  else{
    bleKeyboard.press(MediaKeyReports[iKey-nAsciis-nKeys].code);
  }
}
void release(uint32_t iKey){
  if (iKey < nAsciis){
    if(isDK){
      bleKeyboard.release(asciis[iKey].code);
    }else{
      bleKeyboard.release(asciisEN[iKey].code);
    }
  }
  else if(iKey < nAsciis+nKeys){
    bleKeyboard.release(keys[iKey-nAsciis].code);
  }
  else{
    bleKeyboard.release(MediaKeyReports[iKey-nAsciis-nKeys].code);
  }
}

void isr(void* arg) {
  Button* s = static_cast<Button*>(arg);
  if(millis()-s->lastChange>10){
    if(digitalRead(s->PIN) == s->isPressed){
      s->lastChange = millis();
      s->isPressed = !s->isPressed;
      if(s->isPressed){
        press(s->iKey);
      }else{
        release(s->iKey);
      }
    }
  }
}

void setup(){
  nButtons = sizeof(buttons)/sizeof(buttons[0]);
  nAsciis = sizeof(asciis)/sizeof(asciis[0]);
  nKeys = sizeof(keys)/sizeof(keys[0]);
  nMediaKeys = sizeof(MediaKeyReports)/sizeof(MediaKeyReports[0]);

  //Begin serial communication on speed 115200
  Serial.begin(115200);

  //Get the lastest data from the preferences
  loadPreferences();

  //Begin the BLE keyboard/start advertising the keyboard
  Serial.print("Setting up BLE");
  bleKeyboard.begin();
  Serial.println(" - Done");

  //To limit errors
  delay(2000);

  //Button pins
  Serial.print("Setting up PINS");
  for(int i = 0; i < nButtons;i++){
    pinMode(buttons[i].PIN, INPUT_PULLUP);
    attachInterruptArg(buttons[i].PIN, isr, &buttons[i], CHANGE);
  }
  Serial.println(" - Done");

  //To limit errors
  delay(100);
  
  Serial.print("Setting up WIFI");
  WiFi.softAP(wifiName.c_str());
  WiFi.softAPConfig(local_ip, gateway, subnet);
  Serial.println(" - Done");
  
  //To limit errors
  delay(100);

  //Start the server
  Serial.print("Starting server");
  server.begin();
  Serial.println(" - Done");

  Serial.println("Setup Finished");
}

String getHTML(String line,String id){
  String result = "";
  int i0 = line.indexOf(id);
  if(i0 != -1){
    int i1 = line.indexOf("=",i0);
    if(i1 != -1){
      int i2 = i1+1;
      int i3 = line.indexOf("&",i2);
      if(i3 == -1){
        i3 = line.indexOf(" ",i2);
        if(i3 != -1){
          result = line.substring(i2,i3);
        }
      }else{
        result = line.substring(i2,i3);
      }
    }
  }
  result.replace("+"," ");
  return result;
}

void handleServer(){
  WiFiClient client = server.available();
  if (client) {
    bool restart = false;
    String header = "";
    Serial.println("New Client.");
    String currentLine = "";
    bool get = false;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() != 0) {
            int iGet = currentLine.indexOf("GET");
            if(iGet != -1){
              if(currentLine.indexOf("Reset?")!=-1){
                Serial.println("Resetting and restarting");
                preferences.begin("Elsass", false);
                preferences.clear();
                preferences.end();
                restart = true;
              }
              else if(currentLine.indexOf("Rename?")!=-1){
                Serial.println("Changing names and restarting");
                String wifi = getHTML(currentLine,"wifi");
                if(wifi != ""){
                  get = true;
                  wifiName = wifi;
                  Serial.println("Wifi Name: " + wifiName);
                }
                String bluetooth = getHTML(currentLine,"bluetooth");
                if(bluetooth != ""){
                  get = true;
                  blName = bluetooth;
                  Serial.println("Bluetooth Name: " + blName);
                }
                preferences.begin("Elsass", false);
                preferences.putString("wifiName", wifiName);
                preferences.putString("blName", blName);
                preferences.end();
                restart = true;
              }else{
                String language = getHTML(currentLine,"language");
                if(language != ""){
                  get = true;
                  if(language.equals("DK")){
                    isDK = true;
                  }else if(language.equals("EN")){
                    isDK = false;
                  }
                  if(isDK){
                    Serial.println("Language: DK");
                  }else{
                    Serial.println("Language: EN");
                  }
                }
                for(int i = 0; i < nButtons;i++){
                  String button = getHTML(currentLine,"button"+String(i));
                  if(button != ""){
                    get = true;
                    buttons[i].iKey = button.toInt();
                    Serial.println("Button" + String(i) + " = " + button);
                  }
                }
                if(get){
                  preferences.begin("Elsass", false);
                  for (int i = 0; i < nButtons; i++){
                    String iStr = String(i);
                    preferences.putUInt(iStr.c_str(), buttons[i].iKey);
                  }
                  preferences.putBool("isDK", isDK);
                  preferences.putString("wifiName", wifiName);
                  preferences.putString("blName", blName);
                  preferences.end();
                }
      			  }
            }
            currentLine = "";
          }
          else{
            break;
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    Serial.println(header);
    if(get){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection: close");
        client.println();
        client.println(R"HTML(<!DOCTYPE html><html><head><script>window.onload=function(){if ('referrer' in document) {window.location = document.referrer;} else {window.history.back();}};</script></head></html>)HTML");
    }
      else{
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection: close");
        client.println();
        client.println(sendHTML());
    }
    
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    if(restart){
      ESP.restart();
    }
  }
}
long lastServer = 0;
void loop(){
  if(abs(long(millis()-lastServer))>1000){
    handleServer();
    lastServer = millis();
  }
}
