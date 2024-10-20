#pragma once

#include <utility>
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace respeakerlite {

class RespeakerLite : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;
  float get_setup_priority() const override;

  void set_mute_state(binary_sensor::BinarySensor* mute_state) {
    this->mute_state_ = mute_state;
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

 protected:
  binary_sensor::BinarySensor* mute_state_{nullptr};
};

}  // namespace respeakerlite
}  // namespace esphome