// uncomment the following line to use NimBLE library
//#define USE_NIMBLE

#ifndef ESP32_BLE_KEYBOARD_H
#define ESP32_BLE_KEYBOARD_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#if defined(USE_NIMBLE)

#include "NimBLECharacteristic.h"
#include "NimBLEHIDDevice.h"

#define BLEDevice                  NimBLEDevice
#define BLEServerCallbacks         NimBLEServerCallbacks
#define BLECharacteristicCallbacks NimBLECharacteristicCallbacks
#define BLEHIDDevice               NimBLEHIDDevice
#define BLECharacteristic          NimBLECharacteristic
#define BLEAdvertising             NimBLEAdvertising
#define BLEServer                  NimBLEServer

#else

#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"

#endif // USE_NIMBLE

#include "Print.h"

#define BLE_KEYBOARD_VERSION "0.0.4"
#define BLE_KEYBOARD_VERSION_MAJOR 0
#define BLE_KEYBOARD_VERSION_MINOR 0
#define BLE_KEYBOARD_VERSION_REVISION 4

//Struct to translate from server response to hexcode
typedef struct{
  uint8_t code;
  String str;
} Key;

const Key asciisEN[] = {
  {32,"Space"},
  {33,"!"},
  {34,"\\\""},
  {35,"#"},
  {36,"$"},
  {37,"%"},
  {38,"&"},
  {39,"'"},
  {40,"("},
  {41,")"},
  {42,"*"},
  {43,"+"},
  {44,","},
  {45,"-"},
  {46,"."},
  {47,"/"},
  {48,"0"},
  {49,"1"},
  {50,"2"},
  {51,"3"},
  {52,"4"},
  {53,"5"},
  {54,"6"},
  {55,"7"},
  {56,"8"},
  {57,"9"},
  {58,":"},
  {59,";"},
  {60,"<"},
  {61,"="},
  {62,">"},
  {63,"?"},
  {64,"@"},
  {65,"A"},
  {66,"B"},
  {67,"C"},
  {68,"D"},
  {69,"E"},
  {70,"F"},
  {71,"G"},
  {72,"H"},
  {73,"I"},
  {74,"J"},
  {75,"K"},
  {76,"L"},
  {77,"M"},
  {78,"N"},
  {79,"O"},
  {80,"P"},
  {81,"Q"},
  {82,"R"},
  {83,"S"},
  {84,"T"},
  {85,"U"},
  {86,"V"},
  {87,"W"},
  {88,"X"},
  {89,"Y"},
  {90,"Z"},
  {91,"["},
  {92,"\\\\"},
  {93,"]"},
  {94,"^"},
  {95,"_"},
  {96,"`"},
  {97,"a"},
  {98,"b"},
  {99,"c"},
  {100,"d"},
  {101,"e"},
  {102,"f"},
  {103,"g"},
  {104,"h"},
  {105,"i"},
  {106,"j"},
  {107,"k"},
  {108,"l"},
  {109,"m"},
  {110,"n"},
  {111,"o"},
  {112,"p"},
  {113,"q"},
  {114,"r"},
  {115,"s"},
  {116,"t"},
  {117,"u"},
  {118,"v"},
  {119,"w"},
  {120,"x"},
  {121,"y"},
  {122,"z"},
  {123,"{"},
  {124,"|"},
  {125,"}"},
  {126,"~"},
};
const Key asciis[] = {
  {32,"Space"},
  {33,"!"},
  {34,"\\\""},
  {35,"#"},
  {36,"¤"},
  {37,"%"},
  {38,"/"},
  {40,")"},
  {41,"="},
  {42,"("},
  {43,"`"},
  {44,","},
  {45,"+"},
  {46,"."},
  {47,"-"},
  {48,"0"},
  {49,"1"},
  {50,"2"},
  {51,"3"},
  {52,"4"},
  {53,"5"},
  {54,"6"},
  {55,"7"},
  {56,"8"},
  {57,"9"},
  {60,";"},
  {61,"´"},
  {62,":"},
  {63,"_"},
  {64,"\\\""},
  {65,"A"},
  {66,"B"},
  {67,"C"},
  {68,"D"},
  {69,"E"},
  {70,"F"},
  {71,"G"},
  {72,"H"},
  {73,"I"},
  {74,"J"},
  {75,"K"},
  {76,"L"},
  {77,"M"},
  {78,"N"},
  {79,"O"},
  {80,"P"},
  {81,"Q"},
  {82,"R"},
  {83,"S"},
  {84,"T"},
  {85,"U"},
  {86,"V"},
  {87,"W"},
  {88,"X"},
  {89,"Y"},
  {90,"Z"},
  {58,"Æ"},
  {34,"Ø"},
  {123,"Å"},
  {92,"'"},
  {93,"¨"},
  {94,"&"},
  {95,"?"},
  {96,"½"},
  {97,"a"},
  {98,"b"},
  {99,"c"},
  {100,"d"},
  {101,"e"},
  {102,"f"},
  {103,"g"},
  {104,"h"},
  {105,"i"},
  {106,"j"},
  {107,"k"},
  {108,"l"},
  {109,"m"},
  {110,"n"},
  {111,"o"},
  {112,"p"},
  {113,"q"},
  {114,"r"},
  {115,"s"},
  {116,"t"},
  {117,"u"},
  {118,"v"},
  {119,"w"},
  {120,"x"},
  {121,"y"},
  {122,"z"},
  {59,"æ"},
  {39,"ø"},
  {91,"å"},
  {124,"*"},
  {125,"^"},
  {126,"§"}
};

const Key keys[] = {
  {0x80,"LEFT_CTRL"},
  {0x81,"LEFT_SHIFT"},
  {0x82,"LEFT_ALT"},
  {0x83,"LEFT_GUI"},
  {0x84,"RIGHT_CTRL"},
  {0x85,"RIGHT_SHIFT"},
  {0x86,"RIGHT_ALT"},
  {0x87,"RIGHT_GUI"},
  {0xDA,"UP_ARROW"},
  {0xD9,"DOWN_ARROW"},
  {0xD8,"LEFT_ARROW"},
  {0xD7,"RIGHT_ARROW"},
  {0xB2,"BACKSPACE"},
  {0xB3,"TAB"},
  {0xB0,"RETURN"},
  {0xB1,"ESC"},
  {0xD1,"INSERT"},
  {0xCE,"PRTSC"},
  {0xD4,"DELETE"},
  {0xD3,"PAGE_UP"},
  {0xD6,"PAGE_DOWN"},
  {0xD2,"HOME"},
  {0xD5,"END"},
  {0xC1,"CAPS_LOCK"},
  {0xC2,"F1"},
  {0xC3,"F2"},
  {0xC4,"F3"},
  {0xC5,"F4"},
  {0xC6,"F5"},
  {0xC7,"F6"},
  {0xC8,"F7"},
  {0xC9,"F8"},
  {0xCA,"F9"},
  {0xCB,"F10"},
  {0xCC,"F11"},
  {0xCD,"F12"},
  {0xF0,"F13"},
  {0xF1,"F14"},
  {0xF2,"F15"},
  {0xF3,"F16"},
  {0xF4,"F17"},
  {0xF5,"F18"},
  {0xF6,"F19"},
  {0xF7,"F20"},
  {0xF8,"F21"},
  {0xF9,"F22"},
  {0xFA,"F23"},
  {0xFB,"F24"},
  {0xEA,"NUM_0"},
  {0xE1,"NUM_1"},
  {0xE2,"NUM_2"},
  {0xE3,"NUM_3"},
  {0xE4,"NUM_4"},
  {0xE5,"NUM_5"},
  {0xE6,"NUM_6"},
  {0xE7,"NUM_7"},
  {0xE8,"NUM_8"},
  {0xE9,"NUM_9"},
  {0xDC,"NUM_SLASH"},
  {0xDD,"NUM_ASTERISK"},
  {0xDE,"NUM_MINUS"},
  {0xDF,"NUM_PLUS"},
  {0xE0,"NUM_ENTER"},
  {0xEB,"NUM_PERIOD"}
};

typedef uint8_t MediaKeyReport[2];
typedef struct{
  MediaKeyReport code;
  String str;
} MediaKey;

const MediaKey MediaKeyReports[16]{
  {{1, 0},"MEDIA_NEXT_TRACK"},
  {{2,0},"KEY_MEDIA_PREVIOUS_TRACK"},
  {{4,0},"KEY_MEDIA_STOP"},
  {{8,0},"KEY_MEDIA_PLAY_PAUSE"},
  {{16,0},"KEY_MEDIA_MUTE"},
  {{32,0},"KEY_MEDIA_VOLUME_UP"},
  {{64,0},"KEY_MEDIA_VOLUME_DOWN"},
  {{128,0},"KEY_MEDIA_WWW_HOME"},
  {{0,1},"KEY_MEDIA_LOCAL_MACHINE_BROWSER"},
  {{0,2},"KEY_MEDIA_CALCULATOR"},
  {{0,4},"KEY_MEDIA_WWW_BOOKMARKS"},
  {{0,8},"KEY_MEDIA_WWW_SEARCH"},
  {{0,16},"KEY_MEDIA_WWW_STOP"},
  {{0,32},"KEY_MEDIA_WWW_BACK"},
  {{0,64},"KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION"},
  {{0,128},"KEY_MEDIA_EMAIL_READER"}
};

//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class BleKeyboard : public Print, public BLEServerCallbacks, public BLECharacteristicCallbacks
{
private:
  BLEHIDDevice* hid;
  BLECharacteristic* inputKeyboard;
  BLECharacteristic* outputKeyboard;
  BLECharacteristic* inputMediaKeys;
  BLEAdvertising*    advertising;
  KeyReport          _keyReport;
  MediaKeyReport     _mediaKeyReport;
  std::string        deviceName;
  std::string        deviceManufacturer;
  uint8_t            batteryLevel;
  bool               connected = false;
  uint32_t           _delay_ms = 7;
  void delay_ms(uint64_t ms);

  uint16_t vid       = 0x05ac;
  uint16_t pid       = 0x820a;
  uint16_t version   = 0x0210;

public:
  BleKeyboard(std::string deviceName = "ESP32 Keyboard", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
  void begin(void);
  void end(void);
  void sendReport(KeyReport* keys);
  void sendReport(MediaKeyReport* keys);
  size_t press(uint8_t k);
  size_t press(const MediaKeyReport k);
  size_t release(uint8_t k);
  size_t release(const MediaKeyReport k);
  size_t write(uint8_t c);
  size_t write(const MediaKeyReport c);
  size_t write(const uint8_t *buffer, size_t size);
  void releaseAll(void);
  bool isConnected(void);
  void setBatteryLevel(uint8_t level);
  void setName(std::string deviceName);  
  void setDelay(uint32_t ms);

  void set_vendor_id(uint16_t vid);
  void set_product_id(uint16_t pid);
  void set_version(uint16_t version);
protected:
  virtual void onStarted(BLEServer *pServer) { };
  virtual void onConnect(BLEServer* pServer) override;
  virtual void onDisconnect(BLEServer* pServer) override;
  virtual void onWrite(BLECharacteristic* me) override;

};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_KEYBOARD_H
