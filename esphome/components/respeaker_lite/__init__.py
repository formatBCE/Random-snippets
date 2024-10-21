import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import (
    CONF_ID, 
    CONF_TRIGGER_ID
)
from esphome import automation

MULTI_CONF = True

AUTO_LOAD = [ "binary_sensor", "sensor" ]

DEPENDENCIES = ['i2c']

respeakerlite_ns = cg.esphome_ns.namespace('respeakerlite')
RespeakerLite = respeakerlite_ns.class_('RespeakerLite', i2c.I2CDevice, cg.Component)
CONF_MUTE_STATE= "mute_state"
CONF_FIRMWARE_VERSION= "firmware_version"
DEFAULT_ADDRESS = 0x42

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(RespeakerLite),
        cv.Optional(CONF_MUTE_STATE): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_FIRMWARE_VERSION): sensor.sensor_schema(),
    }
).extend(i2c.i2c_device_schema(DEFAULT_ADDRESS))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_MUTE_STATE in config:
        mute_state = await binary_sensor.new_binary_sensor(config[CONF_MUTE_STATE])
        cg.add(var.set_mute_state(mute_state))
    if CONF_FIRMWARE_VERSION in config:
        firmware_version = await sensor.new_sensor(config[CONF_FIRMWARE_VERSION])
        cg.add(var.set_firmware_version(firmware_version))