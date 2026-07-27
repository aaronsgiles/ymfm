// cl /EHsc mem_rw15.cpp ..\..\src\ymfm_adpcm.cpp ..\..\src\ymfm_opn.cpp ..\..\src\ymfm_ssg.cpp

#include "adpcm_tester.h"

void test_mem_write(adpcm_tester &self, uint16_t start, uint16_t stop, uint8_t data, uint16_t count, char const *msg)
{
    int control1 = 0x70; // REPEAT(bit.4 = 1)
    self.msg(msg);
    self.out(0x10, 0x00).out(0x10, 0x80);
    self.out(0x00, control1).out(0x01, 0x02);
    self.out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
    self.out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
    self.nl();
    self.mwr(data, count).nl();
    self.out(0x00, 0x00).out(0x10, 0x80).nl();
}

int main(int argc, char *argv[])
{
    adpcm_tester rs;
	char tempbuf[100];

    rs.seq_mem_limit(0xffff);
    rs.reset();
	test_mem_write(rs, 0x0000, 0x0000, 0x00, 32, "1. WRITE ADDRESS 0000-0000 (00-1F)");

    int testno = 2;
    for (int i = 0; i < 4; i++)
    {
        rs.reset();
        sprintf(tempbuf, "%d. WRITE ADDRESS 0fff-0fff (40-5F...) (%dB)", testno, 32 + i);
        test_mem_write(rs, 0x0fff, 0x0fff, 0x40, 32 + i, tempbuf);
        testno += 1;
        rs.reset();
        sprintf(tempbuf, "%d. READ ADDRESS 0000-0000", testno);
        rs.seq_mem_read(0x0000, 0x0000, 34, tempbuf);
        testno += 1;
    }

    sprintf(tempbuf, "%d. READ ADDRESS 0fff-0fff", testno);
    rs.reset();
    rs.seq_mem_read(0x0fff, 0x0fff, 34, tempbuf);

    return 0;
}
