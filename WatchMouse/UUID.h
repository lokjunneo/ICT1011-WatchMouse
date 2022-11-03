

#ifndef UUIDH
#define UUIDH
//https://www.bluetooth.com/specifications/assigned-numbers/
//COPY UUID 128 function from tinycircuit 
//https://learn.tinycircuits.com/Communication/Bluetooth-Low-Energy_TinyShield_Tutorial/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
  do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
  }while(0)

//32673b79-e70f-4397-9fb7-7ad4b0d01fbc - some fake custom uuid
//Does not work, windows will hate you and disconnect you
#define COPY_CUSTOM_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x18 ,0x67,0x3b,0x79,0xe7,0x0f,0x43,0x97,0x9f,0xb7,0x7a,0xd4,0xb0,0xd0,0x1f,0xbc)

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

/* The below are object structures referenced from parts of
https://os.mbed.com/users/bowenfeng/code/BLE_HID/
They are mostly in 
HIDServiceBase.h and MouseService.h

Reference materials: https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/

These contains the sizes of the parameters we need
*/

typedef const uint8_t custom_report_map_t[];
typedef const uint8_t * report_t;

uint8_t boot_mouse_report[] = { 0,0,0,0};
uint8_t media_report[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//uint8_t composite_report[] = {0,0,0,0,0,0,0,0,0,0,0,0};

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
    uint8_t type; //apparently, cannot have this
} mouse_report_reference_t;

typedef struct {
    uint8_t ID;
    uint8_t type;
} report_o_reference_t;

typedef struct {
    uint16_t HID_VERSION;
    uint8_t ID;
    uint8_t type;
} HID_information_t;

/*
Report maps
*/

custom_report_map_t REPORT_MAP = {
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x02,         // Mouse
    COLLECTION(1),      0x01,         // Application
    USAGE(1),           0x01,         //  Pointer
    COLLECTION(1),      0x00,         //  Physical
    REPORT_ID(1),       0x01,          //   REPORT_ID 
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
    //INPUT(1),           0x06,         //   Data, Variable, Relative
    INPUT(1),           0x02,         //   Data, Variable, Relative
    END_COLLECTION(0),
    END_COLLECTION(0),
/*
    USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
    USAGE(1),           0x01,          // USAGE (Consumer Control)
    COLLECTION(1),      0x01,          // COLLECTION (Application)
    REPORT_ID(1),       0x02,          //   REPORT_ID 2, media keys id
    USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
    USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0xE0)
    USAGE_MAXIMUM(1),   0xE9,          //   USAGE_MAXIMUM (0xE7)
    LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01,          //   Logical Maximum (1)
    REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
    REPORT_COUNT(1),    0x08,          //   REPORT_COUNT (8)
    USAGE(1),           0xB5,          //   USAGE (Scan Next Track)     ; bit 0: 1
    USAGE(1),           0xB6,          //   USAGE (Scan Previous Track) ; bit 1: 2
    USAGE(1),           0xB7,          //   USAGE (Stop)                ; bit 2: 4
    USAGE(1),           0xCD,          //   USAGE (Play/Pause)          ; bit 3: 8
    USAGE(1),           0xE2,          //   USAGE (Mute)                ; bit 4: 16
    USAGE(1),           0xE9,          //   USAGE (Volume Increment)    ; bit 5: 32
    USAGE(1),           0xEA,          //   USAGE (Volume Decrement)    ; bit 6: 64
    USAGE(2),           0x23, 0x02,    //   Usage (WWW Home)            ; bit 7: 128
    INPUT(1),           0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    END_COLLECTION(0)                  // END_COLLECTION
    */
    // ------------------------------------------------- Media Keys
    USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
    USAGE(1),           0x01,          // USAGE (Consumer Control)
    COLLECTION(1),      0x01,          // COLLECTION (Application)
    REPORT_ID(1),       0x02, //   REPORT_ID (3)
    USAGE_PAGE(1),      0x0C,          //   USAGE_PAGE (Consumer)
    LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01,          //   LOGICAL_MAXIMUM (1)
    REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
    REPORT_COUNT(1),    0x10,          //   REPORT_COUNT (16)
    USAGE(1),           0xB5,          //   USAGE (Scan Next Track)     ; bit 0: 1
    USAGE(1),           0xB6,          //   USAGE (Scan Previous Track) ; bit 1: 2
    USAGE(1),           0xB7,          //   USAGE (Stop)                ; bit 2: 4
    USAGE(1),           0xCD,          //   USAGE (Play/Pause)          ; bit 3: 8
    USAGE(1),           0xE2,          //   USAGE (Mute)                ; bit 4: 16
    USAGE(1),           0xE9,          //   USAGE (Volume Increment)    ; bit 5: 32
    USAGE(1),           0xEA,          //   USAGE (Volume Decrement)    ; bit 6: 64
    USAGE(2),           0x23, 0x02,    //   Usage (WWW Home)            ; bit 7: 128
    USAGE(2),           0x94, 0x01,    //   Usage (My Computer) ; bit 0: 1
    USAGE(2),           0x92, 0x01,    //   Usage (Calculator)  ; bit 1: 2
    USAGE(2),           0x2A, 0x02,    //   Usage (WWW fav)     ; bit 2: 4
    USAGE(2),           0x21, 0x02,    //   Usage (WWW search)  ; bit 3: 8
    USAGE(2),           0x26, 0x02,    //   Usage (WWW stop)    ; bit 4: 16
    USAGE(2),           0x24, 0x02,    //   Usage (WWW back)    ; bit 5: 32
    USAGE(2),           0x83, 0x01,    //   Usage (Media sel)   ; bit 6: 64
    USAGE(2),           0x8A, 0x01,    //   Usage (Mail)        ; bit 7: 128
    INPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    END_COLLECTION(0)                  // END_COLLECTION
    
};

typedef const uint8_t keyboard_report_map_t[];

//Report map befow is from:
//https://github.com/T-vK/ESP32-BLE-Keyboard/blob/63e08b72f2a2c5ffae00950cb17450e4986ee768/BleKeyboard.cpp

// Report IDs:
#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02

typedef uint8_t MediaKeyReport[2];

const MediaKeyReport KEY_MEDIA_NEXT_TRACK = {1, 0};
const MediaKeyReport KEY_MEDIA_PREVIOUS_TRACK = {2, 0};
const MediaKeyReport KEY_MEDIA_STOP = {4, 0};
const MediaKeyReport KEY_MEDIA_PLAY_PAUSE = {8, 0};
const MediaKeyReport KEY_MEDIA_MUTE = {16, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};
const MediaKeyReport KEY_MEDIA_WWW_HOME = {128, 0};
const MediaKeyReport KEY_MEDIA_LOCAL_MACHINE_BROWSER = {0, 1}; // Opens "My Computer" on Windows
const MediaKeyReport KEY_MEDIA_CALCULATOR = {0, 2};
const MediaKeyReport KEY_MEDIA_WWW_BOOKMARKS = {0, 4};
const MediaKeyReport KEY_MEDIA_WWW_SEARCH = {0, 8};
const MediaKeyReport KEY_MEDIA_WWW_STOP = {0, 16};
const MediaKeyReport KEY_MEDIA_WWW_BACK = {0, 32};
const MediaKeyReport KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION = {0, 64}; // Media Selection
const MediaKeyReport KEY_MEDIA_EMAIL_READER = {0, 128};
/*
static const uint8_t _hidReportDescriptor[] = {
  USAGE_PAGE(1),      0x01,          // USAGE_PAGE (Generic Desktop Ctrls)
  USAGE(1),           0x06,          // USAGE (Keyboard)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  // ------------------------------------------------- Keyboard
  REPORT_ID(1),       KEYBOARD_ID,   //   REPORT_ID (1)
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0xE0,          //   USAGE_MINIMUM (0xE0)
  USAGE_MAXIMUM(1),   0xE7,          //   USAGE_MAXIMUM (0xE7)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   Logical Maximum (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x08,          //   REPORT_COUNT (8)
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 1 byte (Reserved)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE (8)
  HIDINPUT(1),        0x01,          //   INPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x05,          //   REPORT_COUNT (5) ; 5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  USAGE_PAGE(1),      0x08,          //   USAGE_PAGE (LEDs)
  USAGE_MINIMUM(1),   0x01,          //   USAGE_MINIMUM (0x01) ; Num Lock
  USAGE_MAXIMUM(1),   0x05,          //   USAGE_MAXIMUM (0x05) ; Kana
  HIDOUTPUT(1),       0x02,          //   OUTPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 3 bits (Padding)
  REPORT_SIZE(1),     0x03,          //   REPORT_SIZE (3)
  HIDOUTPUT(1),       0x01,          //   OUTPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x06,          //   REPORT_COUNT (6) ; 6 bytes (Keys)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE(8)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM(0)
  LOGICAL_MAXIMUM(1), 0x65,          //   LOGICAL_MAXIMUM(0x65) ; 101 keys
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0)
  USAGE_MAXIMUM(1),   0x65,          //   USAGE_MAXIMUM (0x65)
  HIDINPUT(1),        0x00,          //   INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0),                 // END_COLLECTION
  // ------------------------------------------------- Media Keys
  USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
  USAGE(1),           0x01,          // USAGE (Consumer Control)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  REPORT_ID(1),       MEDIA_KEYS_ID, //   REPORT_ID (3)
  USAGE_PAGE(1),      0x0C,          //   USAGE_PAGE (Consumer)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x10,          //   REPORT_COUNT (16)
  USAGE(1),           0xB5,          //   USAGE (Scan Next Track)     ; bit 0: 1
  USAGE(1),           0xB6,          //   USAGE (Scan Previous Track) ; bit 1: 2
  USAGE(1),           0xB7,          //   USAGE (Stop)                ; bit 2: 4
  USAGE(1),           0xCD,          //   USAGE (Play/Pause)          ; bit 3: 8
  USAGE(1),           0xE2,          //   USAGE (Mute)                ; bit 4: 16
  USAGE(1),           0xE9,          //   USAGE (Volume Increment)    ; bit 5: 32
  USAGE(1),           0xEA,          //   USAGE (Volume Decrement)    ; bit 6: 64
  USAGE(2),           0x23, 0x02,    //   Usage (WWW Home)            ; bit 7: 128
  USAGE(2),           0x94, 0x01,    //   Usage (My Computer) ; bit 0: 1
  USAGE(2),           0x92, 0x01,    //   Usage (Calculator)  ; bit 1: 2
  USAGE(2),           0x2A, 0x02,    //   Usage (WWW fav)     ; bit 2: 4
  USAGE(2),           0x21, 0x02,    //   Usage (WWW search)  ; bit 3: 8
  USAGE(2),           0x26, 0x02,    //   Usage (WWW stop)    ; bit 4: 16
  USAGE(2),           0x24, 0x02,    //   Usage (WWW back)    ; bit 5: 32
  USAGE(2),           0x83, 0x01,    //   Usage (Media sel)   ; bit 6: 64
  USAGE(2),           0x8A, 0x01,    //   Usage (Mail)        ; bit 7: 128
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0)                  // END_COLLECTION
};

/*
custom_report_map_t KEYBOARD_REPORT_MAP = {
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
