#include <ctime>
#include "virtual-machine/vm.hpp"
//#include "assembler/assembler.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if(argc > 1)
    DEBUG_OUT = true;
  auto start = clock();
  vm machine{};
  vector<instruction> program{
    {MOV,4,reg_val,0,100},
    {BIO,4,reg_reg,0,2},
    {MOV,0,reg_val,0,'a'},
    {MOV,4,aor_reg,0,0},
    {MTH,4,reg_reg,5,0},
    {MOV,0,reg_val,0,'b'},
    {MOV,4,aor_reg,0,0},
    {MOV,1,reg_val,0,0},
    {BIO,1,reg_reg,0,2},
    {PWR,0,reg_reg,1,0}
  };
  int n = 0;
  for(auto i : program)
    i.write(machine.memory,n++ * 8);
  machine.power();
  auto end = clock();
  cout << "done! execution time (microsec): " << end - start << endl;
  return 0;
}
