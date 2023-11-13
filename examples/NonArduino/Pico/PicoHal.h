#ifndef PICO_HAL_H
#define PICO_HAL_H

// include RadioLib
#include <RadioLib.h>


#include <pico/stdlib.h>

#include "hardware/spi.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define cs_pin 14
#define sck_pin 18
#define mosi_pin 19
#define miso_pin 16

class PicoHal : public RadioLibHal {
public:

    PicoHal(uint8_t spiChannel, uint32_t spiSpeed = 500 * 1000)
      : RadioLibHal(GPIO_IN, GPIO_OUT, 0, 1, GPIO_IRQ_EDGE_RISE, GPIO_IRQ_EDGE_FALL), 
      _spiChannel(spiChannel),
      _spiSpeed(spiSpeed) {
    }

    void init() override {
        stdio_init_all();
        spiBegin();
    }

    void term() override {
        spiEnd();
    }

    void pinMode(uint32_t pin, uint32_t mode) override {
        if (pin == RADIOLIB_NC) {
            return;
        }

        gpio_set_dir(pin, mode);
    }

    void digitalWrite(uint32_t pin, uint32_t value) override {
        if (pin == RADIOLIB_NC) {
            return;
        }

        gpio_put(pin, (bool)value);
    }

    uint32_t digitalRead(uint32_t pin) override {
        if (pin == RADIOLIB_NC) {
            return 0;
        }

        return gpio_get(pin);
    }

    void attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) override {
        if (interruptNum == RADIOLIB_NC) {
            return;
        }

        gpio_set_irq_enabled_with_callback(interruptNum, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t)interruptCb);
    }

    void detachInterrupt(uint32_t interruptNum) override {
        if (interruptNum == RADIOLIB_NC) {
            return;
        }

        gpio_set_irq_enabled_with_callback(interruptNum, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, NULL);
    }

    void delay(unsigned long ms) override {
        sleep_ms(ms);
    }

    void delayMicroseconds(unsigned long us) override {
        sleep_us(us);
    }

    unsigned long millis() override {
        // return time_us_32() / 1000;
        return to_ms_since_boot(get_absolute_time());
    }

    unsigned long micros() override {
        // return time_us_32();
        return to_us_since_boot(get_absolute_time());
    }

    long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) override {
        if (pin == RADIOLIB_NC) {
            return 0;
        }

        this->pinMode(pin, GPIO_IN);
        uint32_t start = this->micros();
        uint32_t curtick = this->micros();

        while (this->digitalRead(pin) == state) {
            if ((this->micros() - curtick) > timeout) {
                return 0;
            }
        }

        return (this->micros() - start);
    }

    void spiBegin() {
        if (_spiHandle < 0) {
            printf("init spi\n");

            if (_spiChannel == 0) {
                printf("spi speed: %d\n", _spiSpeed);
                _spiHandle = spi_init(spi0, _spiSpeed);
                spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            } else {
                _spiHandle = spi_init(spi1, _spiSpeed);
                spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            }

            gpio_set_function(sck_pin, GPIO_FUNC_SPI);
            gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
            gpio_set_function(miso_pin, GPIO_FUNC_SPI);

            
        }
    }

    void spiBeginTransaction() {}

    void spiTransfer(uint8_t* out, size_t len, uint8_t* in) {
        printf("spi transfer\n");
        spi_write_read_blocking(spi0, out, in, len);
    }

    void spiEndTransaction() {}

    void spiEnd() {
        if (_spiHandle >= 0) {
            printf("end spi\n");
            if (_spiChannel == 0) {
                spi_deinit(spi0);
            } else {
                spi_deinit(spi1);
            }
            _spiHandle = -1;
        }
    }

private:

    const unsigned int _spiSpeed;
    const uint8_t _spiChannel;
    int _spiHandle = -1;

};


#endif