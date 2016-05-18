from gpio import GPIO
import spidev

class NRF24:

    R_RX_PAYLOAD = 0x61
    R_RX_PL_WID = 0x60

    REG_CONFIG = 0x00
    PRIM_RX = 0
    PWR_UP = 1

    REG_STATUS = 0x07
    MAX_RT = 4
    TX_DS = 5
    RX_DR = 6

    REG_PW_P0 = 0x11

    def __init__(self, ce , irq, spi_bus, spi_device):
        self.ce = GPIO(ce, GPIO.OUTPUT)
        self.irq = GPIO(irq, GPIO.INPUT)
        self.irq.set_edge(GPIO.FALLING)
        self.irq.set_callback(self._callback)
        self.irq.enable_edge_detect()
        self.spi = spidev.SpiDev()

        self.spi.open(spi_bus, spi_device)
        self.spi.bits_per_word = 8
        self.spi.max_speed_hz = 6000000
        self.spi.mode = 0
        self.spi.cshigh = False
        self.spi.lsbfirst = False
        self.spi.loop = False
        self.spi.threewire = False

    def _get_interrupt_reason(self):
        status = self.reg_read(self.REG_STATUS)[0]
        if status & ~(1 << self.TX_DS):
            return self.TX_DS
        elif status & ~(1 << self.RX_DR):
            return self.RX_DR
        elif status & ~(1 << self.MAX_RT):
            return self.MAX_RT

    def _clear_interrupt(self, reason):
        self.reg_bit_set(self.REG_STATUS, reason, 1)

    def _callback(self):
        reason = self._get_interrupt_reason()
        if self.user_callback is not None:
            self.user_callback(reason)
        self._clear_interrupt(reason)


    def set_callback(self, callback):
        self.user_callback = callback

    def send_cmd(self, cmd, data):
        result = self.spi.xfer2([cmd] + data)
        return result[1:]

    def reg_read(self, reg):
        return self.send_cmd(reg, [0])

    def reg_write(self, reg, data):
        return self.send_cmd(0x20 | reg, data)

    def reg_bit_set(self, reg, bit, value):
        data = self.reg_read(reg)
        data[bit // 8] = (data[bit // 8] & ~(1 << (bit % 8))) | (value << (bit % 8))
        return self.reg_write(reg, data)

    def pwr_up(self):
        return self.reg_bit_set(self.REG_CONFIG, self.PWR_UP, 1)
    def pwr_down(self):
        return self.reg_bit_set(self.REG_CONFIG, self.PWR_UP, 0)

    def set_prx(self):
        return self.reg_bit_set(self.REG_CONFIG, self.PRIM_RX, 1)

    def datapipe_width_set(self, pipe, width):
        return self.reg_write(self.REG_PW_P0 + pipe, [width])

    def read_rx_payload_len(self):
        return self.send_cmd(self.R_RX_PL_WID, [0])[0]

    def read_rx_payload(self):
        return self.send_cmd(self.R_RX_PAYLOAD, [0] * self.read_rx_payload_len())

    def enable(self):
        self.ce.set_high()
