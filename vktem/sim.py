import vktem_native_submodule as tem_native

class Config:
    def __init__(self) -> None:
        self.rows = 0
        self.cols = 0
        self.atomicPotentialsRows = 0
        self.atomicPotentialsCols = 0
        self.atomicPotentialsCount = 0
        self.maxAtomCount = 0
    
    def setImageSize(self, rows: int, cols: int):
        self.rows = rows
        self.cols = cols
    
    def setAtomicPotentialsSize(self, rows: int, cols: int, count: int):
        self.atomicPotentialsRows = rows
        self.atomicPotentialsCols = cols
        self.atomicPotentialsCount = count
    
    def setMaxAtomCount(self, max_count: int):
        self.maxAtomCount = max_count

    def getConfigDict(self):
        return {
            "rows": self.rows,
            "cols": self.cols,
            "atomicPotentialsRows": self.atomicPotentialsRows,
            "atomicPotentialsCols": self.atomicPotentialsCols,
            "atomicPotentialsCount": self.atomicPotentialsCount,
            "maxAtomCount": self.maxAtomCount
        }

class Simulator:
    def __init__(self, config: Config, device_index: int = 0) -> None:
        self.simulator_handle = tem_native.create_simulator(device_index, config.getConfigDict())

        print(self.simulator_handle)
