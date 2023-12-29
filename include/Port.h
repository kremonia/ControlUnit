//
// Created by alex on 22-Dec-23.
//

#pragma once

#include <Arduino.h>

namespace Ports {
    struct Port {
        volatile uint8_t *DirectionReg;
        volatile uint8_t *DataWriteReg;
        volatile uint8_t *DataReadReg;
    };

    // Global ports variables
    const Port PortA{&DDRA, &PORTA, &PINA};
    const Port PortB{&DDRB, &PORTB, &PINB};
    const Port PortC{&DDRC, &PORTC, &PINC};
    const Port PortD{&DDRD, &PORTD, &PIND};
    const Port PortE{&DDRE, &PORTE, &PINE};
    const Port PortF{&DDRF, &PORTF, &PINF};
    const Port PortG{&DDRG, &PORTG, &PING};
    const Port PortH{&DDRH, &PORTH, &PINH};
    const Port PortJ{&DDRJ, &PORTJ, &PINJ};
    const Port PortK{&DDRK, &PORTK, &PINK};
    const Port PortL{&DDRL, &PORTL, &PINL};

    /// Read a whole port value
    /// \param Port - port constant to read from
    /// \return value along the port states
    uint8_t inline static read(Port &Port) {
        auto RegState = *Port.DirectionReg;     // cache previous directions
        *Port.DirectionReg = 0x0;                      // set all directions to input
        auto RegRead = *Port.DataReadReg;       // read value
        *Port.DirectionReg = RegState;                 // restore previous directions
        return RegRead;
    }

    /// Write a whole port value
    /// \param Port - port constant to write
    /// \param value - value to write
    void inline static write(Port &Port, const uint8_t &value) {
        auto RegState = *Port.DirectionReg;      // cache previous directions
        *Port.DirectionReg = 0xFF;                      // set all directions to output
        *Port.DataWriteReg = value;                     // write value
        *Port.DirectionReg = RegState;                  // restore previous directions
    }

    /// Read a single bit value
    /// \param Port - port constant to read from
    /// \param bit - bit to read
    /// \return value of the bit\n
    /// Does change the direction of the port pin
    bool inline static readBit(Port &Port, const uint8_t &bit) {
        bitClear(*Port.DirectionReg, bit);              // set direction to input
        auto value =  bitRead(*Port.DataReadReg, bit);// read value
        return (bool)value;
    }

    /// Write a single bit value
    /// \param Port - port constant to write
    /// \param bit - bit to write
    /// \param data - value to write\n
    /// Does change the direction of the port pin
    void inline static writeBit(Port &Port, const uint8_t &bit, const bool &data) {
        bitSet(*Port.DirectionReg, bit);                // set direction to output
        bitWrite(*Port.DataWriteReg, bit, data);        // write value
    }
}