#pragma once

#include "GDiskSubplugin.hpp"
#include "guid.h"
#include "plugin_version.hpp"

//------------------------------------------------------------------------------
TSubplugin::TSubplugin(HINSTANCE HInst,
  const subplugin_startup_info_t * startup_info) :
  TBaseSubplugin(HInst, startup_info),
  FTabID(0),
  FTabControlID(0),
  FProtocolID(0)
{
  // DEBUG_PRINTF(L"begin")
  // DEBUG_PRINTF(L"end")
}
//------------------------------------------------------------------------------
TSubplugin::~TSubplugin()
{
  // DEBUG_PRINTF(L"begin")
  // FarConfiguration->SetPlugin(NULL);
  // CoreFinalize();
  // DEBUG_PRINTF(L"end")
}
//------------------------------------------------------------------------------
subplugin_error_t TSubplugin::NotifyEditSessionInitTabs(subplugin_t * subplugin,
  const notification_t * notification)
{
  // DEBUG_PRINTF(L"begin");
  FTabID = FStartupInfo.get_next_id(subplugin);
  // DEBUG_PRINTF(L"FTabID = %d", FTabID);
  const wchar_t * TabCaption = FStartupInfo.get_subplugin_msg(subplugin, L"Tab.Caption");
  // DEBUG_PRINTF(L"TabCaption = %s", TabCaption);
  send_message_baton_t baton;
  baton.struct_size = sizeof(baton);
  baton.subplugin = subplugin;
  baton.notification = notification;
  baton.message_id = L"addtab";
  key_value_pair_t pair;
  pair.struct_size = sizeof(pair);
  pair.key = FTabID;
  pair.value = TabCaption;
  baton.message_data = &pair;
  FTabControlID = reinterpret_cast<intptr_t>(FStartupInfo.send_message(&baton));

  // DEBUG_PRINTF(L"end");
  return SUBPLUGIN_NO_ERROR;
}
//------------------------------------------------------------------------------
subplugin_error_t TSubplugin::NotifyEditSessionInitSessionTab(subplugin_t * subplugin,
  const notification_t * notification)
{
  // DEBUG_PRINTF(L"begin");
  // int DialogItemID = FStartupInfo.get_dialog_item_id(subplugin, notification, L"TransferProtocolCombo");
  // DEBUG_PRINTF(L"DialogItemID = %d", DialogItemID);
  FProtocolID = FStartupInfo.get_next_id(subplugin);
  // DEBUG_PRINTF(L"FProtocolID = %d", FProtocolID);
  const wchar_t * ProtocolName = FStartupInfo.get_subplugin_msg(subplugin, L"Protocol.Name");
  // DEBUG_PRINTF(L"ProtocolName = %s", ProtocolName);
  send_message_baton_t baton;
  baton.struct_size = sizeof(baton);
  baton.subplugin = subplugin;
  baton.notification = notification;
  baton.message_id = L"addprotocoldescription";
  key_value_pair_t pair;
  pair.struct_size = sizeof(pair);
  pair.key = FProtocolID;
  pair.value = ProtocolName;
  baton.message_data = &pair;
  FStartupInfo.send_message(&baton);

  // DEBUG_PRINTF(L"end");
  return SUBPLUGIN_NO_ERROR;
}
//------------------------------------------------------------------------------
subplugin_error_t TSubplugin::NotifyEditSessionAfterInitSessionTabs(subplugin_t * subplugin,
  const notification_t * notification)
{
  // DEBUG_PRINTF(L"begin");
  send_message_baton_t baton;
  baton.struct_size = sizeof(baton);
  baton.subplugin = subplugin;
  baton.notification = notification;

  baton.message_id = L"setnextitemposition";
  baton.message_data = reinterpret_cast<const void *>(ipNewLine);
  FStartupInfo.send_message(&baton);

  baton.message_id = L"setdefaultgroup";
  baton.message_data = reinterpret_cast<const void *>(FTabID);
  FStartupInfo.send_message(&baton);

  baton.message_id = L"newseparator";
  baton.message_data = reinterpret_cast<const void *>(FStartupInfo.get_subplugin_msg(subplugin, L"Separator.Caption"));
  FStartupInfo.send_message(&baton);

  // DEBUG_PRINTF(L"end");
  return SUBPLUGIN_NO_ERROR;
}
//------------------------------------------------------------------------------
subplugin_error_t TSubplugin::NotifyEditSessionUpdateControls(subplugin_t * subplugin,
  const notification_t * notification)
{
  // DEBUG_PRINTF(L"begin");
  property_baton_t baton;
  baton.struct_size = sizeof(baton);
  baton.subplugin = subplugin;
  baton.notification = notification;

  baton.item_id = 0;
  baton.property_name = L"protocol";
  baton.property_value = NULL;
  // int CurProtocol = reinterpret_cast<int>(FStartupInfo.dialog_item_get_property(subplugin, notification, 0, L"protocol", NULL));
  intptr_t CurProtocol = reinterpret_cast<intptr_t>(FStartupInfo.dialog_item_get_property(&baton));
  // DEBUG_PRINTF(L"FProtocolID = %d, CurProtocol = %d", FProtocolID, CurProtocol);
  // FStartupInfo.dialog_item_set_property(subplugin, notification, FTabControlID,
    // L"enabled", (const void *)(CurProtocol == FProtocolID));
  baton.item_id = FTabControlID;
  baton.property_name = L"enabled";
  baton.property_value = reinterpret_cast<void *>(CurProtocol == FProtocolID);
  FStartupInfo.dialog_item_set_property(&baton);
  // DEBUG_PRINTF(L"end");
  return SUBPLUGIN_NO_ERROR;
}
//------------------------------------------------------------------------------
subplugin_error_t TSubplugin::Notify(
  subplugin_t * subplugin, const notification_t * notification)
{
  // DEBUG_PRINTF(L"begin");
  assert(notification);

  typedef subplugin_error_t (TSubplugin::*notify_handler_t)(
    subplugin_t * subplugin, const notification_t * notification);

  struct notification_func_t
  {
    intptr_t message_id;
    const wchar_t * text;
    notify_handler_t handler;
  };

  static notification_func_t notify_handlers[] =
  {
    {
      SUBPLUGIN_MSG_SESSION_DIALOG_INIT,
      L"init tabs",
      &TSubplugin::NotifyEditSessionInitTabs
    },
    {
      SUBPLUGIN_MSG_SESSION_DIALOG_INIT,
      L"init session tab",
      &TSubplugin::NotifyEditSessionInitSessionTab
    },
    {
      SUBPLUGIN_MSG_SESSION_DIALOG_INIT,
      L"after init tabs",
      &TSubplugin::NotifyEditSessionAfterInitSessionTabs
    },
    {
      SUBPLUGIN_MSG_SESSION_DIALOG_UPDATE_CONTROLS,
      NULL,
      &TSubplugin::NotifyEditSessionUpdateControls
    },
    {-1}
  };

  // DEBUG_PRINTF(L"notification->message_id = %d", notification->message_id);
  for (notification_func_t * f = notify_handlers; f->message_id != -1; ++f)
  {
    if ((f->message_id == notification->message_id) &&
        ((notification->text && f->text &&
         (wcsncmp(notification->text, f->text, notification->text_length) == 0) &&
         (wcslen(f->text) == notification->text_length)) ||
         (!notification->text && !f->text)) &&
        f->handler)
    {
      return (this->*(f->handler))(subplugin, notification);
    }
  }
  // DEBUG_PRINTF(L"end");
  return SUBPLUGIN_NO_ERROR;
}
//------------------------------------------------------------------------------
subplugin_error_t TSubplugin::GetMetaData(subplugin_t * subplugin,
  subplugin_meta_data_t * meta_data)
{
  wchar_t Buffer[100] = {0};
  wchar_t * Guid = ::GuidToStr(GDiskGuid, Buffer, sizeof(Buffer));
  meta_data->guid = FStartupInfo.NSF->pstrdup(subplugin, Guid, wcslen(Guid));
  meta_data->api_version =
    (NETBOX_VERSION_MAJOR << 24) +
    (NETBOX_VERSION_MINOR << 16) +
    (NETBOX_VERSION_PATCH << 8) +
    NETBOX_VERSION_BUILD;
  meta_data->version =
    (SUBPLUGIN_VERSION_MAJOR << 24) +
    (SUBPLUGIN_VERSION_MINOR << 16) +
    (SUBPLUGIN_VERSION_PATCH << 8) +
    SUBPLUGIN_VERSION_BUILD;
  return SUBPLUGIN_NO_ERROR;
}
//------------------------------------------------------------------------------
