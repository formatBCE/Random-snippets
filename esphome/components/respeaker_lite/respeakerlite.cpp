#include "respeakerlite.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace respeakerlite {

static const char* const TAG = "respeaker_lite";

float RespeakerLite::get_setup_priority() const {
  return setup_priority::IO;
}

void RespeakerLite::setup() {
  ESP_LOGI(TAG, "Setting up RespeakerLite...");
}

uint8_t xmos_read_1byte(uint8_t resid, uint8_t cmd, uint8_t *value) {
    return xmos_read_bytes(resid, cmd, value, 1);
}

uint8_t xmos_read_bytes(uint8_t resid, uint8_t cmd, uint8_t *value, uint8_t read_byte_num) {
    if (read_byte_num == 0 || read_byte_num > 254) return 0xFF;
    
    uint8_t read_result;
    std::vector<uint8_t> data_to_send = {resid, cmd, (uint8_t)(read_byte_num + 1)};
    if (!this->write(data_to_send)) {
        ESP_LOGE(TAG, "Failed to write to XMOS device.");
        return 0xFF;
    }

    std::vector<uint8_t> read_data = this->read(read_byte_num + 1);
    if (read_data.empty()) {
        ESP_LOGE(TAG, "Failed to read from XMOS device.");
        return 0xFF;
    }

    read_result = read_data[0];
    for (uint8_t i = 0; i < read_byte_num; i++) {
        value[i] = read_data[i + 1];
    }

    return read_result;
}

void RespeakerLite::loop() {
  uint8_t mute_byte;
  if (this->xmos_read_1byte(0xF1, 0x81, &mute_byte) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read mute state");
    // this->mark_failed();
    //return;
  }
  bool new_mute_state = mute_byte == 0x01;
  if (this->mute_state_ != nullptr) {
    if (!this->mute_state_->has_state() || (this->mute_state_->state != new_mute_state)) {
      ESP_LOGD(TAG, "RespeakerLite mute state: %d", new_mute_state);
      this->mute_state_->publish_state(new_mute_state);
    }
  }
}

}  // namespace respeakerlite
}  // namespace esphome