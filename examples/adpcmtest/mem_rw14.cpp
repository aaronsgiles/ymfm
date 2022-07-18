// cl /EHsc mem_rw14.cpp ..\..\src\ymfm_adpcm.cpp ..\..\src\ymfm_opn.cpp ..\..\src\ymfm_ssg.cpp

#include "adpcm_tester.h"

void test_seq_mem_read(adpcm_tester &self, uint16_t start, uint16_t stop, uint16_t count, char const *msg)
{
    int control1 = 0x30;
    self.msg(msg);
    self.out(0x10, 0x00).out(0x10, 0x80);
    self.out(0x00, control1).out(0x01, 0x02);
    self.out(0x02, start & 0xff).out(0x03, (start >> 8) & 0xff);
    self.out(0x04, stop & 0xff).out(0x05, (stop >> 8) & 0xff);
    self.nl();
    self.mrd(count).nl();
    self.out(0x00, 0x00).out(0x10, 0x80).nl();
}

int main(int argc, char *argv[])
{
    adpcm_tester rs;

    rs.reset();

    rs.seq_mem_limit(0xffff);
    rs.seq_mem_write(0x0000, 0x0001, 0x00, 64, "1. WRITE ADDRESS 0000-0001 (00-3F)");
    rs.seq_mem_write(0x0fff, 0x0fff, 0xA0, 32, "2. WRITE ADDRESS 0fff-0fff (A0-BF)");

    int testno = 3;
    static const uint8_t ivals[] = {1, 2, 3, 4, 31, 32, 33, 34, 35, 36, 37};
    for (int index = 0; index < sizeof(ivals); index++)
    {
        char tempbuf[100];
        int i = ivals[index];
        rs.reset();
        sprintf(tempbuf, "%d. READ ADDRESS 0000-0000 (%dB) (REPEAT=1)", testno, i);
        test_seq_mem_read(rs, 0x0000, 0x0000, i, tempbuf);
        testno += 1;
        rs.reset();
        sprintf(tempbuf, "%d. READ ADDRESS 0fff-0fff (DUMMY READ TEST)", testno);
        rs.seq_mem_read(0x0fff, 0x0fff, 32+2, tempbuf);
        testno += 1;
    }

    return 0;
}
