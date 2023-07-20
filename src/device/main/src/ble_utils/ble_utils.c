/*******************************************************************************
 * @file        ble_utils.c
 * @author      Leonardo Acha Boiano
 * @date        18 Jul 2023
 * 
 * @note        This code is written in C and is used on an ESP32-CAM development board.
 *              Based on https://github.com/Zeni241/ESP32-NimbleBLE-For-Dummies.git.
 *
 *******************************************************************************/
#if ENABLE_BLE
#include "../ble_utils/ble_utils.h"

esp_err_t ret;
static uint8_t own_addr_type;
uint16_t notification_handle;
uint16_t conn_handle;
bool notify_state; // When client subscribe to notifications, the value is set to 1.Check this value before sending notifictions.
char notification[MAX_NOTIFICATION_LEN];

//**************************Define UUIDs***************************************/
// 860480f2-d1b2-428a-ab2c-cd0d9087a84f
static const ble_uuid128_t gatt_svr_svc_uuid =
    BLE_UUID128_INIT(0x4f, 0xa8, 0x87, 0x90, 0x0d, 0xcd, 0x2c, 0xab, 0x8a, 0x42, 0xb2, 0xd1, 0xf2, 0x80, 0x04, 0x86);

// ae15eabd-a08b-4f2d-9f39-f80e6f8efe5b
static const ble_uuid128_t gatt_svr_chr_uuid =
    BLE_UUID128_INIT(0x5b, 0xfe, 0x8e, 0x6f, 0x0e, 0xf8, 0x39, 0x9f, 0x2d, 0x4f, 0x8b, 0xa0, 0xbd, 0xea, 0x15, 0xae);

// Variables used in service and characteristic declaration
char characteristic_value[50] = "I am characteristic value"; // When client read characteristic, he get this value. You can also set this value in your code.
char characteristic_received_value[500];                     // When client write to characteristic , he set value of this. You can read it in code.

uint16_t min_length = 1;   // minimum length the client can write to a characterstic
uint16_t max_length = 700; // maximum length the client can write to a characterstic

static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {

        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &gatt_svr_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){{
        .uuid = &gatt_svr_chr_uuid.u,     // UUID as given above
        .access_cb = gatt_svr_chr_access, // Callback function. When ever this characrstic will be accessed by user, this function will execute
        .val_handle = &notification_handle,
        .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY, // flags set permissions. In this case User can read this characterstic, can write to it,and get notified. 
        },
        {
            0, /* No more characteristics in this service. This is necessary */
        }},
    },

    {
        0, /* No more services. This is necessary */
    },
};

void ble_printf(const char *format, ...)
{
    // Clear the notification buffer
    memset(notification, 0, MAX_NOTIFICATION_LEN);

    va_list args;
    va_start(args, format);
    vsnprintf(notification, MAX_NOTIFICATION_LEN, format, args);
    va_end(args);
}


static int gatt_svr_chr_access(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt,
                               void *arg)  // Callback function. When ever characrstic will be accessed by user, this function will execute
{

  int rc;

  switch (ctxt->op)
  {
  case BLE_GATT_ACCESS_OP_READ_CHR: // In case user accessed this characterstic to read its value, bellow lines will execute
    rc = os_mbuf_append(ctxt->om, &characteristic_value,
                        sizeof characteristic_value);
    return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;

  case BLE_GATT_ACCESS_OP_WRITE_CHR: // In case user accessed this characterstic to write, bellow lines will executed.
    rc = gatt_svr_chr_write(ctxt->om, min_length, max_length, &characteristic_received_value, NULL); // Function "gatt_svr_chr_write" will fire.
    ESP_LOGI(BLE_TAG,"Received=%s\n", characteristic_received_value);  // Print the received value
    //! Use received value in you code. For example
    char stp[]="stop";
int x=strcmp(characteristic_received_value,stp);
    if(x==0){
      stopBLE();
    }

    return rc;
  default:
    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
  }
}

void sendNotification() //!Use this function to send notification once (after setting value of variable "notification")
{
  int rc;
  struct os_mbuf *om;

  if (notify_state) // This value is checked so that we don't send notifications if user has not subscribed to our notification handle.
  {
    om = ble_hs_mbuf_from_flat(notification, sizeof(notification)); // Value of variable "notification" will be sent as notification.

    rc = ble_gattc_notify_custom(conn_handle, notification_handle, om);

    if (rc != 0)
    {
      ESP_LOGI(BLE_TAG,"\n error notifying; rc\n");
    }
  }
  else
  {
    ESP_LOGI(BLE_TAG,"user not subscribed to notifications.\n");
  }
}

void startBLE() //! Call this function to start BLE
{

  //! Below is the sequence of APIs to be called to init/enable NimBLE host and ESP controller:
  ESP_LOGI(BLE_TAG,"\n Staring BLE \n");
  int rc;

  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(nimble_port_init());

  /* Initialize the NimBLE host configuration. */
  ble_hs_cfg.reset_cb = bleprph_on_reset;
  ble_hs_cfg.sync_cb = bleprph_on_sync;
  ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
  ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
  ble_hs_cfg.sm_io_cap = BLE_HS_IO_NO_INPUT_OUTPUT;

#ifdef CONFIG_EXAMPLE_BONDING
  ble_hs_cfg.sm_bonding = 1;
#endif
#ifdef CONFIG_EXAMPLE_MITM
  ble_hs_cfg.sm_mitm = 1;
#endif
#ifdef CONFIG_EXAMPLE_USE_SC
  ble_hs_cfg.sm_sc = 1;
#else
  ble_hs_cfg.sm_sc = 0;
#endif
#ifdef CONFIG_EXAMPLE_BONDING
  ble_hs_cfg.sm_our_key_dist = 1;
  ble_hs_cfg.sm_their_key_dist = 1;
#endif

  rc = gatt_svr_init();
  assert(rc == 0);

  /* Set the default device name. */
  rc = ble_svc_gap_device_name_set("SMK-ble"); // Set the name of this device
  assert(rc == 0);

  /* XXX Need to have template for store */

  nimble_port_freertos_init(bleprph_host_task);
  ESP_LOGI(BLE_TAG,"characteristic_value at end of startBLE=%s\n", characteristic_value);

}

void stopBLE() //! Call this function to stop BLE
{
  //! Below is the sequence of APIs to be called to disable/deinit NimBLE host and ESP controller:
  ESP_LOGI(BLE_TAG,"\n Stoping BLE\n");

  int ret = nimble_port_stop();
  if (ret == 0)
  {
    nimble_port_deinit();

    ret = nimble_port_deinit();
    if (ret != ESP_OK)
    {
      ESP_LOGE(BLE_TAG, "nimble_port_deinit() failed with error: %d", ret);
    }
  }
}

static int gatt_svr_chr_write(struct os_mbuf *om, uint16_t min_len, uint16_t max_len, void *dst, uint16_t *len)
{
  uint16_t om_len;
  int rc;

  om_len = OS_MBUF_PKTLEN(om);
  if (om_len < min_len || om_len > max_len)
  {
    return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
  }

  rc = ble_hs_mbuf_to_flat(om, dst, max_len, len);
  if (rc != 0)
  {
    return BLE_ATT_ERR_UNLIKELY;
  }

  return 0;
}

void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
  char buf[BLE_UUID_STR_LEN];

  switch (ctxt->op)
  {
  case BLE_GATT_REGISTER_OP_SVC:
    MODLOG_DFLT(DEBUG, "registered service %s with handle=%d\n",
                ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                ctxt->svc.handle);
    break;

  case BLE_GATT_REGISTER_OP_CHR:
    MODLOG_DFLT(DEBUG, "registering characteristic %s with "
                       "def_handle=%d val_handle=%d\n",
                ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                ctxt->chr.def_handle,
                ctxt->chr.val_handle);
    break;

  case BLE_GATT_REGISTER_OP_DSC:
    MODLOG_DFLT(DEBUG, "registering descriptor %s with handle=%d\n",
                ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                ctxt->dsc.handle);
    break;

  default:
    assert(0);
    break;
  }
}

int gatt_svr_init(void)
{
  int rc;

  ble_svc_gap_init();
  ble_svc_gatt_init();

  rc = ble_gatts_count_cfg(gatt_svr_svcs);
  if (rc != 0)
  {
    return rc;
  }

  rc = ble_gatts_add_svcs(gatt_svr_svcs);
  if (rc != 0)
  {
    return rc;
  }

  return 0;
}

static void
bleprph_print_conn_desc(struct ble_gap_conn_desc *desc)
{
  MODLOG_DFLT(INFO, "handle=%d our_ota_addr_type=%d our_ota_addr=",
              desc->conn_handle, desc->our_ota_addr.type);
  print_addr(desc->our_ota_addr.val);
  MODLOG_DFLT(INFO, " our_id_addr_type=%d our_id_addr=",
              desc->our_id_addr.type);
  print_addr(desc->our_id_addr.val);
  MODLOG_DFLT(INFO, " peer_ota_addr_type=%d peer_ota_addr=",
              desc->peer_ota_addr.type);
  print_addr(desc->peer_ota_addr.val);
  MODLOG_DFLT(INFO, " peer_id_addr_type=%d peer_id_addr=",
              desc->peer_id_addr.type);
  print_addr(desc->peer_id_addr.val);
  MODLOG_DFLT(INFO, " conn_itvl=%d conn_latency=%d supervision_timeout=%d "
                    "encrypted=%d authenticated=%d bonded=%d\n",
              desc->conn_itvl, desc->conn_latency,
              desc->supervision_timeout,
              desc->sec_state.encrypted,
              desc->sec_state.authenticated,
              desc->sec_state.bonded);
}

static void
bleprph_advertise(void)
{
  struct ble_gap_adv_params adv_params;
  struct ble_hs_adv_fields fields;
  const char *name;
  int rc;

  /**
   *  Set the advertisement data included in our advertisements:
   *     o Flags (indicates advertisement type and other general info).
   *     o Advertising tx power.
   *     o Device name.
   *     o 16-bit service UUIDs (alert notifications).
   */

  memset(&fields, 0, sizeof fields);

  /* Advertise two flags:
   *     o Discoverability in forthcoming advertisement (general)
   *     o BLE-only (BR/EDR unsupported).
   */
  fields.flags = BLE_HS_ADV_F_DISC_GEN |
                 BLE_HS_ADV_F_BREDR_UNSUP;

  /* Indicate that the TX power level field should be included; have the
   * stack fill this value automatically.  This is done by assigning the
   * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
   */
  fields.tx_pwr_lvl_is_present = 1;
  fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

  name = ble_svc_gap_device_name();
  fields.name = (uint8_t *)name;
  fields.name_len = strlen(name);
  fields.name_is_complete = 1;

  fields.uuids16 = (ble_uuid16_t[]){
      BLE_UUID16_INIT(GATT_SVR_SVC_ALERT_UUID)};
  fields.num_uuids16 = 1;
  fields.uuids16_is_complete = 1;

  rc = ble_gap_adv_set_fields(&fields);
  if (rc != 0)
  {
    MODLOG_DFLT(ERROR, "error setting advertisement data; rc=%d\n", rc);
    return;
  }

  /* Begin advertising. */
  memset(&adv_params, 0, sizeof adv_params);
  adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
  rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER,
                         &adv_params, bleprph_gap_event, NULL);
  if (rc != 0)
  {
    MODLOG_DFLT(ERROR, "error enabling advertisement; rc=%d\n", rc);
    return;
  }
}

/**
 * The nimble host executes this callback when a GAP event occurs.  The
 * application associates a GAP event callback with each connection that forms.
 * bleprph uses the same callback for all connections.
 *
 * @param event                 The type of event being signalled.
 * @param ctxt                  Various information pertaining to the event.
 * @param arg                   Application-specified argument; unused by
 *                                  bleprph.
 *
 * @return                      0 if the application successfully handled the
 *                                  event; nonzero on failure.  The semantics
 *                                  of the return code is specific to the
 *                                  particular GAP event being signalled.
 */
static int
bleprph_gap_event(struct ble_gap_event *event, void *arg)
{
  struct ble_gap_conn_desc desc;
  int rc;

  switch (event->type)
  {
  case BLE_GAP_EVENT_CONNECT:
    /* A new connection was established or a connection attempt failed. */
    MODLOG_DFLT(INFO, "connection %s; status=%d ",
                event->connect.status == 0 ? "established" : "failed",
                event->connect.status);
    if (event->connect.status == 0)
    {
      rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
      assert(rc == 0);
      bleprph_print_conn_desc(&desc);
    }
    MODLOG_DFLT(INFO, "\n");

    if (event->connect.status != 0)
    {
      /* Connection failed; resume advertising. */
      bleprph_advertise();
    }
    conn_handle = event->connect.conn_handle;
    return 0;

  case BLE_GAP_EVENT_DISCONNECT:
    MODLOG_DFLT(INFO, "disconnect; reason=%d ", event->disconnect.reason);
    bleprph_print_conn_desc(&event->disconnect.conn);
    MODLOG_DFLT(INFO, "\n");

    /* Connection terminated; resume advertising. */
    bleprph_advertise();
    return 0;

  case BLE_GAP_EVENT_CONN_UPDATE:
    /* The central has updated the connection parameters. */
    MODLOG_DFLT(INFO, "connection updated; status=%d ",
                event->conn_update.status);
    rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
    assert(rc == 0);
    bleprph_print_conn_desc(&desc);
    MODLOG_DFLT(INFO, "\n");
    return 0;

  case BLE_GAP_EVENT_ADV_COMPLETE:
    MODLOG_DFLT(INFO, "advertise complete; reason=%d",
                event->adv_complete.reason);
    bleprph_advertise();
    return 0;

  case BLE_GAP_EVENT_SUBSCRIBE:

    MODLOG_DFLT(INFO, "subscribe event; cur_notify=%d\n value handle; "
                      "val_handle=%d\n"
                      "conn_handle=%d attr_handle=%d "
                      "reason=%d prevn=%d curn=%d previ=%d curi=%d\n",
                event->subscribe.conn_handle,
                event->subscribe.attr_handle,
                event->subscribe.reason,
                event->subscribe.prev_notify,
                event->subscribe.cur_notify,
                event->subscribe.cur_notify, notification_handle, // Client Subscribed to notification_handle
                event->subscribe.prev_indicate,
                event->subscribe.cur_indicate);

    if (event->subscribe.attr_handle == notification_handle)
    {
      ESP_LOGI(BLE_TAG,"\nSubscribed with notification_handle =%d\n", event->subscribe.attr_handle);
      notify_state = event->subscribe.cur_notify; // As the client is now subscribed to notifications, the value is set to 1
      ESP_LOGI(BLE_TAG,"notify_state=%d\n", notify_state);
    }
    // if (event->subscribe.attr_handle == notification_handle)
    // {
    //   ESP_LOGI(BLE_TAG,"\nSubscribed with notification_handle =%d\n", event->subscribe.attr_handle);
    //   notify_state1 = event->subscribe.cur_notify; // As the client is now subscribed to notifications, the value is set to 1
    //   ESP_LOGI(BLE_TAG,"notify_state=%d\n", notify_state1);
    // }

    return 0;

  case BLE_GAP_EVENT_MTU:
    MODLOG_DFLT(INFO, "mtu update event; conn_handle=%d cid=%d mtu=%d\n",
                event->mtu.conn_handle,
                event->mtu.channel_id,
                event->mtu.value);
    return 0;
  }

  return 0;
}

static void
bleprph_on_reset(int reason)
{
  MODLOG_DFLT(ERROR, "Resetting state; reason=%d\n", reason);
}

static void
bleprph_on_sync(void)
{
  int rc;

  rc = ble_hs_util_ensure_addr(0);
  assert(rc == 0);

  /* Figure out address to use while advertising (no privacy for now) */
  rc = ble_hs_id_infer_auto(0, &own_addr_type);
  if (rc != 0)
  {
    MODLOG_DFLT(ERROR, "error determining address type; rc=%d\n", rc);
    return;
  }

  /* Printing ADDR */
  uint8_t addr_val[6] = {0};
  rc = ble_hs_id_copy_addr(own_addr_type, addr_val, NULL);

  MODLOG_DFLT(INFO, "Device Address: ");
  print_addr(addr_val);
  MODLOG_DFLT(INFO, "\n");
  /* Begin advertising. */
  bleprph_advertise();
}

void bleprph_host_task(void *param)
{
  ESP_LOGI(BLE_TAG, "BLE Host Task Started");
  /* This function will return only when nimble_port_stop() is executed */
  nimble_port_run();

  nimble_port_freertos_deinit();
}

#endif /* ENABLE_BLE */

/********************************* END OF FILE ********************************/
/******************************************************************************/