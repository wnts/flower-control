from nrf24 import NRF24
from gpio import GPIO
from time import sleep

def my_callback(reason):
    res = nrf.read_rx_payload()
    val = res[0] | res[1] << 8
    print val


nrf = NRF24(14, 15, 0, 0)
nrf.pwr_up()
nrf.set_prx()
nrf.datapipe_width_set(0, 2)
nrf.set_callback(my_callback)
nrf.enable()
print "starting receive..."
print "0x%02x" % nrf.reg_read(nrf.REG_STATUS)[0]









