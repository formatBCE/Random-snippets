#include "respeakerlite.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "Wire.h"

namespace esphome {
namespace respeakerlite {

static const char* const TAG = "respeaker_lite";

float RespeakerLite::get_setup_priority() const {
  return setup_priority::IO;
}

uint8_t xmos_read_1byte(uint8_t resid, uint8_t cmd, uint8_t *value) {
    return xmos_read_bytes(resid, cmd, value, 1);
}

uint8_t xmos_read_bytes(uint8_t resid, uint8_t cmd, uint8_t *value, uint8_t read_byte_num) {
    uint8_t read_result;
    if (read_byte_num == 0 || read_byte_num > 254) return 0xFF;
    read_byte_num++;
    Wire.beginTransmission(0x42);
    Wire.write(resid);
    Wire.write(cmd);
    Wire.write(read_byte_num);
    Wire.endTransmission();

    Wire.requestFrom(0x42, read_byte_num);
    
    read_result = Wire.read();
    for (uint8_t i = 0; i < read_byte_num - 1; i++) {
        value[i] = Wire.read();
    }
    
    return read_result;
}

void RespeakerLite::setup() {
  ESP_LOGI(TAG, "Setting up RespeakerLite...");
}

void RespeakerLite::loop() {
  uint8_t command = [0xF1, 0x81, 0x01]
  uint8_t data[1];
  write(&command, 3)
  if (this->read(&data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read mute state");
    this->mark_failed();
    return;
  }
  bool new_mute_state = data[0] == 0x01;
  if (this->mute_state_ != nullptr) {
    if (!this->mute_state_->has_state() || (this->mute_state_->state != new_mute_state)) {
      ESP_LOGD(TAG, "RespeakerLite mute state: %d", new_mute_state);
      this->mute_state_->publish_state(new_mute_state);
    }
  }
}

}  // namespace respeakerlite
}  // namespace esphome