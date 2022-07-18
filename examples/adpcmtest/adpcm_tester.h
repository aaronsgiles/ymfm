// cl /EHsc mem_rw13.cpp ..\..\src\ymfm_adpcm.cpp ..\..\src\ymfm_opn.cpp ..\..\src\ymfm_ssg.cpp

#include "../../src/ymfm_opn.h"

using namespace ymfm;

#ifndef PRINT_READS
#define PRINT_READS (false)
#endif

#ifndef PRINT_WRITES
#define PRINT_WRITES (false)
#endif

class adpcm_tester : public ymfm_interface
{
public:
    adpcm_tester() :
        m_chip(*this)
    {
    }

    adpcm_tester &out(uint16_t reg, uint8_t data)
    {
        printf("O%02X:%02X ", reg, data);
        m_chip.write_address_hi(reg);
        m_chip.write_data_hi(data);
        return *this;
    }

    adpcm_tester &inp(uint16_t reg)
    {
        m_chip.write_address_hi(reg);
        uint8_t result = m_chip.read_data_hi();
        printf("I%02X:%02X ", reg, result);
        return *this;
    }

    adpcm_tester &stat()
    {
        printf("S%02X ", m_chip.read_status_hi());
        return *this;
    }

    adpcm_tester &nl()
    {
        printf("\n");
        return *this;
    }

    adpcm_tester &msg(char const *string)
    {
        printf("\n%s\n", string);
        return *this;
    }

    adpcm_tester &out0(uint16_t reg, uint8_t data)
    {
        printf("O%02X:%02X ", reg, data);
        m_chip.write_address(reg);
        m_chip.write_data(data);
        return *this;
    }

    adpcm_tester &inp0(uint16_t reg)
    {
        m_chip.write_address(reg);
        uint8_t result = m_chip.read_data();
        printf("I%02X:%02X ", reg, result);
        return *this;
    }

    adpcm_tester &stat0()
    {
        printf("S%02X ", m_chip.read_status());
        return *this;
    }

    adpcm_tester &poll_stat(double wait)
    {
        return *this;
    }

    adpcm_tester &mwr(uint8_t data, uint16_t count)
    {
        for (uint16_t i = 0; i < count; i++)
        {
            m_chip.write_address_hi(8);
            m_chip.write_data_hi(data);
            uint8_t stat = m_chip.read_status_hi();
            m_chip.write_address_hi(0x10);
            m_chip.write_data_hi(0x80);
            printf("W%02X:%02X ", data, stat);
            data++;
        }
        return *this;
    }

    adpcm_tester &mrd(uint16_t count)
    {
        for (uint16_t i = 0; i < count; i++)
        {
            m_chip.write_address_hi(8);
            uint8_t data = m_chip.read_data_hi();
            uint8_t stat = m_chip.read_status_hi();
            m_chip.write_address_hi(0x10);
            m_chip.write_data_hi(0x80);
            printf("R%02X:%02X ", data, stat);
        }
        return *this;
    }

    adpcm_tester &reset()
    {
        return out(0x00, 0x01).out(0x00, 0x00).nl();
    }

    adpcm_tester &seq_mem_limit(uint16_t adr)
    {
        return out(0x0c, adr & 0xff).out(0x0d, (adr >> 8) & 0xff);
    }

    adpcm_tester &seq_mem_write(uint16_t start, uint16_t stop, uint16_t data, uint16_t count, char const *_msg)
    {
        msg(_msg);
        out(0x10, 0x00).out(0x10, 0x80);
        out(0x00, 0x60).out(0x01, 0x02);
        out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
        out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
        nl();
        mwr(data, count).nl();
        return out(0x00, 0x00).out(0x10, 0x80).nl();
    }

    adpcm_tester &seq_mem_read(uint16_t start, uint16_t stop, uint16_t count, char const *_msg)
    {
        msg(_msg);
        out(0x10, 0x00).out(0x10, 0x80);
        out(0x00, 0x20).out(0x01, 0x02);
        out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
        out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
        nl();
        mrd(count).nl();
        return out(0x00, 0x00).out(0x10, 0x80).nl();
    }

    adpcm_tester &seq_mem_fill(uint16_t start, uint16_t stop, uint16_t data, uint16_t count, char const *_msg)
    {
        msg(_msg);
        out(0x10, 0x00).out(0x10, 0x80);
        out(0x00, 0x60).out(0x01, 0x02);
        out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
        out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
        nl();
        for (uint16_t i = 0; i < count; i++)
        {
            if (i % 16 == 0)
                printf("\n%05x ", i);
            out(0x08, data);
        }
        return out(0x00, 0x00).out(0x10, 0x80).nl();
    }

    adpcm_tester &seq_mem_fill_pat(uint16_t start, uint16_t stop, uint8_t const *pat, uint16_t patlen, uint16_t count, char const *_msg)
    {
        msg(_msg);
        out(0x10, 0x00).out(0x10, 0x80);
        out(0x00, 0x60).out(0x01, 0x02);
        out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
        out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
        nl();
        for (uint16_t i = 0; i < count; i++)
        {
            if (i % 16 == 0)
                printf("\n%05x ", i);
            out(0x08, pat[i % patlen]);
        }
        return out(0x00, 0x00).out(0x10, 0x80).nl();
    }

    adpcm_tester &seq_mem_fill_random(uint16_t start, uint16_t stop, uint16_t count, char const *_msg)
    {
        msg(_msg);
        out(0x10, 0x00).out(0x10, 0x80);
        out(0x00, 0x60).out(0x01, 0x02);
        out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
        out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
        nl();
        for (uint16_t i = 0; i < count; i++)
        {
            if (i % 16 == 0)
                printf("\n%05x ", i);
            out(0x08, rand() & 0xff);
        }
        return out(0x00, 0x00).out(0x10, 0x80).nl();
    }

    adpcm_tester &seq_play(uint16_t start, uint16_t stop, uint16_t deltan, uint16_t volume, bool repeat, char const *_msg)
    {
        msg(_msg);
        out(0x10, 0x00).out(0x10, 0x80);
        out(0x00, 0x20);
        out(0x01, 0xc2);
        out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
        out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
        out(0x09, deltan & 0xff).out(0x0a, (deltan >> 8) & 0xff);
        out(0x0b, volume);
        if (repeat)
            out(0x00, 0xb0);
        else
            out(0x00, 0xa0);
        return nl();
    }

    adpcm_tester &seq_stop(char const *_msg)
    {
        msg(_msg);
        out(0x00, 0xa1).poll_stat(0.1);
        return out(0x00, 0x00).poll_stat(0.1).out(0x10, 0x80).poll_stat(0.1).nl();
    }

    virtual uint8_t ymfm_external_read(access_class type, uint32_t address) override
    {
		if (PRINT_READS)
			printf("(Read %X=%X) ", address, m_memory[address % sizeof(m_memory)]);
        return m_memory[address % sizeof(m_memory)];
    }

    virtual void ymfm_external_write(access_class type, uint32_t address, uint8_t data) override
    {
		if (PRINT_WRITES)
			printf("(Write %X=%X) ", address, data);
        m_memory[address % sizeof(m_memory)] = data;
    }

    ym2608 m_chip;
    uint8_t m_memory[0x40000];
};
