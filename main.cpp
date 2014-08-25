#if __cplusplus != 201103L
#error MUST BE COMPILED WITH C++11 STANDARD
#endif

#include "vm/main.hpp"

using namespace std;

int main(int argc, char** args)
{
  glutInit(&argc,args);
  //get vm
  shared_ptr<vm> m = make_shared<vm>();

  m->clear_registers();

  //make test program

  /*
  mov AX, 5
  cmp AX, 5
  outb 1,0
  outb 0,1
  */

  //mov AX, 5 # set AX register to 5
  mov_i i1;
  i1.op = opcode::mov;
  i1.addr_mode_a = addressing_mode::direct;
  i1.addr_mode_b = addressing_mode::direct;
  i1.type_a = argument_type::registers;
  i1.type_b = argument_type::value;
  i1.reg = register_e::AX;
  i1.data = 5;

  //cmp AX,5 # compare AX register and 5
  math_i i2;
  i2.op = opcode::math;
  i2.addr_mode = addressing_mode::direct;
  i2.type = argument_type::value;
  i2.reg = register_e::AX;
  i2.flag = 1;
  i2.data = 5;

  //outb 1,1 # dump registers
  io_i i3;
  i3.op = opcode::io;
  i3.addr_mode = addressing_mode::direct;
  i3.type = argument_type::value;
  i3.flag = 2;
  i3.port = 1;
  i3.data = 1;

  //outb 0,1 # power off
  io_i i4;
  i4.op = opcode::io;
  i4.addr_mode = addressing_mode::direct;
  i4.type = argument_type::value;
  i4.flag = 2;
  i4.port = 0;
  i4.data = 1;

  m->_databridge->power();
  m->_cpu->power();
  m->_ram->power();
  m->_psu->power();

  std::shared_ptr<vm_dmp_regs_port> dmp_reg_port = std::make_shared<vm_dmp_regs_port>(&*m);
  m->_databridge->register_port(1,dmp_reg_port);
  m->add_device(make_shared<VGA_dev>());

  m->_ram->write(128,i1.raw);
  m->_ram->write(128+8,i2.raw);
  m->_ram->write(128+8*2,i3.raw);
  m->_ram->write(128+8*3,i4.raw);

  m->MD = true;

  m->get_device(4)->power();

  cout << "EXECUTING!" << endl;
  m->execute(128);

  while(true) {
    m->get_device(4)->tick();
  }

  return 0;
}
