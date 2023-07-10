#include <cstdint>
#include <iostream>
//#include "cpu.hh"

class Memory {
public:
    Memory(uint64_t size) {
        // Bellek oluşturulur
        mem = new uint64_t[size/8];
        mem_size = size;
        for (int i = 0; i < size/8; i++) {
            mem[i] = 0;
        }
    }

    // Bellekteki bir adresi 64 bit olarak yükle
    uint64_t ld(uint64_t addr) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return 0;
        }
        return mem[addr/8];
    }

    // Bellekteki bir adresi 32 bit olarak yükle
    uint32_t lw(uint64_t addr) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return 0;
        }
        return (uint32_t) mem[addr/8];
    }

    // Bellekteki bir adresi 16 bit olarak yükle
    uint16_t lh(uint64_t addr) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return 0;
        }
        uint16_t* p = (uint16_t*)&mem[addr/8];
        return *p + ((addr % 8 ) ? 0 : (*p >> 8));
    }

    // Bellekteki bir adresi 8 bit olarak yükle
    uint8_t lb(uint64_t addr) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return 0;
        }
        uint8_t* p = (uint8_t*)&mem[addr/8];
        return *(p + (addr % 8));
    }

    // Bellekteki bir adrese 64 bit değeri yaz
    void sd(uint64_t addr, uint64_t value) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return ;
        }
        mem[addr/8] = value;
    }

    // Bellekteki bir adrese 32 bit değeri yaz
    void sw(uint64_t addr, uint32_t value) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return ;
        }
        mem[addr/8] = value;
    }

    // Bellekteki bir adrese 16 bit değeri yaz
    void sh(uint64_t addr, uint16_t value) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return ;
        }
        uint16_t* p = (uint16_t*)&mem[addr/8];
       
        *p = value;
    }

    // Bellekteki bir adrese 8 bit değeri yaz
    void sb(uint64_t addr, uint8_t value) {
        if (addr >= mem_size) {
            // Bellek dışına erişim hatası
            // Burada istenilen bir hata kodu veya işlem yapılabilir
            return;
        }
        uint8_t* p = (uint8_t*)&mem[addr/8];
        *(p + (addr % 8)) = value;
    }

    private:
        uint64_t* mem; // Bellek
        uint64_t mem_size; // Bellek boyutu
};



