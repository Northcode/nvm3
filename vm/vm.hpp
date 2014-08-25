#include "main.hpp"

#ifndef VM_H
#define VM_H

class vm
{
  std::vector<std::shared_ptr<device>> devices;

public:
  byte A,B,C,D;
  dword AX,BX,CX,DX;

  dword IP,SP,BP;
  dword DA,DB,DC,DD;
  dword PD,PT;

  bool EQ,GT,RN,PE,MD;

  bool FAULT,DOUBLE_FAULT;

  dword IVT,FAULT_ADDR;

  std::shared_ptr<cpu> _cpu;
  std::shared_ptr<memory> _ram;
  std::shared_ptr<databridge> _databridge;
  std::shared_ptr<psu> _psu;

  std::string bios_file_path;
  std::vector<std::string> mount_paths;

  vm();

  std::shared_ptr<device> get_device(dword dev);
  void add_device(std::shared_ptr<device> dev);

  void power();
  void tick();
  void reset();

  void poweroff_signal(int data);

  void force_interupt(byte interupt);
  void force_interupt(interupts interupt);
  void iret();

  void push_registers();
  void pop_registers();
  void dump_registers();
  void clear_registers();

  void load_program(dword address, std::vector<byte> program);

  void execute(dword address);

  void call(dword address);
  void ret();

  void push(byte value);
  void push(dword value);
  byte pop_b();
  dword pop_dw();

  void set_register_b(register_e reg, byte b);
  void set_register_dw(register_e reg, dword v);
  bool get_register_t(register_e reg);
  byte get_register_b(register_e reg);
  dword get_register_dw(register_e reg);

  byte get_register_size(register_e reg);

  bool can_access_reg(register_e reg);

  dword get_address(mov_arg arg);
  dword get_address(dword address, addressing_mode mode);

  bool can_read(dword address);
  bool can_write(dword address);
};
#endif
