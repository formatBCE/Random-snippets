#include "respeakerlite.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace respeakerlite {

static const char* const TAG = "respeaker_lite";

static const uint8_t RESPEAKER_LITE_MUTE_STATE = 0x81;

float RespeakerLite::get_setup_priority() const {
  return setup_priority::IO;
}

void RespeakerLite::setup() {
  ESP_LOGI(TAG, "Setting up RespeakerLite...");
}

uint32_t now_time = 0;

void RespeakerLite::loop() {
  uint8_t data;
  if (this->read_byte(RESPEAKER_LITE_MUTE_STATE, &data) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read mute state");
    this->mark_failed();
    return;
  }
  bool new_mute_state = data == 0x01;
  if (this->mute_state_ != nullptr) {
    ESP_LOGD(TAG, "RespeakerLite mute state: %d", new_mute_state);
    if (!this->mute_state_->has_state() || (this->mute_state_->state != new_mute_state)) {
      this->mute_state_->publish_state(new_mute_state);
    }
  }
}

}  // namespace respeakerlite
}  // namespace esphome