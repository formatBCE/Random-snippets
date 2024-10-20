#include "respeakerlitec.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace respeakerlitec {

static const char* const TAG = "respeaker_lite_c";

static const uint8_t RESPEAKER_LITE_C_MUTE_STATE = 0x81;

float RespeakerLiteC::get_setup_priority() const {
  return setup_priority::IO;
}

void RespeakerLiteC::setup() {
  ESP_LOGI(TAG, "Setting up RespeakerLiteC...");
}

void RespeakerLiteC::loop() {
  uint8_t data[1];
 
  if (this->read_register(RESPEAKER_LITE_C_MUTE_STATE, data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read mute state");
    this->mark_failed();
    return;
  }
  bool new_mute_state = data[0] == 0x01;
  if (this->mute_state_ != nullptr) {
    if (!this->mute_state_->has_state() || (this->mute_state_->state != new_mute_state)) {
      ESP_LOGD(TAG, "RespeakerLiteC mute state: %d", new_mute_state);
      this->mute_state_->publish_state(new_mute_state);
    }
  }
}

}  // namespace respeakerlitec
}  // namespace esphome