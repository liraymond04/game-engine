---@meta rtc_handler

--------------------------------------------------------------------------------
--- RTC Handler
--------------------------------------------------------------------------------
---
---Generate UUID string
---@return string
---
function generate_uuid() end

---
---Initialize RTC connection information
---@param ws_url string Websocket URL
---@param username string Username to connect with
---@param room string Room code to join
---
function rtc_initialize(ws_url, username, room) end

---
---Start RTC connection 
---
function rtc_handle_connection() end

---
---Cleanup RTC connection 
---
function rtc_cleanup() end

---
---Send message to connected data channels
---@param message string Message string to send
---
function rtc_send_message(message) end

---
---Send json message to connected data channels
---@param type string Type as string
---@param object string Object string
---
function rtc_send_typed_object(type, object) end

---
---Set callback function when data channel is opened
---@param on_message_opened function cb(int id, void *ptr)
---
function rtc_set_message_opened_callback(on_message_opened) end

---
---Set callback function when message is received from a data channel
---@param on_message_received function cb(int id, const char *message, int size, void *ptr)
---
function rtc_set_message_received_callback(on_message_received) end

---
---Set callback function when data channel is closed
---@param on_message_closed function cb(int id, void *ptr)
---
function rtc_set_message_closed_callback(on_message_closed) end
