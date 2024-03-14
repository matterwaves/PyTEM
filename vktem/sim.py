import vktem_native_submodule as tem_native

class Simulator:
    def __init__(self, device_index: int, rows: int, cols: int) -> None:
        self.simulator_handle = tem_native.create_simulator(device_index, rows, cols)

        print(self.simulator_handle)
