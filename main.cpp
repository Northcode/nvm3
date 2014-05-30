#include <ctime>
#include "vm.hpp"

using namespace std;

int main() {
  auto start = clock();
  vm machine{};
  instruction movax3{MOV,1,reg_val,0,3};
  movax3.write(machine.memory,0);
  instruction poweroff{PWR,0,reg_reg,1,0};
  poweroff.write(machine.memory,8);
  machine.power();
  auto end = clock();
  cout << "done! execution time (microsec): " << end - start << endl;
  return 0;
}
