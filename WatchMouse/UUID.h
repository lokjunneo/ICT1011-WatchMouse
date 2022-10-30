

#ifndef UUIDH
#define UUIDH
//https://www.bluetooth.com/specifications/assigned-numbers/

#define COPY_UUID_16(uuid_struct, uuid_1, uuid_0) \
  do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1;\
  }while(0)
#define COPY_HID_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x18,0x12) 

#define COPY_HID_CP_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x2A,0x4C) 

#define COPY_CTS_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x18,0x05) 

#define COPY_CTS_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x18,0x05) 

//0x2A4E Protocol mode
#define COPY_PM_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x2A,0x4E)

//0x2A4A HID INFORMATION
#define COPY_HID_INFO_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x2A,0x4A)

//mouse characteristics: need report map, (boot mouse) input report, and dont forget a ticker to decide how fast to spam

#define COPY_REPORT_MAP_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x2A,0x4B)

//BOOT MOUSE INPUT REPORT
#define COPY_BOOT_MOUSE_INPUT_REPORT_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x2A,0x33)

//REPORT REFERENCE
#define COPY_REPORT_REFERENCE_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x29,0x08)

#define COPY_REPORT_CHAR_UUID(uuid_struct) COPY_UUID_16(uuid_struct,0x2A,0x4D)

//Need to shrink down interval to detect it
#define  ADV_INTERVAL_MIN_MS  50
#define  ADV_INTERVAL_MAX_MS  100

//Assigning intervals will make it completely undiscoverable for some reason
#define  CONN_INTERVAL_MIN_MS 0
#define  CONN_INTERVAL_MAX_MS 0

/*
BLEConnection structure is copied from:
https://learn.tinycircuits.com/Kits/TinyScreen-Smart-Watch-Kit_Tutorial/ ios example, from the BLETypes.h file

Only the struct is copied from the example
*/
typedef struct _BLEConnection {
  uint16_t handle;
  uint8_t isAdvertising;
  uint8_t isConnected;
  uint8_t isBonded;
  uint8_t connectedAddr[6];
  void (* onConnect)();
  void (* onBond)();
  void (* onDisconnect)();
  void * firstChar;
} BLEConnection;

/*
  report[0] == buttons
  report[1] == speed[0]
  report[2] == speed[1]
  report[3] == speed[2]
 */

/* The below are object structures copied from parts of
https://os.mbed.com/users/bowenfeng/code/BLE_HID/
They are mostly in 
HIDServiceBase.h and MouseService.h

These contains the sizes of the parameters we need
*/

typedef const uint8_t report_map_t[];
typedef const uint8_t * report_t;

uint8_t report[] = { 0,0,0,0};

typedef struct {
    uint32_t length;
    uint8_t data[MAX_HID_REPORT_SIZE];
} HID_REPORT;

enum ReportType {
    INPUT_REPORT    = 0x1,
    OUTPUT_REPORT   = 0x2,
    FEATURE_REPORT  = 0x3,
};

enum ProtocolMode {
    BOOT_PROTOCOL   = 0x0,
    REPORT_PROTOCOL = 0x1,
};

typedef struct {
    uint8_t ID;
    //uint8_t type;
} report_reference_t;

typedef struct {
    uint8_t ID;
    uint8_t type;
} report_o_reference_t;

typedef struct {
    uint16_t HID_VERSION;
    uint8_t ID;
    uint8_t type;
} HID_information_t;

report_map_t MOUSE_REPORT_MAP = {
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x02,         // Mouse
    COLLECTION(1),      0x01,         // Application
    USAGE(1),           0x01,         //  Pointer
    COLLECTION(1),      0x00,         //  Physical
    USAGE_PAGE(1),      0x09,         //   Buttons
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x03,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x03,         //   3 bits (Buttons)
    REPORT_SIZE(1),     0x01,
    INPUT(1),           0x02,         //   Data, Variable, Absolute
    REPORT_COUNT(1),    0x01,         //   5 bits (Padding)
    REPORT_SIZE(1),     0x05,
    INPUT(1),           0x01,         //   Constant
    USAGE_PAGE(1),      0x01,         //   Generic Desktop
    USAGE(1),           0x30,         //   X
    USAGE(1),           0x31,         //   Y
    USAGE(1),           0x38,         //   Wheel
    LOGICAL_MINIMUM(1), 0x81,         //   -127
    LOGICAL_MAXIMUM(1), 0x7f,         //   127
    REPORT_SIZE(1),     0x08,         //   Three bytes
    REPORT_COUNT(1),    0x03,
    INPUT(1),           0x06,         //   Data, Variable, Relative
    END_COLLECTION(0),
    END_COLLECTION(0),
};
/*
report_map_t MOUSE_REPORT_MAP = {
    USAGE_PAGE(1), 0x01, // Generic Desktop Ctrls
    USAGE(1), 0x06,      // Keyboard
    COLLECTION(1), 0x01, // Application
    USAGE_PAGE(1), 0x07, //   Kbrd/Keypad
    USAGE_MINIMUM(1), 0xE0,
    USAGE_MAXIMUM(1), 0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1), 0x01, //   1 byte (Modifier)
    REPORT_COUNT(1), 0x08,
    INPUT(1), 0x02,        //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1), 0x01, //   1 byte (Reserved)
    REPORT_SIZE(1), 0x08,
    INPUT(1), 0x01,        //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1), 0x05, //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
    REPORT_SIZE(1), 0x01,
    USAGE_PAGE(1), 0x08,    //   LEDs
    USAGE_MINIMUM(1), 0x01, //   Num Lock
    USAGE_MAXIMUM(1), 0x05, //   Kana
    OUTPUT(1), 0x02,        //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1), 0x01,  //   3 bits (Padding)
    REPORT_SIZE(1), 0x03,
    OUTPUT(1), 0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1), 0x06, //   6 bytes (Keys)
    REPORT_SIZE(1), 0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65, //   101 keys
    USAGE_PAGE(1), 0x07,      //   Kbrd/Keypad
    USAGE_MINIMUM(1), 0x00,
    USAGE_MAXIMUM(1), 0x65,
    INPUT(1), 0x00, //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    END_COLLECTION(0),
};*/
 

#endif