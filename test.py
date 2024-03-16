import vktem

for devs in vktem.get_devices():
    print(devs)

config = vktem.Config()

config.setImageSize(1024, 1024)
config.setAtomicPotentialsSize(80, 80, 11)
config.setMaxAtomCount(10000)

sim = vktem.Simulator(config)