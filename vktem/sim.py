import vktem_native_submodule as tem_native

import numpy as np

class Config:
    def __init__(self) -> None:
        self.rows = 0
        self.cols = 0
        self.atomicPotentialsRows = 0
        self.atomicPotentialsCols = 0
        self.atomicPotentialsCount = 0
        self.maxAtomCount = 0
        self.pixelSize = 0
    
    def setImageSize(self, rows: int, cols: int):
        self.rows = rows
        self.cols = cols
    
    def setAtomicPotentialsSize(self, count: int, rows: int, cols: int):
        self.atomicPotentialsCount = count
        self.atomicPotentialsRows = rows
        self.atomicPotentialsCols = cols
    
    def setMaxAtomCount(self, max_count: int):
        self.maxAtomCount = max_count
    
    def setPixelSize(self, pixel_size: float):
        self.pixelSize = pixel_size

    def getConfigDict(self):
        return {
            "rows": self.rows,
            "cols": self.cols,
            "atomicPotentialsRows": self.atomicPotentialsRows,
            "atomicPotentialsCols": self.atomicPotentialsCols,
            "atomicPotentialsCount": self.atomicPotentialsCount,
            "maxAtomCount": self.maxAtomCount,
            "pixelSize": self.pixelSize
        }

class Simulator:
    def __init__(self, config: Config, device_index: int = 0) -> None:
        self.simulator_handle = tem_native.create_simulator(device_index, config.getConfigDict())

        print(self.simulator_handle)
    
    def setAtomicPotentials(self, atomic_potentials: np.ndarray):
        tem_native.set_atomic_potentials(self.simulator_handle, atomic_potentials)
    
    def setAtoms(self, coords: np.ndarray, proton_counts: np.ndarray):
        tem_native.set_atoms(self.simulator_handle, coords, proton_counts)

    def computePotential(self, angles: list[float]):
        tem_native.compute_potential(self.simulator_handle, angles)
    
    def getPotential(self):
        return tem_native.get_potential(self.simulator_handle)
