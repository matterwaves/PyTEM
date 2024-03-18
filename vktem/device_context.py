import vktem_native_submodule as tem_native

import numpy as np

class DeviceContext:
    def __init__(self, devices: list[int]) -> None:
        self._handle = tem_native.create_device_context(devices)