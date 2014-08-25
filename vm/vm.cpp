#include "main.hpp"

#ifndef VM_C
#define VM_C
vm::vm() {
  devices = std::vector<std::shared_ptr<device>>();
  mount_paths = std::vector<std::string>();

  //initialize default devices
  _cpu = std::make_shared<cpu>();
  _ram = std::make_shared<memory>();
  _databridge = std::make_shared<databridge>();
  _psu = std::make_shared<psu>();

  //set referances
  _cpu->machine = this;
  _ram->machine = this;
  _databridge->machine = this;
  _psu->machine = this;

  //add devices
  devices.push_back(_databridge);
  devices.push_back(_cpu);
  devices.push_back(_ram);
  devices.push_back(_psu);
}

std::shared_ptr<device> vm::get_device(dword dev) {
  return devices[dev];
}

void vm::add_device(std::shared_ptr<device> dev) {
  dev->machine = this;
  devices.push_back(dev);
}

void vm::push_registers() {
  push(A);push(B);push(C);push(D);
  push(AX);push(BX);push(CX);push(DX);
  push(IP);push(SP);push(BP);
  push(DA);push(DB);push(DC);push(DD);
  push((byte)EQ);push((byte)GT);push((byte)RN);push((byte)PE);push((byte)MD);
  push(PD);push(PT);
}

void vm::pop_registers() {
  PT = pop_dw(); PD = pop_dw();
  MD = (bool)pop_b();PE = (bool)pop_b();RN = (bool)pop_b();GT = (bool)pop_b();EQ = (bool)pop_b();
  DD = pop_dw();DC = pop_dw();DX = pop_dw();DA = pop_dw();
  BP = pop_dw();SP = pop_dw();IP = pop_dw();
  DX = pop_dw();CX = pop_dw();BX = pop_dw();AX = pop_dw();
  D = pop_b();C = pop_b();B = pop_b();A = pop_b();
}

void vm::dump_registers() {
  std::cout << "A: " << A << "(" << (int)A << ")" << " , B: " << B << "(" << (int)B << ")" << " , C: " << C << "(" << (int)C << ")" << " , D: " << D << "(" << (int)D << ")" << std::endl
  << "AX: " << AX << " , BX: " << BX << " , CX: " << CX << " , DX: " << DX << std::endl
  << "IP: " << IP << " , SP: " << SP << " , BP: " << BP << std::endl
  << "DA: " << DA << " , DB: " << DB << " , DC: " << DC << " , DD: " << DD << std::endl
  << "PD: " << PD << " , PT: " << PT << std::endl
  << "EQ: " << (EQ ? "Y" : "N") << " ,GT: " << (GT ? "Y" : "N") << " ,RN: " << (RN ? "Y" : "N") << " ,PE: " << (PE ? "Y" : "N") << " ,MD: " << (MD ? "Y" : "N") << std::endl;
}

void vm::clear_registers() {
  A = B = C = D = 0;
  AX = BX = CX = DX = IP = SP = BP = DA = DB = DC = DD = PD = PT = 0;
  EQ = GT = RN = MD = PE = false;
  FAULT = DOUBLE_FAULT = false;
}

void vm::force_interupt(byte interupt) {
  interupts i = (interupts)interupt;
  if(i == triple_fault) {
    //something really bad happened, reset the vm and give us a massive error
    std::cout << "ERROR: TRIPLE_FAULT! VM CANNOT CONTINUE!" << std::endl
    << "REGISTER DATA: " << std::endl;
    dump_registers();
    std::cout << "--------------" << std::endl << "STACK TRACE (top down):";
    while(SP > BP) {
      std::cout << (int)pop_b() << " , ";
    }
    std::cout << "--------------" << std::endl << "HIDDEN REGISTERS:";
    std::cout << "FAULT_ADDR: " << FAULT_ADDR << " , IVT: " << IVT << " , FAULT: " << (FAULT ? "YES" : "NO") << " , DOUBLE_FAULT: " << (DOUBLE_FAULT ? "YES" : "NO") << std::endl;
    std::cout << "--------------" << std::endl << "PRESS ANY KEY TO RESET";
    int c;
    std::cin >> c;
    reset();
  }

  if(DOUBLE_FAULT)
    force_interupt(triple_fault);

  if(FAULT) {
    DOUBLE_FAULT = true;
    force_interupt(double_fault);
  }

  if (i == div_by_zero || i == invalid_opcode || i == gp_fault || i == page_fault || i == stack_overflow || i == no_bios)
    FAULT = true;

  push(IP);
  push_registers();

  if(FAULT) {
    //information for the fault handler
    DA = FAULT_ADDR;
    DB = IP;
    DC = SP;
    DD = BP;
  }

  //all interupts are root mode
  MD = true;

  //JUMP!
  dword iaddr = _ram->read_dw(IVT + interupt * 4); //get address

  //preform last check to see this interupt isn't blank
  if(iaddr != 0)
    IP = iaddr; //JUMP!
  else {
    //this is an invalid interupt
    FAULT_ADDR = interupt;
    force_interupt(invalid_interupt);
  }
}

void vm::iret() {
  pop_registers();
  IP = pop_dw();
}

void vm::call(dword address) {
  if(BP == 0 || SP == 0)
    return;

  push(IP);
  push(BP);
  BP = SP - 1;

  IP = address;
}

void vm::ret() {
  if(BP == 0 || SP == 0)
    return;

  BP = pop_dw();
  IP = pop_dw();
}

void vm::push(byte data) {
  _ram->write(SP,data);
  SP++;
}

void vm::push(dword data) {
  _ram->write(SP,data);
  SP += 4;
}

byte vm::pop_b() {
  if(SP - 1 < BP) {
    force_interupt(stack_overflow);
    return 0;
  }
  SP--;
  return _ram->read_b(SP);
}

dword vm::pop_dw() {
  if(SP - 4 < BP) {
    force_interupt(stack_overflow);
    return 0;
  }
  SP -= 4;
  return _ram->read_dw(SP);
}

void vm::force_interupt(interupts interupt) {
  force_interupt((byte)interupt);
}

void vm::load_program(dword address, std::vector<byte> program) {
  if(!_ram->initialized)
    return;

  for(dword i = 0; i < program.size(); i++)
    _ram->write(i,program[i]);
}

void vm::execute(dword address) {
  IP = address;
  RN = true;

  while(RN) {
    tick();
  }
}

struct vm_dmp_regs_port : public port_c
{
  vm* m;

  vm_dmp_regs_port(vm* m) : port_c() {
    this->m = m;
  }

  void send(byte b) {
    m->dump_registers();
  }
};

void vm::power() {
  clear_registers();

  //check if bios is specified
  if(bios_file_path.empty())
    force_interupt(no_bios);

  //load bios
  std::vector<byte> bios = read_all_bytes(bios_file_path);
  load_program(0,bios);

  //power on default devices
  _databridge->power();
  _cpu->power();
  _ram->power();
  _psu->power();

  std::shared_ptr<vm_dmp_regs_port> dmp_reg_port = std::make_shared<vm_dmp_regs_port>(this);
  _databridge->register_port(1,dmp_reg_port);

  execute(0);
}

void vm::poweroff_signal(int data) {
  if(data > 0)
    RN = false;
}

void vm::tick() {
  for(auto dev : devices)
    dev->tick();
}

void vm::reset() {
  for(auto dev : devices)
    dev->reset();

  power();
}

void vm::set_register_b(register_e reg, byte b) {
  if(reg == register_e::A)
    A = b;
  else if(reg == register_e::B)
    B = b;
  else if(reg == register_e::C)
    C = b;
  else if(reg == register_e::D)
    D = b;
  else if(reg == register_e::EQ)
    EQ = (b != 0);
  else if(reg == register_e::GT)
    GT = (b != 0);
  else if (reg == register_e::PE)
    PE = (b != 0);
  else if (reg == register_e::MD)
    MD = (b != 0);
  else if (reg == register_e::RN)
    MD = (b != 0);
}

void vm::set_register_dw(register_e reg, dword b) {
  if(reg == register_e::AX)
    AX = b;
  else if (reg == register_e::BX)
    BX = b;
  else if (reg == register_e::CX)
    CX = b;
  else if (reg == register_e::DX)
    DX = b;
  else if (reg == register_e::IP)
    IP = b;
  else if (reg == register_e::SP)
    SP = b;
  else if (reg == register_e::BP)
    BP = b;
  else if (reg == register_e::DA)
    DA = b;
  else if (reg == register_e::DB)
    DB = b;
  else if (reg == register_e::DC)
    DC = b;
  else if (reg == register_e::DD)
    DD = b;
  else if (reg == register_e::PT)
    PT = b;
  else if (reg == register_e::PD)
    PD = b;
}

bool vm::get_register_t(register_e reg) {
  if(reg == register_e::EQ)
    return EQ;
  else if (reg == register_e::GT)
    return GT;
  else if (reg == register_e::PE)
    return PE;
  else if (reg == register_e::MD)
    return MD;
  else if (reg == register_e::RN)
    return RN;
}

byte vm::get_register_b(register_e reg) {
  if (reg == register_e::A)
    return A;
  else if (reg == register_e::B)
    return B;
  else if (reg == register_e::B)
    return C;
  else if (reg == register_e::B)
    return D;
}

dword vm::get_register_dw(register_e reg) {
  if(reg == register_e::AX)
    return AX;
  else if (reg == register_e::BX)
    return BX;
  else if (reg == register_e::CX)
    return CX;
  else if (reg == register_e::DX)
    return DX;
  else if (reg == register_e::IP)
    return IP;
  else if (reg == register_e::SP)
    return SP;
  else if (reg == register_e::BP)
    return BP;
  else if (reg == register_e::DA)
    return DA;
  else if (reg == register_e::DB)
    return DB;
  else if (reg == register_e::DC)
    return DC;
  else if (reg == register_e::DD)
    return DD;
  else if (reg == register_e::PD)
    return PD;
  else if (reg == register_e::PT)
    return PT;
}

bool vm::can_access_reg(register_e reg) {
  if (PE)
    if (reg == register_e::PE || reg == register_e::MD || reg == register_e::RN || reg == register_e::IP || reg == register_e::SP || reg == register_e::BP || reg == register_e::PD || reg == register_e::PT)
      return MD;
  return true;
}

byte vm::get_register_size(register_e reg) {
  if (reg == register_e::A || reg == register_e::B || reg == register_e::C || reg == register_e::D)
    return 8;
  else if (reg == register_e::EQ || reg == register_e::EQ || reg == register_e::GT || reg == register_e::RN || reg == register_e::PE || reg == register_e::MD)
    return 1;
  else
    return 32;
}

dword vm::get_address(dword address, addressing_mode mode) {
  dword addr = address;

  if (mode == addressing_mode::direct)
    addr = address;
  else if (mode == addressing_mode::IP_relative)
    addr = IP + address;
  else if (mode == addressing_mode::SP_relative)
    addr = SP + address;

  if(PE && PT != 0) {
    page_address addr;
    addr.raw = address;

    page_table pt;
    pt.raw = _ram->read_dw(PT + 4 * addr.page_table);

    dword page = _ram->read_dw(pt.address + 4 * addr.page);
    page_entry pe;
    pe.raw = page;
    address = pe.address + addr.offset;
  }

  return address;
}

dword vm::get_address(mov_arg arg) {
  if(arg.type == argument_type::registers)
    return get_register_dw((register_e)arg.data);
  else if (arg.type == argument_type::data_at_address)
    return _ram->read_dw(get_address(arg.data,arg.addr_mode));
  else if (arg.type == argument_type::address_of_register)
    return _ram->read_dw(get_address(get_register_dw((register_e)arg.data),arg.addr_mode));
  else if (arg.type == argument_type::value)
    return arg.data;
}

bool vm::can_read(dword address) {
  if(PE && PT != 0) {
    if(MD) {
      return true;
    }
    page_address addr;
    addr.raw = address;

    page_table pt;
    pt.raw = _ram->read_dw(PT + 4 * addr.page_table);

    dword page = _ram->read_dw(pt.address + 4 * addr.page);
    page_entry pe;
    pe.raw = page;

    return pe.mode == false;
  } else {
    return true;
  }
}

bool vm::can_write(dword address) {
  if(PE && PT != 0) {
    if(MD) {
      return true;
    }
    page_address addr;
    addr.raw = address;

    page_table pt;
    pt.raw = _ram->read_dw(PT + 4 * addr.page_table);

    dword page = _ram->read_dw(pt.address + 4 * addr.page);
    page_entry pe;
    pe.raw = page;

    return pe.mode == false && pe.write;
  } else {
    return true;
  }
}
#endif
