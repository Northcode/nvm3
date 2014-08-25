#include "main.hpp"

#ifndef CPU_CPP
#define CPU_CPP

cpu::cpu() : device() {
  initialized = true;
}

void cpu::tick() {
  exec_instruction(machine->IP);
  machine->IP += 8;
}

void cpu::power() {

}

void cpu::reset() {

}

void cpu::exec_instruction(dword address) {
  opcode op = (opcode)machine->_ram->read_b(address);
  dword instruction_data = machine->_ram->read_dw(address);
  dword data = machine->_ram->read_dw(address + 4);

  long ins_raw = (((long)data) << 32 | instruction_data);

  if(op == opcode::mov) {
    mov_i mi;
    mi.raw = ins_raw;
    mov(mi);
  } else if (op == opcode::jmp) {
    jmp_i ji;
    ji.raw = ins_raw;
    jmp(ji);
  } else if (op == opcode::math) {
    math_i i;
    i.raw = ins_raw;
    math(i);
  } else if (op == opcode::interupt) {
    interupt_i i;
    i.raw = ins_raw;
    interupt(i);
  } else if (op == opcode::stack) {
    stack_i i;
    i.raw = ins_raw;
    stack(i);
  } else if (op == opcode::io) {
    io_i i;
    i.raw = ins_raw;
    io(i);
  } else if (op == opcode::cmp) {
    cmp_i i;
    i.raw = ins_raw;
    cmp(i);
  } else if (op == opcode::nop) {
    std::cout << "NOP HIT!" << std::endl;
    int c = 0;
    std::cin >> c;
  }
}

void cpu::mov(mov_i i) {
  dword valf = get_Rarg_val(i.data,i.type_b,i.addr_mode_b);
  if(machine->FAULT) return; //if get_Rarg_val generated an exception, stop execution

  if(i.type_a == argument_type::registers) {
    byte reg_size = machine->get_register_size(i.reg);
    if(reg_size == 32) {
      machine->set_register_dw(i.reg,valf);
    } else {
      machine->set_register_b(i.reg, valf & 0x000000ff);
    }
  } else if (i.type_a == argument_type::address_of_register) {
    dword address = machine->get_register_dw(i.reg);
    if(!machine->can_write(address)) { // check if we can write to this
      machine->FAULT_ADDR = address;
      machine->force_interupt(page_fault);
      return;
    }
    address = machine->get_address(address,i.addr_mode_a); // apply paging
    dword ssize = 1;
    if(i.type_b == argument_type::registers) {
      ssize = machine->get_register_size(get_register(i.data));
    }
    if(ssize == 32) {
      machine->_ram->write(address, valf);
    } else if (ssize == 8 || ssize == 1) {
      //allways assume we are writing bytes if not
      //explicitly stated by writing from 32 bit register
      machine->_ram->write(address, (byte)(valf & 0x000000ff));
    }
  }
}

void cpu::jmp(jmp_i i) {
  dword address = 0;
  if(i.type == argument_type::registers) {
    address = machine->get_register_dw(i.reg);
  } else if (i.type == argument_type::value) {
    address = i.data;
  } else if (i.type == argument_type::data_at_address) {
    if(!machine->can_read(i.data)) { // check if we can read this
      machine->FAULT_ADDR = address;
      machine->force_interupt(page_fault);
      return;
    }
    address = machine->_ram->read_dw(i.data);
  } else if (i.type == argument_type::address_of_register) {
    dword addr = machine->get_register_dw(i.reg);
    if(!machine->can_read(addr)) { // check if we can read this
      machine->FAULT_ADDR = address;
      machine->force_interupt(page_fault);
      return;
    }
    address = machine->_ram->read_dw(addr);
  }

  if(!machine->can_read(address)) { // check if we can read this
    machine->FAULT_ADDR = address;
    machine->force_interupt(page_fault);
    return;
  }

  address = machine->get_address(address,i.addr_mode); // apply paging

  if(i.flag == 0) { //normal jmp
    jump(address);
  } else if (i.flag == 1) { //normal call
    machine->call(address);
  } else if (i.flag == 2) { //je
    if(machine->EQ)
      jump(address);
  } else if (i.flag == 3) { //jgt
    if(machine->GT && !machine->EQ)
      jump(address);
  } else if (i.flag == 4) { //jge
    if(machine->GT)
      jump(address);
  } else if (i.flag == 5) { //jne
    if(!machine->EQ)
      jump(address);
  } else if (i.flag == 6) { //jls
    if(!machine->GT && !machine->EQ)
      jump(address);
  } else if (i.flag == 7) { //jle
    if(!machine->GT)
      jump(address);
  } else if (i.flag == 8) { //ret
    machine->ret();
  }
}

void cpu::jump(dword address) {
  machine->IP = address;
}

void cpu::math(math_i i) {
  dword vala = 0;
  byte rega_size = machine->get_register_size(i.reg);
  if(rega_size == 32) {
    vala = machine->get_register_dw(i.reg);
  } else if (rega_size == 1) {
    vala = (dword)machine->get_register_t(i.reg);
  } else {
    vala = (dword)machine->get_register_b(i.reg);
  }

  dword valb = get_Rarg_val(i.data,i.type,i.addr_mode);
  if(machine->FAULT) return; //if get_Rarg_val generated an exception, stop execution

  if(i.flag == 1) { // add reg, val
    vala += valb;
  } else if (i.flag == 2) { //sub reg, val
    vala -= valb;
  } else if (i.flag == 3) { //mul reg, val
    vala *= valb;
  } else if (i.flag == 4) { //div reg, val
    if(valb == 0) {
      machine->force_interupt(interupts::div_by_zero);
      return;
    }
    vala /= valb;
  } else if (i.flag == 5) { //mod reg, val
    vala %= valb;
  } else if (i.flag == 6) { //neg reg
    vala = -vala;
  } else if (i.flag == 7) { //sqrt reg
    vala = sqrt(vala);
  } else if (i.flag == 8) { //pow reg, val
    vala = pow(vala,valb);
  }

  if(rega_size == 32) {
    machine->set_register_dw(i.reg,vala);
  } else {
    machine->set_register_b(i.reg, vala & 0x000000ff);
  }
}

void cpu::interupt(interupt_i i) {
  if(i.flag == 0) { //int i
    if(i.index > 64) {
      machine->FAULT_ADDR = i.index;
      machine->force_interupt(invalid_interupt);
      return;
    }
    machine->force_interupt(i.index);
    return;
  } else if (i.flag == 1) { //iret
    machine->iret();
    return;
  } else if (i.flag == 2) { //reg_int i, addr
    if(!machine->MD) { //must be root to do this
      machine->FAULT_ADDR = machine->IP;
      machine->force_interupt(gp_fault);
      return;
    }
    dword address = i.address;
    if(!machine->can_read(address)) { // check if we can read to this
      machine->FAULT_ADDR = address;
      machine->force_interupt(page_fault);
      return;
    }
    address = machine->get_address(address,i.addr_mode);
    if(i.index > 64) {
      machine->FAULT_ADDR = i.index;
      machine->force_interupt(invalid_interupt);
      return;
    }
    machine->_ram->write(machine->IVT + i.index*4,address); //write address pointer to ivt
  }
}

void cpu::io(io_i i) {
  if(!machine->MD) { //all bus io requires root!
    machine->force_interupt(gp_fault);
    return;
  }

  if(i.flag == 0) { //inb port,reg
    register_e reg = get_register(i.data);
    byte reg_size = machine->get_register_size(reg);
    byte val = machine->_databridge->in_b(i.port);
    if(reg_size == 32) {
      machine->set_register_dw(reg,(dword)val);
    } else {
      machine->set_register_b(reg,val);
    }
  } else if(i.flag == 1) { //indw port,reg
    register_e reg = get_register(i.data);
    byte reg_size = machine->get_register_size(reg);
    dword val = machine->_databridge->in_dw(i.port);
    if(reg_size == 32) {
      machine->set_register_dw(reg,val);
    } else {
      machine->set_register_b(reg,val & 0x000000ff);
    }
  } else if (i.flag == 2) { //outb port,val
    dword val = get_Rarg_val(i.data,i.type,i.addr_mode);
    machine->_databridge->out_b(i.port,val);
  } else if (i.flag == 3) { //outdw port,val
    dword val = get_Rarg_val(i.data,i.type,i.addr_mode);
    machine->_databridge->out_dw(i.port,val);
  }
}

void cpu::stack(stack_i i) {
  if(i.flag == 0) { //pushb val
    byte val = get_Rarg_val(i.data,i.type,i.addr_mode) & 0x000000ff;
    machine->push(val);
  } else if (i.flag == 1) { //pushdw val
    dword val = get_Rarg_val(i.data,i.type,i.addr_mode);
    machine->push(val);
  } else if (i.flag == 2) { //popb reg
    byte regsize = machine->get_register_size(i.reg);
    byte val = machine->pop_b();
    if(regsize == 32) {
      machine->set_register_dw(i.reg,val);
    } else {
      machine->set_register_b(i.reg,val);
    }
  } else if (i.flag == 3) { //popdw reg
    byte regsize = machine->get_register_size(i.reg);
    dword val = machine->pop_dw();
    if(regsize == 32) {
      machine->set_register_dw(i.reg,val);
    } else {
      machine->set_register_b(i.reg,val & 0x000000ff);
    }
  }
}

void cpu::cmp(cmp_i i) {
  dword vala = get_Rarg_val(i.val_a,i.type_a,i.addr_mode_a);
  dword valb = get_Rarg_val(i.val_b,i.type_b,i.addr_mode_b);

  machine->EQ = (vala == valb);
  machine->GT = (vala > valb);
}

dword cpu::get_Rarg_val(dword data, argument_type type, addressing_mode mode) {
  //function for converting a right argument (source argument), to a value for use in an opcode

  if (type == argument_type::registers) {
    //reg
    register_e reg = get_register(data);
    byte regb_size = machine->get_register_size(reg);
    if(regb_size == 32) {
      return machine->get_register_dw(reg);
    } else if (regb_size == 1) {
      return (dword)machine->get_register_t(reg);
    } else {
      return (dword)machine->get_register_b(reg);
    }
  } else if (type == argument_type::value) {
    //val
    return data;
  } else if (type == argument_type::data_at_address) {
    //[adr]
    register_e reg = get_register(data);
    dword address = machine->get_register_dw(reg);

    if(!machine->can_read(address)) {
      machine->FAULT_ADDR = address;
      machine->force_interupt(page_fault);
      return 0;
    }

    address = machine->get_address(address,mode);

    return machine->_ram->read_dw(address);
  } else if (type == argument_type::address_of_register) {
    //[reg]
    dword addr = machine->get_register_dw(get_register(data));
    if(!machine->can_read(addr)) { // check if we can read this
      machine->FAULT_ADDR = addr;
      machine->force_interupt(page_fault);
      return 0;
    }
    return machine->_ram->read_dw(addr);
  }
}

#endif
