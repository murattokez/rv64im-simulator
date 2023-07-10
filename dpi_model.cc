#include "cpu.hh"
#include "dpiheader.hh"


using namespace std;

decoded_instruction_t decode_inst(predecoded_instr_t predecoded_inst)
/*opcode'larına gore if else'ler ile sınıflandırıcam. bazısının func7si var
bazısının immediate'i falan*/
{
    /**/
    
    
    decoded_instruction_t decoded_instr;
    //opcode ve func3 hepsi icin ortak
    decoded_instr.opcode=(predecoded_inst.instr>>(0))&0b1111111; //0-6. bitler
    decoded_instr.func3=(predecoded_inst.instr>>(12))&0b111; //12-14 bitler
    if(decoded_instr.opcode==LUI){//LUI    
        decoded_instr.rd=(predecoded_inst.instr>>(7))&0b11111;
        decoded_instr.immed=(int64_t)(int32_t)(predecoded_inst.instr>>(12))&0b11111111111111111111; 
        
        decoded_instr.is_lui=1;
    }

    else if(decoded_instr.opcode==AUIPC){
        decoded_instr.rd=(predecoded_inst.instr>>(7))&0b11111;
        decoded_instr.immed=(int64_t)(int32_t)(predecoded_inst.instr & 0xfffff999);
        decoded_instr.is_auipc=1;
    }

    else if(decoded_instr.opcode==JAL){
        decoded_instr.rd=(predecoded_inst.instr>>(7))&0b11111;
        decoded_instr.immed=(uint64_t)((int64_t)(int32_t)(predecoded_inst.instr & 0x80000000) >> 11)
                            | (predecoded_inst.instr & 0xff000) // imm[19:12]
                            | ((predecoded_inst.instr >> 9) & 0x800) // imm[11]
                            | ((predecoded_inst.instr >> 20) & 0x7fe); // imm[10:1]
        decoded_instr.is_jal=1;
        
    }

    else if(decoded_instr.opcode==JALR){
        decoded_instr.rd=(predecoded_inst.instr>>(7))&0b11111;
        decoded_instr.rs1=(predecoded_inst.instr>>(15))&0b11111; //15-19 arası
        decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr& 0xfff00000)) >> 20;
        decoded_instr.is_jalr=1;

    }

    else if(decoded_instr.opcode==BEQ){
        decoded_instr.rs1=(predecoded_inst.instr>>(15))&0b11111; //15-19 arası
        decoded_instr.rs2=(predecoded_inst.instr>>(20))&0b11111; //20-24 arası
        decoded_instr.immed=((int64_t)(int32_t)(predecoded_inst.instr & 0x80000000) >> 19)
                            | ((predecoded_inst.instr & 0x80) << 4) // imm[11]
                            | ((predecoded_inst.instr >> 20) & 0x7e0) // imm[10:5]
                            | ((predecoded_inst.instr >> 7) & 0x1e); // imm[4:1]

        if(decoded_instr.func3==0b000) {decoded_instr.is_beq=1;}
        else if(decoded_instr.func3==0b001) {decoded_instr.is_bne=1;}
        else if(decoded_instr.func3==0b100) {decoded_instr.is_blt=1;}
        else if(decoded_instr.func3==0b101) {decoded_instr.is_bge=1;}
        else if(decoded_instr.func3==0b110) {decoded_instr.is_bltu=1;}
        else if(decoded_instr.func3==0b111) {decoded_instr.is_bgeu=1;}
    }

    else if(decoded_instr.opcode==R_format){
        decoded_instr.rs1=(predecoded_inst.instr>>(15))&0b11111; //15-19 arası
        decoded_instr.rs2=(predecoded_inst.instr>>(20))&0b11111; //20-24 arası
        decoded_instr.rd=(predecoded_inst.instr>>(7))&0b11111;
        decoded_instr.func7=(predecoded_inst.instr>>(25))&0b11111; //25-31

        if((decoded_instr.func3==0b000) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_add=1;}
        if((decoded_instr.func3==0b000) && (decoded_instr.func7==0b0100000)) {decoded_instr.is_sub=1;}
        if((decoded_instr.func3==0b001) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_sll=1;}
        if((decoded_instr.func3==0b010) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_slt=1;}
        if((decoded_instr.func3==0b011) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_sltu=1;}
        if((decoded_instr.func3==0b100) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_xor=1;}
        if((decoded_instr.func3==0b101) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_srl=1;}
        if((decoded_instr.func3==0b101) && (decoded_instr.func7==0b0100000)) {decoded_instr.is_sra=1;}
        if((decoded_instr.func3==0b110) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_or=1;}
        if((decoded_instr.func3==0b111) && (decoded_instr.func7==0b0000000)) {decoded_instr.is_and=1;}
         
        
    
    
    }

    else if(decoded_instr.opcode==I_format){
        decoded_instr.rs1=(predecoded_inst.instr>>(15))&0b11111; //15-19 arası
        decoded_instr.rd=(predecoded_inst.instr>>(7))&0b11111;
        
        if((decoded_instr.func3==0b000)) {
            decoded_instr.is_addi=1;
            decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr& 0xfff00000)) >> 20;
        }
        else if((decoded_instr.func3==0b010)) {
            decoded_instr.is_slti=1;
            decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr& 0xfff00000)) >> 20;
        }
        else if((decoded_instr.func3==0b011)) {
            decoded_instr.is_sltiu=1;
            decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr& 0xfff00000)) >> 20;    
        }
        else if((decoded_instr.func3==0b100)) {
            decoded_instr.is_xori=1;
            decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr& 0xfff00000)) >> 20;
        }
        else if((decoded_instr.func3==0b110)) {
            decoded_instr.is_ori=1;
            decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr& 0xfff00000)) >> 20;    
        }
        else if((decoded_instr.func3==0b111)) {
            decoded_instr.is_andi=1;
            decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr& 0xfff00000)) >> 20;    
        }

        else if((decoded_instr.func3==0b001)) { //shamt grubu
            decoded_instr.is_slli=1;
            decoded_instr.immed=(int32_t) (predecoded_inst.instr>>(20))&0b11111; //20-24 arası;    
        }
        else if((decoded_instr.func3==0b101)) { //shamt grubu
            decoded_instr.func7=(predecoded_inst.instr>>(25))&0b11111; //func3:101 de ortaklık var func7'den anlicaz
            if((decoded_instr.func7==0b0000000)){
                decoded_instr.is_srli=1;
                decoded_instr.immed=(int32_t) (predecoded_inst.instr>>(20))&0b11111; //20-24 arası;    
            }
            else if((decoded_instr.func7==0b0100000)) { //shamt grubu
                decoded_instr.is_srai=1;
                decoded_instr.immed=(int32_t) (predecoded_inst.instr>>(20))&0b11111; //20-24 arası;    
        }
            
        }
       
    }

    else if(decoded_instr.opcode==LOAD){
        decoded_instr.rs1=(predecoded_inst.instr>>(15))&0b11111; //15-19 arası
        decoded_instr.rd=(predecoded_inst.instr>>(7))&0b11111;
        decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr & 0xfff00000)) >> 20; 
    
        if(decoded_instr.func3==0b000) {decoded_instr.is_lb=1;}
        else if(decoded_instr.func3==0b001) {decoded_instr.is_lh=1;}
        else if(decoded_instr.func3==0b010) {decoded_instr.is_lw=1;}
        else if(decoded_instr.func3==0b100) {decoded_instr.is_lbu=1;}
        else if(decoded_instr.func3==0b101) {decoded_instr.is_lhu=1;}

    }

    else if(decoded_instr.opcode==STORE){
        decoded_instr.rs1 = (predecoded_inst.instr>>(15))&0b11111; //15-19 arası
        decoded_instr.rs2 = (predecoded_inst.instr>>(20))&0b11111; //20-24 arası        decoded_instr.immed=((int64_t)(int32_t) (predecoded_inst.instr & 0xfff00000)) >> 20; 
        decoded_instr.immed = ((int64_t)(int32_t)(predecoded_inst.instr & 0xfe000000) >> 20)
                                | ((predecoded_inst.instr >> 7) & 0x1f);  

        if(decoded_instr.func3==0b000) {decoded_instr.is_sb=1;}
        else if(decoded_instr.func3==0b001) {decoded_instr.is_sh=1;}
        else if(decoded_instr.func3==0b010) {decoded_instr.is_sw=1;}
        
    }

    
    

    return decoded_instr;
    
}


decoded_instruction_M_extens_t decode_inst_M_extens(predecoded_instr_t predecoded_inst)
/*opcode'larına gore if else'ler ile sınıflandırıcam. bazısının func7si var
bazısının immediate'i falan*/
{
    /**/
    
    
    decoded_instruction_M_extens_t decoded_instr_M_extens;
    //opcode ve func3 hepsi icin ortak
    decoded_instr_M_extens.opcode=(predecoded_inst.instr>>(0))&0b1111111; //0-6. bitler
    decoded_instr_M_extens.func3=(predecoded_inst.instr>>(12))&0b111; //12-14 bitler
    decoded_instr_M_extens.rs1=(predecoded_inst.instr>>(15))&0b11111; //15-19 arası
    decoded_instr_M_extens.rs2=(predecoded_inst.instr>>(20))&0b11111; //20-24 arası
    decoded_instr_M_extens.rd=(predecoded_inst.instr>>(7))&0b11111;
    decoded_instr_M_extens.func7=(predecoded_inst.instr>>(25))&0b11111; //25-31

    if((decoded_instr_M_extens.func3==0b000) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_mul=1;}
    if((decoded_instr_M_extens.func3==0b001) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_mulh=1;}
    if((decoded_instr_M_extens.func3==0b010) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_mulhsu=1;}
    if((decoded_instr_M_extens.func3==0b011) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_mulhu=1;}
    if((decoded_instr_M_extens.func3==0b100) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_div=1;}
    if((decoded_instr_M_extens.func3==0b101) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_divu=1;}
    if((decoded_instr_M_extens.func3==0b110) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_rem=1;}
    if((decoded_instr_M_extens.func3==0b111) && (decoded_instr_M_extens.func7==0b0000001)) {decoded_instr_M_extens.is_remu=1;}
         

    return decoded_instr_M_extens;
    
}


void CPU::exec(decoded_instruction_t instr, decoded_instruction_M_extens_t m_instr) {
    
    if(instr.is_add){ //ADD
        this->regs[instr.rd] =(uint64_t) ((int64_t)this->regs[instr.rs1] + (int64_t)this->regs[instr.rs2]);
        cout<<"add x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }  

    else if(instr.is_lui){ //LUI
        this->regs[instr.rd] =(uint64_t)(int64_t)(instr.immed<<12);
        std::cout<<"lui x"<<instr.rd<<std::hex<<", 0x"<<instr.immed<<std::dec<<std::endl;
    }  

    else if(instr.is_auipc){ //auipc
        this->regs[instr.rd] =((int64_t) this->pc + (int64_t) instr.immed) - 4;
        std::cout<<"auipc x"<<instr.rd<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    }  

    else if(instr.is_jal){ //JAL
        this->regs[instr.rd]=this->pc+4;
        this->pc = this->pc + (int64_t) instr.immed;
        std::cout<<"jal x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    }  

    else if(instr.is_jalr){ //JALR
        uint64_t temp = this->pc;
        this->pc = (this->regs[instr.rs1] + (int64_t) instr.immed) & 0xfffffffe;
        this->regs[instr.rd] = temp;
        std::cout<<"jalr x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    } 

    else if(instr.is_beq){ //BEQ
        if ((int64_t) this->regs[instr.rs1] == (int64_t) this->regs[instr.rs2])
            this->pc = (this->pc + (int64_t) instr.immed);

        std::cout<<"beq x"<<instr.rs1<<",x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;

    } 
    
    else if(instr.is_bne){ //BNE
        if ((int64_t) this->regs[instr.rs1] != (int64_t) this->regs[instr.rs2])
            this->pc = (this->pc + (int64_t) instr.immed);
        
        std::cout<<"bne x"<<instr.rs1<<",x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    } 

    else if(instr.is_blt){ //BLT
        if ((int64_t) this->regs[instr.rs1] < (int64_t) this->regs[instr.rs2])
            this->pc = (this->pc + (int64_t) instr.immed);
    
        std::cout<<"blt x"<<instr.rs1<<",x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    } 

    else if(instr.is_bge){ //BGE
        if ((int64_t) this->regs[instr.rs1] >= (int64_t) this->regs[instr.rs2])
            this->pc = (this->pc + (int64_t) instr.immed);
    
        std::cout<<"bge x"<<instr.rs1<<",x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    } 

    else if(instr.is_bltu){ //BLTU
        if (this->regs[instr.rs1] < this->regs[instr.rs2])
            this->pc = (this->pc + (int64_t) instr.immed);
    
        std::cout<<"bltu x"<<instr.rs1<<",x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    } 

    else if(instr.is_bgeu){ //BGEU
        if (this->regs[instr.rs1] >= this->regs[instr.rs2])
            this->pc = ((int64_t) this->pc + (int64_t) instr.immed);
    
        std::cout<<"bgeu x"<<instr.rs1<<",x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    } 



    else if(instr.is_lb){ //lb
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->regs[instr.rd] = (int64_t)(int8_t) this->dmem.lb(addr);

        std::cout<<"lb x"<<instr.rd<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;
    } 

    else if(instr.is_lh){ //lh
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->regs[instr.rd] = (int64_t)(int16_t) this->dmem.lh(addr);

        std::cout<<"lh x"<<instr.rd<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;
       
    } 

    else if(instr.is_lw){ //lw
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->regs[instr.rd] = (int64_t)(int32_t) this->dmem.lw(addr);
        
        std::cout<<"lw x"<<instr.rd<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;
    } 

    else if(instr.is_lbu){ //lbu
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->regs[instr.rd] = this->dmem.lw(addr);
        
        std::cout<<"lbu x"<<instr.rd<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;
    }

    else if(instr.is_lhu){ //lhu
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->regs[instr.rd] = this->dmem.lw(addr);
        
        std::cout<<"lhu x"<<instr.rd<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;
    } 

    else if(instr.is_sb){ //sb
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->dmem.sb(addr,  this->regs[instr.rs2]);

        std::cout<<"sb x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;
    }

    else if(instr.is_sh){ //sh
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->dmem.sh(addr,  this->regs[instr.rs2]);
    
        std::cout<<"sh x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;    
    }

    else if(instr.is_sw){ //sw
        uint64_t addr = (this->regs[instr.rs1] + (int64_t) instr.immed);
        this->dmem.sw(addr, this->regs[instr.rs2]);

        std::cout<<"sw x"<<instr.rs2<<", 0x"<<std::hex<<instr.immed<<std::dec<<"(x"<<instr.rs1<<")"<<std::endl;
    }

    else if(instr.is_addi){
        this->regs[instr.rd] = this->regs[instr.rs1] + (int64_t) instr.immed;

        std::cout<<"addi x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    }

    else if(instr.is_slli){
        this->regs[instr.rd] = this->regs[instr.rs1] << instr.immed;
    
        std::cout<<"slli x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;
    }

    else if(instr.is_slti){
        this->regs[instr.rd] = (this->regs[instr.rs1] < (int64_t) instr.immed) ? 1 : 0;
    
        std::cout<<"slti x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    }

    else if(instr.is_sltiu){
        this->regs[instr.rd] = (this->regs[instr.rs1] > instr.immed) ? 1 : 0; 
    
        std::cout<<"sltiu x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    }

    else if(instr.is_xori){
        this->regs[instr.rd] = this->regs[instr.rs1] ^ instr.immed;
    
        std::cout<<"xori x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    }

    else if(instr.is_srli){
        this->regs[instr.rd] = this->regs[instr.rs1] >> instr.immed;

        std::cout<<"srli x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    }

    else if(instr.is_srai){
        this->regs[instr.rd] = (int32_t) this->regs[instr.rs1] >> instr.immed;

        std::cout<<"srli x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    }

    else if(instr.is_ori){
        this->regs[instr.rd] = this->regs[instr.rs1] | instr.immed;

        std::cout<<"srli x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    }

    else if(instr.is_andi){
        this->regs[instr.rd] = this->regs[instr.rs1] & instr.immed;

        std::cout<<"srli x"<<instr.rd<<",x"<<instr.rs1<<", 0x"<<std::hex<<instr.immed<<std::dec<<std::endl;    
    }

    else if(instr.is_sub){
        this->regs[instr.rd] = (uint64_t) ((int64_t) this->regs[instr.rs1] - (int64_t) this->regs[instr.rs2]);

        cout<<"sub x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_sll){
        this->regs[instr.rd] = (this->regs[instr.rs1] << (int64_t) this->regs[instr.rs2]);

        cout<<"sll x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_slt){
        this->regs[instr.rd] = (this->regs[instr.rs1] < (int64_t) this->regs[instr.rs2]) ? 1 : 0;

        cout<<"slt x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_sltu){
        this->regs[instr.rd] = (this->regs[instr.rs1] < this->regs[instr.rs2]) ? 1 : 0;

        cout<<"sltu x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_xor){
        this->regs[instr.rd] =  (this->regs[instr.rs1] ^ this->regs[instr.rs2]);

        cout<<"xor x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_srl){
        this->regs[instr.rd] = (this->regs[instr.rs1] >> this->regs[instr.rs2]);

        cout<<"srl x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_sra){
        this->regs[instr.rd] = ( (int32_t) this->regs[instr.rs1] >> (int64_t) this->regs[instr.rs2]);

        cout<<"sra x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_or){
        this->regs[instr.rd] = (this->regs[instr.rs1] | this->regs[instr.rs2]);

        cout<<"or x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    else if(instr.is_and){
        this->regs[instr.rd] = (this->regs[instr.rs1] & this->regs[instr.rs2]);

        cout<<"and x"<<instr.rd<<",x"<<instr.rs1<<",x"<<instr.rs2<<endl; //debug
    }

    /**************M extension*************/

    else if(m_instr.is_mul){ //MUL
        this->regs[m_instr.rd] = ((int32_t)this->regs[m_instr.rs1] * (int32_t)this->regs[m_instr.rs2]);
        
        cout<<"mul x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug
    }   

    else if(m_instr.is_mulh){ //MULH
        this->regs[m_instr.rd] = (uint64_t) ((int32_t)this->regs[m_instr.rs1] * (int32_t)this->regs[m_instr.rs2]) >> 32;
        
        cout<<"mulh x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug
    }

    else if(m_instr.is_mulhu){ //MULHU
        this->regs[m_instr.rd] = (uint64_t) (this->regs[m_instr.rs1] * this->regs[m_instr.rs2]) >> 32;
        
        cout<<"mulhu x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug
    }

    else if(m_instr.is_mulhsu){ //MULHSU
        this->regs[m_instr.rd] = (uint64_t) ((int32_t)this->regs[m_instr.rs1] * this->regs[m_instr.rs2]) >> 32;
        
        cout<<"mulhsu x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug
    }

    else if(m_instr.is_div){ //DIV
        const int32_t dividend = (int32_t) this->regs[m_instr.rs1];
        const int32_t divisor = (int32_t) this->regs[m_instr.rs2];

        this->regs[m_instr.rd] = !divisor ? ~0U
                                : (divisor == -1 && this->regs[m_instr.rs1] == 0x80000000U)
                                    ? this->regs[m_instr.rs1] /*overflow*/
                                    : (unsigned int) (dividend / divisor);


        cout<<"div x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug
    }

    else if(m_instr.is_divu){
        const uint32_t dividend = this->regs[m_instr.rs1];
        const uint32_t divisor = this->regs[m_instr.rs2];

        this->regs[m_instr.rd] = !divisor ? ~0U : dividend / divisor;

        cout<<"divu x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug
    }

    else if(m_instr.is_rem){
        const int32_t dividend = this->regs[m_instr.rs1];
        const int32_t divisor = this->regs[m_instr.rs2];

        this->regs[m_instr.rd] = !divisor ? dividend
                    : (divisor == -1 && this->regs[m_instr.rs1] == 0x80000000U)
                        ? 0 /* overflow */
                        : (dividend % divisor);

        cout<<"rem x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug

    }

    else if(m_instr.is_remu){
        const uint32_t dividend = this->regs[m_instr.rs1];
        const uint32_t divisor = this->regs[m_instr.rs2];

        this->regs[m_instr.rd] = !divisor ? dividend : dividend % divisor;

        cout<<"remu x"<<m_instr.rd<<",x"<<m_instr.rs1<<",x"<<m_instr.rs2<<endl; //debug
    }


}



void dpi_model(instr_packet_t* instructions, execute_output_t* output)
{
    
    
    output->ex_mispred_i = 0;//ilk deger

    CPU cpu;
    
    

    decoded_instruction_t decoded_instructions[4]; // 4 adet çözülecek instr var

    decoded_instruction_M_extens_t decoded_instructions_M_extens[4];
    

    for(int i = 0; i < 4; ++i){

        
        // systemverilog dosyasındaki fetchtten gelen current pc ile CPU objesinin pc'si set edilir.
        cpu.set_pc(instructions->inst_pkg[i].current_pc); //eger burdan gelen pc ile branch sonucu hesaplanan pc eşleşmezse mispredict olacak
        output->ex_pc_i=cpu.get_pc(); 
        

        decoded_instructions[i] = decode_inst(instructions->inst_pkg[i]);
        decoded_instructions_M_extens[i] = decode_inst_M_extens(instructions->inst_pkg[i]);


        cout<<"********"<<i<<"**********\n"<<endl;

        if(decoded_instructions[i].opcode==BEQ){
            output->ex_branch_i=1;
        }
        else if(decoded_instructions[i].opcode==JAL || decoded_instructions[i].opcode==JALR){
            output->ex_jump_i=1;
        }
        else{
            output->ex_branch_i=0;
            output->ex_jump_i=0;
        }
        
        
        cpu.exec(decoded_instructions[i], decoded_instructions_M_extens[i]); //calisiyor. branch gelirse cpu classındaki pc'yi degistirecek

        output->ex_target_pc_i=cpu.get_pc(); //hesaplanan yeni pc target pc'ye veirlir.


        if(instructions->inst_pkg[i].if_jumps_pc==output->ex_target_pc_i){ // eger fetch'den gelen pc ile execute sonucu hesaplanan pc esit olmazsa mispredict
            output->ex_mispred_i=0;
        }
        else{
            output->ex_mispred_i=1;

        }

        cout<<"mispredict singal:"<<output->ex_mispred_i<<endl;

        
    }
    /*
    decode edilen instructionlar sırayla execute edilip load store ise dmem-rf arasında okuma-yazma,
    aritmatik işlem ise rf'den okuma ve yazma vs. yapılacak. branch, jal ya da jalr ise next pc hesaplanacak daha sonra gelen 
    instruction valid ise ve adresi olması gerekenden farklı ise misprediction üretilecek. (bkz. execute_output_t)
    */

   for(int i=0;i<32;i++){ //regleri bastırma
        std::cout<<"r["<<i<<"]:"<<cpu.regs[i]<<"\n"<<std::endl;
        
    };
    
    std::cout<<std::hex<<"pc:"<<cpu.get_pc()<<"\n"<<std::endl;

   
   
   return;

}






int main(){



    execute_output_t* output = new execute_output_t();


    instr_packet_t* y = new instr_packet_t(); //new anahtar kelimesiyle bellekte yer ayırmamız
    
    //deneme_instr=dpi_model(y);
  
    //cout<<"deger:" << deneme_instr.ex_branch_i << endl;

    predecoded_instr_t instr1;

    //CPU cpu;
    
    y->inst_pkg[0].instr=0b00000110111100000000001000100011; //sb x15, 100(x0) #memory'nin 100. slotuna x15+x0'ı yaz
    y->inst_pkg[0].current_pc=0x00400000;
    y->inst_pkg[0].pred_type.is_pc_changer=0;
    y->inst_pkg[0].if_jumps_pc=0x00400000; //if jumps pcnin degeri mantiken eger is_pc_changer varken onemli ama ben yine de koydum. yani is_pc_changer 0 ken if_jumps_pc=current_pc

    y->inst_pkg[1].instr=0b00000110010000000000100000000011; //lb x16, 100(x0) #memory'nin 100+x0 slotundan datayı cekip x16'ya yaz 
    y->inst_pkg[1].current_pc=0x00400004;
    y->inst_pkg[1].pred_type.is_pc_changer=0;
    y->inst_pkg[1].if_jumps_pc=0x00400004;

    y->inst_pkg[2].instr=0b11111110101001100001110011100011; //bne x12,x10, label(0x0040000)
    y->inst_pkg[2].current_pc=0x00400008;
    y->inst_pkg[2].pred_type.is_pc_changer=1; //predecodde instr'in branch oldugu anlasildi
    y->inst_pkg[2].if_jumps_pc=0x00400000; //tahmin edilen atlanacka adres

    y->inst_pkg[3].instr=0b00000010010111100111001110110011; //add x7,x15,x23
    y->inst_pkg[3].current_pc=0x00400000;
    y->inst_pkg[3].pred_type.is_pc_changer=0;
    y->inst_pkg[3].if_jumps_pc=0x00400000;
    
    decoded_instruction_t sonuc;
    dpi_model(y, output);

    //std::bitset<32> rs1(y.rs2); // bit dizisi olarak sakla
    //cout<<"*****\n"<<rs1<<endl;

    //cout<<deneme_instr.ex_target_pc_i<<endl;





    return 0;
}
