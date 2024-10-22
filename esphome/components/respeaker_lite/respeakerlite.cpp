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
  this->set_timeout(3000, [this]() {
    if (!this->get_firmware_version_()) {
      ESP_LOGE(TAG, "Failed to initialize DFU");
      this->mark_failed();
    }
  });
}

unsigned long last_time = 0;
const unsigned long interval = 1000;

bool RespeakerLite::get_firmware_version_() {
  if (this->firmware_version_ != nullptr) {
    if (!this->firmware_version_->has_state()) {
      const uint8_t version_req[] = {0xF0, 0xD8, 4};
      uint8_t version_resp[4];

      auto error_code = this->write(version_req, sizeof(version_req));
      if (error_code != i2c::ERROR_OK) {
        ESP_LOGW(TAG, "Request version failed");
        return false;
      }

      error_code = this->read(version_resp, sizeof(version_resp));
      if (error_code != i2c::ERROR_OK || version_resp[0] != 0) {
        ESP_LOGW(TAG, "Read version failed");
        return false;
      }
      
      std::string version = str_sprintf("%u.%u.%u", version_resp[1], version_resp[2], version_resp[3]);
      ESP_LOGI(TAG, "DFU version: %s", version.c_str());
      this->firmware_version_->publish_state(version);
    }
  }
  return true;
}

void RespeakerLite::get_mute_state_() {
  uint8_t mute_req[3] = {0xF1, 0x81, 1};
  uint8_t mute_resp[2];

  auto error_code = this->write(mute_req, sizeof(mute_req));
  if (error_code != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Request mic mute state failed");
    return;
  }

  error_code = this->read(mute_resp, sizeof(mute_resp));
  if (error_code != i2c::ERROR_OK || mute_resp[0] != 0) {
    ESP_LOGW(TAG, "Read mic mute state failed");
    return;
  }

  bool new_mute_state = mute_resp[1] == 0x01;
  if (this->mute_state_ != nullptr) {
    if (!this->mute_state_->has_state() || (this->mute_state_->state != new_mute_state)) {
      ESP_LOGI(TAG, "Mic mute state: %d", new_mute_state);
      this->mute_state_->publish_state(new_mute_state);
    }
  }
}

void RespeakerLite::loop() {
    this->get_mute_state_();
}

}  // namespace respeakerlite
}  // namespace esphome