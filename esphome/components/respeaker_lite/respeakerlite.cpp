#include "respeakerlite.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace respeakerlite {

static const char* const TAG = "respeaker_lite";

float RespeakerLite::get_setup_priority() const {
  return setup_priority::IO;
}

void RespeakerLite::setup() {
  ESP_LOGI(TAG, "Setting up RespeakerLite...");
}

unsigned long last_time = 0;
const unsigned long interval = 1000;

void RespeakerLite::get_firmware_version_() {
  const uint8_t version_req[] = {0xF0, 0xD8, 4};
  uint8_t version_resp[4];

  auto error_code = this->write(version_req, sizeof(version_req));
  if (error_code != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Request version failed");
    return;
  }

  error_code = this->read(version_resp, sizeof(version_resp));
  if (error_code != i2c::ERROR_OK || version_resp[0] != 0) {
    ESP_LOGW(TAG, "Read version failed");
    return;
  }
  ESP_LOGI(TAG, "DFU version: %u.%u.%u", version_resp[1], version_resp[2], version_resp[3]);
  this->firmware_version_->publish_state(std::to_string(version_resp[1]) + "." + std::to_string(version_resp[2]) + "." + std::to_string(version_resp[3]));
}

void RespeakerLite::loop() {
  if (!this->firmware_version_->has_state()) {
    get_firmware_version()
  }
  unsigned long current_time = millis();
  if (current_time - last_time >= interval) {
    last_time = current_time;
    uint8_t command[3] = {0xF1, 0x81, 0x01};
    this->write(command, 3);

    uint8_t data[2];
    if (this->read(data, 2) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "Unable to read mute state");
      return;
    }
    bool new_mute_state = data[1] == 0x01;
    if (this->mute_state_ != nullptr) {
      if (!this->mute_state_->has_state() || (this->mute_state_->state != new_mute_state)) {
        ESP_LOGD(TAG, "Mic mute state: %d", new_mute_state);
        this->mute_state_->publish_state(new_mute_state);
      }
    }
  }
}

}  // namespace respeakerlite
}  // namespace esphome