//Need to credit TinyCircuit for providing example codes (parts of the example code were used in here)
//Need to credit MBed.org for USBHID_Types.h
//Need to include method to disable debug mode


//the connection we will be using
BLEConnection * primaryConn = NULL;

//We store the service handle for human interface device here, and HID Control Point Characteristic Handle
uint16_t HIDServHandle, HIDCPCharHandle, HIDRPCharHandle, HIDBMIRCharHandle, HIDBMIRDescCharHandle, HIDInfCharHandle, HIDProtocolCharHandle, 
        HIDReportCharHandle, HIDReportDescCharHandle, HIDMediaReportOCharHandle, HIDMediaReportODescCharHandle;

//const long BLE_timer;

void BLE_Process() {
  HCI_Process();
}

/**
 *Initialize HCI_Init(), aci_gatt_init(), gap_init_IDB04A1
 *The BlueNRG library we use (ported over by STBLE.h) provides all the information, as well as example code we need
 *@param localName local name of our device. Just give it a name
 */
uint8_t BLE_Init(char * localName){
  //tBleStatus is uint8_t
  uint8_t ret;
  HCI_Init();

  //from arduino_bluenrg files, need these for BLE to work
  BNRG_SPI_Init();
  BlueNRG_RST();

  //BLE_STATUS_SUCCESS is 0
  //Generic ATTribute Profile: protocol style of exchange data
  ret = aci_gatt_init();
  if (ret) { 
    SerialMonitorInterface.println("INIT FAIL: ACI_GATT \n");
    //can implement possible reboot
  } 

  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  //1st paramater sets us as the peripheral, the rest are outputs from the function
  //IDB05A1 get set local name, 04A1 cannot
  aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1,0,strlen(localName),&service_handle,&dev_name_char_handle,&appearance_char_handle);
  if(ret){
    SerialMonitorInterface.println("GAP_Init failed.\n");
  }  
  ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, strlen(localName), (uint8_t *)localName);        
  if(ret){
    SerialMonitorInterface.println("aci_gatt_update_char_value failed.\n");
  } 
    
  //referenced from TinyCircuit code
  //https://www.st.com/resource/en/user_manual/um1865-the-bluenrgms-bluetooth-le-stack-application-command-interface-aci-stmicroelectronics.pdf
  //EN_High_Power 1 or 0: set high power on or off
  //PA_Level 0 to 7: Set the PA level value
  ret = aci_hal_set_tx_power_level(1, 7);
  if (ret == ERR_INVALID_HCI_CMD_PARAMS){
  }

  SerialMonitorInterface.println("Initialization complete");
}

uint8_t BLE_Conn_Setup( BLEConnection* conn,void (*connect)(), void (*disconnect)()){
  SerialMonitorInterface.println("setting up primaryConn");
  //set up primaryConn object, with onConnect, onBond and onDisconnect functions
  primaryConn = conn;
  primaryConn->handle = NULL;
  primaryConn->onConnect = connect;
  primaryConn->onBond = NULL;
  primaryConn->onDisconnect = disconnect;
  primaryConn->isAdvertising = 0;
  primaryConn->isConnected = 0;
  primaryConn->isBonded = 0;
  primaryConn->firstChar = NULL;
  SerialMonitorInterface.println("setting up primaryConn: complete");

}

//Add the HID Service and characteristics
//Lots of references, including official bluetooth manual and stm32's library documentation, and some part of nordic's one
//Reference projects are as listed (aka we learn from this projects, NOT COPY)
//https://github.com/jpbrucker/BLE_HID/blob/master/doc/HIDService.md
//https://github.com/jpbrucker/BLE_HID/blob/master/doc/HID.md#mouse
//https://github.com/PixlRainbow/TinyCircuits-TinyShield-BLE-ASD2116/blob/a4f926d72a8e6264ea06030a27de2e3c9c63afe9/examples/STBLE/examples/BLEKeyboard/HID.ino
uint8_t init_HID_service() {
  tBleStatus ret;

  const mouse_report_reference_t mouseInputDesc  = { 0x01,INPUT_REPORT};// INPUT_REPORT};//, INPUT_REPORT };
  const report_o_reference_t reportODesc  = { 0x01, INPUT_REPORT};

  //const mouse_report_reference_t mediaInputDesc  = { 0x02,INPUT_REPORT};// INPUT_REPORT};//, INPUT_REPORT };
  const report_o_reference_t mediaReportODesc  = { 0x02, INPUT_REPORT};
  
  const static HID_information_t info = { HID_VERSION_1_11, 0x00, 0x03 };
  const ProtocolMode mode = REPORT_PROTOCOL;
  //const report_t report = {0x00, INPUT_REPORT};

  unsigned int errcode = 0;
  
  uint8_t uuid128[16];
  uint8_t uuid16[2];

  COPY_HID_UUID(uuid16);
  ret = aci_gatt_add_serv(UUID_TYPE_16,  uuid16, PRIMARY_SERVICE, 50, &HIDServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  //aci_gatt_add_char means that you will be providing the service on the tinycircuit (e.g. mouse control)
  //HID Control Point
  COPY_HID_CP_UUID(uuid16);
  
  ret =  aci_gatt_add_char(HIDServHandle, UUID_TYPE_16, uuid16, 20, CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 1, &HIDCPCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  COPY_REPORT_MAP_UUID(uuid16);
  //MOUSE REPORT MAP..20?
  ret =  aci_gatt_add_char(HIDServHandle, UUID_TYPE_16, uuid16, sizeof(REPORT_MAP), CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
                           16, CHAR_VALUE_LEN_VARIABLE, &HIDRPCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  ret = aci_gatt_update_char_value(HIDServHandle, HIDRPCharHandle, 0, sizeof(REPORT_MAP), REPORT_MAP);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  COPY_BOOT_MOUSE_INPUT_REPORT_UUID(uuid16);
  //BOOT MOUSE INPUT REPORT
  ret = aci_gatt_add_char(HIDServHandle, UUID_TYPE_16, uuid16, sizeof(boot_mouse_report), (CHAR_PROP_READ | CHAR_PROP_NOTIFY), ATTR_PERMISSION_NONE, 
                           GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, CHAR_VALUE_LEN_VARIABLE, &HIDBMIRCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  COPY_REPORT_REFERENCE_UUID(uuid16);

  ret = aci_gatt_add_char_desc(HIDServHandle, HIDBMIRCharHandle, UUID_TYPE_16, uuid16,
                              sizeof(mouse_report_reference_t), sizeof(mouse_report_reference_t), &mouseInputDesc,
                              ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY, GATT_NOTIFY_ATTRIBUTE_WRITE,
                              10, CHAR_VALUE_LEN_CONSTANT, &HIDBMIRDescCharHandle);

  SerialMonitorInterface.println(sizeof(mouse_report_reference_t));
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  //Protocol mode characteristics

  COPY_PM_UUID(uuid16);

  ret = aci_gatt_add_char(HIDServHandle, UUID_TYPE_16, uuid16, 1, (CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RESP), ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           10, CHAR_VALUE_LEN_CONSTANT, &HIDProtocolCharHandle);

  if (ret != BLE_STATUS_SUCCESS) goto fail;

  //Can't sizeof enum
  ret = aci_gatt_update_char_value(HIDServHandle, HIDProtocolCharHandle, 0, 1, &mode);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  //HID Information
  //need add also
  COPY_HID_INFO_UUID(uuid16);

  ret = aci_gatt_add_char(HIDServHandle, UUID_TYPE_16, uuid16, sizeof(HID_information_t),
                           (CHAR_PROP_READ), ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
                           10, CHAR_VALUE_LEN_VARIABLE, &HIDInfCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  ret = aci_gatt_update_char_value(HIDServHandle, HIDInfCharHandle, 0, sizeof(HID_information_t), &info);
  if (ret != BLE_STATUS_SUCCESS) goto fail;


  SerialMonitorInterface.println("HID success!.\n");
  //REPORT O
  COPY_REPORT_CHAR_UUID(uuid16);
  ret = aci_gatt_add_char(HIDServHandle, UUID_TYPE_16, uuid16, sizeof(boot_mouse_report),
                           (CHAR_PROP_READ | CHAR_PROP_NOTIFY), ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, CHAR_VALUE_LEN_VARIABLE, &HIDReportCharHandle);
  
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  COPY_REPORT_REFERENCE_UUID(uuid16);

  ret = aci_gatt_add_char_desc(HIDServHandle, HIDReportCharHandle, UUID_TYPE_16, uuid16,
                              sizeof(report_o_reference_t), sizeof(report_o_reference_t), &reportODesc,
                              ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY, GATT_NOTIFY_ATTRIBUTE_WRITE,
                              10, CHAR_VALUE_LEN_CONSTANT, &HIDReportDescCharHandle);

  if (ret != BLE_STATUS_SUCCESS) goto fail;

  //MEDIA REPORT O
  COPY_REPORT_CHAR_UUID(uuid16);
  ret = aci_gatt_add_char(HIDServHandle, UUID_TYPE_16, uuid16, sizeof(media_report),
                           (CHAR_PROP_READ | CHAR_PROP_NOTIFY), ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, CHAR_VALUE_LEN_VARIABLE, &HIDMediaReportOCharHandle);
  
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  SerialMonitorInterface.println("Update_char_value SUCCESS.\n");

  COPY_REPORT_REFERENCE_UUID(uuid16);

  ret = aci_gatt_add_char_desc(HIDServHandle, HIDReportCharHandle, UUID_TYPE_16, uuid16,
                              sizeof(report_o_reference_t), sizeof(report_o_reference_t), &mediaReportODesc,
                              ATTR_PERMISSION_NONE, ATTR_ACCESS_READ_ONLY, GATT_NOTIFY_ATTRIBUTE_WRITE,
                              10, CHAR_VALUE_LEN_CONSTANT, &HIDMediaReportODescCharHandle);

  SerialMonitorInterface.println("Added char desc.\n"); 

  if (ret != BLE_STATUS_SUCCESS) goto fail;



  SerialMonitorInterface.println("BLE_INIT success!.\n");

  return BLE_STATUS_SUCCESS;

fail:
  SerialMonitorInterface.println("Error while adding UART service.\n");
  return BLE_STATUS_ERROR ;

}

//This is from example code provided in the documentation of the BlueNRG library, for aci_gap_set_discoverable
uint8_t setDiscoverable(){
  uint8_t ret;
  
  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME, 'W', 'a', 't', 'c', 'h', 'M', 'o', 'u', 's', 'e'};
  SerialMonitorInterface.println(local_name);
  hci_le_set_scan_resp_data(0, NULL);
  
    ret = aci_gap_set_discoverable(ADV_IND, (ADV_INTERVAL_MIN_MS*1000)/625,
                                          (ADV_INTERVAL_MAX_MS*1000)/625,
                                          STATIC_RANDOM_ADDR, NO_WHITE_LIST_USE,
                                          sizeof(local_name), local_name,
                                          0, NULL,
                                          (CONN_INTERVAL_MIN_MS*1000)/1250,
                                          (CONN_INTERVAL_MAX_MS*1000)/1250);

  if (ret != BLE_STATUS_SUCCESS)
    SerialMonitorInterface.println("Unable to set discoverable");     

  SerialMonitorInterface.println("Discoverable");
}

//https://learn.tinycircuits.com/Kits/TinyScreen-Smart-Watch-Kit_Tutorial/
//Ios Example
uint8_t init_Security(void (*bond)()){
  
  //Put no input no output, so these capabilities will not be demanded
  uint8_t ret = aci_gap_set_io_capability(IO_CAP_NO_INPUT_NO_OUTPUT);
  if (ret) {
    SerialMonitorInterface.println("aci_gap_set_io_capability failed.\n");
  }
  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_NOT_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL,
                                     7,  /* Min. encryption key size */
                                     16, /* Max encryption key size */
                                     DONOT_USE_FIXED_PIN_FOR_PAIRING,/* no fixed pin */
                                     0, /* fixed pin not used */
                                     BONDING );
  if (ret) {
    SerialMonitorInterface.println("aci_gap_set_auth_requirement failed.\n");
  }
  primaryConn->onBond = bond;
}

//https://learn.tinycircuits.com/Kits/TinyScreen-Smart-Watch-Kit_Tutorial/
//Ios Example
uint8_t requestSecurity() {
  SerialMonitorInterface.println("Requesting Security");
  tBleStatus ret = aci_gap_slave_security_request(primaryConn->handle, BONDING, MITM_PROTECTION_NOT_REQUIRED);
  if (ret != BLE_STATUS_SUCCESS)
    SerialMonitorInterface.println("Slave security request error");
  return ret;
}


void HCI_Event_CB(void *pckt){

  hci_uart_pckt *hci_pckt = (hci_uart_pckt *)pckt;
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
  
  switch (event_pckt->evt) {
    case EVT_DISCONN_COMPLETE:
      {
        SerialMonitorInterface.println("Disconnected");
        BLE_Disconnected();
      }
      break;
    case EVT_LE_META_EVENT:
      {
        evt_le_meta_event *evt = (evt_le_meta_event *)event_pckt->data;
        switch (evt->subevent) {
          case EVT_LE_CONN_COMPLETE:
            {
              SerialMonitorInterface.println("Connected");
              evt_le_connection_complete *cc = (evt_le_connection_complete *)evt->data;
              primaryConn->handle = cc->handle;
              primaryConn->onConnect();
            }
            break;
        }
      }
      break;

      case EVT_VENDOR:
        {
        evt_blue_aci *blue_evt = (evt_blue_aci *)event_pckt->data;
        switch (blue_evt->ecode) {
          //Should timeout by itself
          case EVT_BLUE_GAP_PAIRING_CMPLT:
          {
            evt_gap_pairing_cmplt *pc = (evt_gap_pairing_cmplt *)blue_evt->data;
            if (pc->status == 0) {
              primaryConn->isBonded = true;
              SerialMonitorInterface.println("Pairing success");
              if (primaryConn->onBond) {
                primaryConn->onBond();
              }
            }
            else {
              SerialMonitorInterface.println("Pairing failed, timeout");
              hci_disconnect(primaryConn->handle, BLE_STATUS_TIMEOUT);
              //primaryConn->onDisconnect();
            }
          }
          break;
          //Sometimes when bonding, might fail
          //So need to rebond
          case EVT_BLUE_GAP_BOND_LOST:
            {
              SerialMonitorInterface.println("bond lost, allowing rebond\n");
              aci_gap_allow_rebond_IDB05A1(primaryConn->handle);
            }
            break;
        }
      }
      break;
    }

}

//instead of just calling the primaryConn->onDisconnect, we need to set the parameters of primaryConn first
void BLE_Disconnected(){
  primaryConn->handle = NULL;
  primaryConn->isAdvertising = false;
  primaryConn->isConnected = false;
  primaryConn->isBonded = false;
  if (primaryConn->onDisconnect) {
    primaryConn->onDisconnect();
  }
  ble_connection_state = false;
}

void update_mouse(uint8_t input_report[]){
  uint8_t uuid16[2], ret;
  uint8_t updated_report[] = {0,0,0,0};
  
  updated_report[0] = input_report[0];
  updated_report[1] = input_report[1];
  updated_report[2] = input_report[2];
  updated_report[3] = input_report[3];

  COPY_BOOT_MOUSE_INPUT_REPORT_UUID(uuid16);
  //BOOT MOUSE INPUT REPORT
  ret = aci_gatt_update_char_value(HIDServHandle, HIDBMIRCharHandle,0,sizeof(boot_mouse_report),&updated_report);

  if (ret != BLE_STATUS_SUCCESS) {
    SerialMonitorInterface.println("Failed to update Boot Mouse Input Report");
  };
                           
  COPY_REPORT_CHAR_UUID(uuid16);
  //REPORT O
  ret = aci_gatt_update_char_value(HIDServHandle, HIDReportCharHandle,0,sizeof(boot_mouse_report),&updated_report);

  if (ret != BLE_STATUS_SUCCESS) {
    SerialMonitorInterface.println("Failed to update Report characteristics");
  };
  SerialMonitorInterface.println("Mouse update process: complete");
}

void update_media(uint8_t input_report[]){
  
  uint8_t uuid16[2], ret;
  uint8_t updated_report[] = {0,0,0,0,0,0,0,0};
  
  updated_report[0] = input_report[0];
  updated_report[1] = input_report[1];
  updated_report[2] = input_report[2];
  updated_report[3] = input_report[3];
  updated_report[4] = input_report[4];
  updated_report[5] = input_report[5];
  updated_report[6] = input_report[6];
  updated_report[7] = input_report[7];
  
  //Media REPORT O
  ret = aci_gatt_update_char_value(HIDServHandle, HIDMediaReportOCharHandle,0,sizeof(media_report),&updated_report);

  if (ret != BLE_STATUS_SUCCESS) {
    SerialMonitorInterface.println("Failed to update Media Report O characteristics");
  };
  SerialMonitorInterface.println("Media update process: complete");
}

