import vktem

for devs in vktem.get_devices():
    print(devs)

sim = vktem.Simulator(0, 100, 100)