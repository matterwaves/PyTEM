import vktem
import numpy as np
import os
import time

from pandas import read_csv
from scipy import ndimage, special
from matplotlib import pyplot as plt

a0 = 0.529 # bohr radius [A]
e_charge_VA = 14.4 # electron charge [V*A]

def get_binned(arr, bin_factor, mean_flag=False):
    """
    Bins an array by bin_factor, a divisor of the array size.
    """
    if np.mod(arr.shape[0], bin_factor) != 0:
        raise Exception('bin_factor should be a divisor of array size')
    elif np.mod(arr.shape[1], bin_factor) != 0:
        raise Exception('bin_factor should be a divisor of array size')
    if mean_flag:
        arr /= (bin_factor**2) # return mean instead of sum of elements
    return np.add.reduceat(np.add.reduceat(arr,
                                           np.arange(0, arr.shape[0],
                                                     bin_factor),
                                           axis=0),
                           np.arange(0, arr.shape[1],
                                     bin_factor),
                           axis=1)

def get_atomicPotential(proton_count, pixel_size, sample_size=11, ss_factor=16, params_file='fparams.csv',
                        params_path=os.path.dirname(os.path.realpath(__file__))):
    """
    Calculates potential for an atom by super-sampling.
    """
	# get atom parameters
    with open(os.path.join(params_path, params_file)) as csvfile:
        params_list = read_csv(csvfile, header=None).to_numpy()
    ap = np.array(params_list[proton_count,:]).astype(float)

	# make super-sampled grid
    sup_ax = np.linspace(-sample_size/2, sample_size/2, ss_factor*int(sample_size/pixel_size))
    [sup_c, sup_r] = np.meshgrid(sup_ax, sup_ax) # [A] super-sampled
    sup_rad = (sup_r**2 + sup_c**2)**(1/2) # [A] radial coordinate

	# Eq C.20, Kirkland, "Advanced Computing in Electron Microscopy" (2020)
    pot_const = 2 * np.pi**2 * a0 * e_charge_VA
    sup_pot = (2 * ap[0] * special.kn(0, 2 * np.pi * sup_rad * np.sqrt(ap[1])) +\
            2 * ap[2] * special.kn(0, 2 * np.pi * sup_rad * np.sqrt(ap[3])) +\
                2 * ap[4] * special.kn(0, 2 * np.pi * sup_rad * np.sqrt(ap[5])) +\
                    ap[6] / ap[7 ] * np.exp(-np.pi**2 * sup_rad**2 / ap[7]) +\
                        ap[8] / ap[9] * np.exp(-np.pi**2 * sup_rad**2 / ap[9]) +\
                            ap[10] / ap[11] * np.exp(-np.pi**2 * sup_rad**2 / ap[11])) * pot_const

	# integrate super-sampled squares
    mini_pot = get_binned(sup_pot, ss_factor)
    mini_pot /= ss_factor**2

	# cut off radius of potential to remove edge discontinuities
    n_grid_pix = len(sup_ax) / ss_factor
    dc = np.round(np.sqrt(n_grid_pix))
    ci = np.floor(n_grid_pix / 2)
    li = n_grid_pix - 1
    xy_cutoff = [(ci - dc, 0), (ci + dc, 0), (ci - dc, li), (ci + dc, li),
              (0, ci - dc), (0, ci + dc), (li, ci - dc), (li, ci + dc)]
    pot_min = []
    for (x, y) in xy_cutoff:
        pot_min.append(mini_pot[int(x), int(y)])
    pot_min = np.max(pot_min) # baseline value

    pot = mini_pot - pot_min # subtract baseline
    pot[pot < 0] = 0 # potential is non-negative

    return pot

def get_pseudowaterPotential(pix_size, scale_factor=1.22338, sample_size=11, ss_factor=16):
    """
    Calculates potential of a 'pseudo-water' atom using a scaled oxygen atom potential.
    """
    pot_O = get_atomicPotential(8, pix_size, sample_size=sample_size, ss_factor=ss_factor)
    
    return scale_factor * pot_O


proton_count_dict = {'H': 1, 'C': 6, 'N': 7, 'O': 8,
                     'NA': 11, 'MG': 12, 'P': 15, 'S': 16,
                     'FE': 26, 'AU': 79}

def get_atomPots(pix_size_rc):
    """
    Prepare atom potentials for the different elements. Pseudowater is added as index 0.
    """
    unique_potentials = {}

    # add all elements indexed by atomic number
    for proton_count in proton_count_dict.values():
        unique_potentials.update({ proton_count: get_atomicPotential(proton_count, pix_size_rc) })

    # add pseudowater to index 0
    unique_potentials.update({ 0: get_pseudowaterPotential(pix_size_rc) })

    # prepare atom_pots array
    atom_pot_shape = (max(unique_potentials.keys()) + 1,
                      *(next(iter(unique_potentials.values())).shape))
    atom_pots = np.zeros(shape=atom_pot_shape)
    for kk in unique_potentials:
        atom_pots[kk, :, :] = unique_potentials[kk]

    return atom_pots

for devs in vktem.get_devices():
    print(devs)

pix_size_rc = 1.058
atom_pots = get_atomPots(pix_size_rc)

data = np.load('4v6x_ribosome.npz')
coords = data["coords"]
proton_counts = data["proton_counts"]

config = vktem.Config()

config.setImageSize(512, 512)
config.setAtomicPotentialsSize(*atom_pots.shape)
config.setMaxAtomCount(coords.shape[0])
config.setPixelSize(pix_size_rc)

sim = vktem.Simulator(config)

start_time = time.time()

sim.setAtomicPotentials(atom_pots)
sim.setAtoms(coords, proton_counts)

sim.computePotential([0.0, 18.0, 0.0])

print("--- %s seconds ---" % (time.time() - start_time))


potential = sim.getPotential() / (512 * 512 * 512 * 512)

fig, axs = plt.subplots(1, 2, figsize=(10, 5)) # Create a figure and a 1x2 subplot

# Plot the real part on the first subplot
axs[0].imshow(potential.real, cmap='viridis')
axs[0].set_title('Real Part')

# Plot the imaginary part on the second subplot
axs[1].imshow(potential.imag, cmap='viridis')
axs[1].set_title('Imaginary Part')

plt.show()