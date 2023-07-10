#ifndef CPU_HH
#define CPU_HH


#include "Memory.hh"
#include <bitset>
#include <iostream>

using namespace std;

#define R_format 0b0110011 //add,sub,sll,slt,sltu,xor,srl,sra,or,and
#define I_format 0b0010011 //addi,slti,sltiu,xori,ori,andi,slli,srli,srai,
#define LOAD 0b0000011 // lb,lh,lbu,lhu,
#define STORE 0b0100011 //sb,sh,
#define BEQ 0b1100011 //beq,bne,blt,bge,bltu,bgeu
#define LUI 0b0110111
#define AUIPC 0b0010111
#define JAL 0b1101111
#define JALR 0b1100111


typedef struct
{
    bool is_branch;
    bool is_j;          // jal or jalr.
    bool is_pc_changer; // branch or jal or jalr.
    bool is_jal;
    bool is_jalr;
    bool is_pop_ras;
    bool is_push_ras;
    bool is_other;
} predecoded_t; //predecode fetch icerisinde branch prediction'ın gerceklsmesi icin instrucion'ın branch olu olmauyacagını anlayacak



typedef struct
{
    uint32_t instr;
    uint64_t current_pc=0;
    uint32_t immediate; // its calculated for branch and jump instructions in fetch
    uint64_t if_jumps_pc;
    predecoded_t pred_type;
    bool is_compressed;
    bool valid;
} predecoded_instr_t;

typedef struct
{
    predecoded_instr_t inst_pkg[4]; //4 adet instr
} instr_packet_t;

typedef struct
{
    bool ex_branch_i;
    bool ex_jump_i;
    uint64_t ex_result_i;
    uint64_t ex_pc_i;
    uint64_t ex_target_pc_i;
    bool ex_mispred_i;
} execute_output_t;


typedef struct
{
    //immed, rs1, rs2 rd vs. değerleri olacak. decode_inst fonksiyonu ile return edilecek. 
    uint64_t immed=0;
    uint32_t rs1=0;
    uint32_t rs2=0;
    uint32_t rd=0;
    uint32_t opcode=0;
    uint32_t func3=0;
    uint32_t func7=0;

    
    bool is_lui=0;
    bool is_auipc=0;

    bool is_jal=0;
    bool is_jalr=0;
    bool is_beq=0;//beq,bne,blt,bge,bltu,bgeu
    bool is_bne=0;
    bool is_blt=0;
    bool is_bge=0;
    bool is_bltu=0;
    bool is_bgeu=0;
    
    bool is_add=0;
    bool is_sub=0;
    bool is_sll=0;
    bool is_slt=0;
    bool is_sltu=0;
    bool is_xor=0;
    bool is_srl=0;
    bool is_sra=0;
    bool is_or=0;
    bool is_and=0;

    bool is_addi=0;
    bool is_slti=0;
    bool is_sltiu=0;
    bool is_xori=0;
    bool is_ori=0;
    bool is_andi=0;
    bool is_slli=0;
    bool is_srli=0;
    bool is_srai=0;

    bool is_lb=0;
    bool is_lh=0;
    bool is_lw=0;
    bool is_lbu=0;
    bool is_lhu=0;

    bool is_sb=0;
    bool is_sh=0;
    bool is_sw=0;  

    
    //struct'ın limiti asıldıgı icin segmentaion fault hatası veriyor baska bir struct icinde kullanacagım
    
      

} decoded_instruction_t;

typedef struct
{
    uint32_t rs1=0;
    uint32_t rs2=0;
    uint32_t rd=0;
    uint32_t opcode=0;
    uint32_t func3=0;
    uint32_t func7=0;

    ////////32m
    bool is_mul=0;
    bool is_mulh=0;
    bool is_mulhsu=0;
    bool is_mulhu=0;
    bool is_div=0;
    bool is_divu=0;
    bool is_rem=0;
    bool is_remu=0;

} decoded_instruction_M_extens_t;

class CPU
{
   private:
        uint64_t dMem_size=1024; //data memory size
        uint64_t pc;
    
    public:
        
        uint64_t regs[32]; //registerlar
        //uint64_t pc;

        //memory.hh'dan dmem isimli bir obje yaptık ve constructo3 parametersini verdik
        CPU() : dmem(dMem_size/4) //byte addressable olması icin dörde böleriz.
        {
            
            for(int i=0;i<32;i++){ // constructor cpu: initialize reg file
                this->regs[i]=i;
            }

            this->pc=0;
            
        }

        Memory dmem;

        void exec(decoded_instruction_t instr, decoded_instruction_M_extens_t m_instr);
        
        uint64_t get_pc(){
            return pc;
        }

        void set_pc(uint64_t pc){
            this->pc=pc;
        }
        
        
} ;

void dpi_model(instr_packet_t* instructions, execute_output_t* output);





#endif
