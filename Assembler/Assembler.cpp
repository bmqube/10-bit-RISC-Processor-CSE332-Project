#include <bits/stdc++.h>
using namespace std;

map<string, string> opname_to_opcode;
map<string, string> regname_to_regno;
vector<string> r_type = {"add", "sub", "sll"};
vector<string> i_type = {"addi", "lw", "sw", "beq"};
vector<string> j_type = {"jmp"};

void init() {
  opname_to_opcode["add"] = "000";
  opname_to_opcode["sub"] = "001";
  opname_to_opcode["addi"] = "010";
  opname_to_opcode["sll"] = "011";
  opname_to_opcode["lw"] = "100";
  opname_to_opcode["sw"] = "101";
  opname_to_opcode["beq"] = "110";
  opname_to_opcode["jmp"] = "111";

  regname_to_regno["$ac"] = "00";
  regname_to_regno["$s1"] = "01";
  regname_to_regno["$s2"] = "10";
  regname_to_regno["$s3"] = "11";
}

string to_binary(int n, int size) {
  string r;
  while (n != 0) {
    r = (n % 2 == 0 ? "0" : "1") + r;
    n /= 2;
  }

  while (r.size() < size) {
    r = "0" + r;
  }

  return r;
}

int main() {
  init();
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  cout << "v2.0 raw" << endl;
  string inst_line;
  while (getline(cin, inst_line)) {
    vector<string> curr_operation;
    string temp_inst;
    string opname;
    string final_binary_instruction;
    // cout << inst_line << endl;

    for (const auto &ch : inst_line) {
      if (ch == ' ' || ch == ',') {
        if (temp_inst.size()) {
          curr_operation.push_back(temp_inst);
          temp_inst.clear();
        }
      } else {
        temp_inst.append(1, ch);
      }
    }

    if (temp_inst.size()) {
      curr_operation.push_back(temp_inst);
      temp_inst.clear();
    }

    opname = curr_operation.front();

    if (find(r_type.begin(), r_type.end(), opname) != r_type.end()) {
      final_binary_instruction.append(opname_to_opcode[opname]);
      if (opname == "sll") {
        final_binary_instruction.append("00");
        final_binary_instruction.append(regname_to_regno[curr_operation.at(2)]);
        final_binary_instruction.append(regname_to_regno[curr_operation.at(1)]);
        final_binary_instruction.append(curr_operation.at(3) == "1" ? "1"
                                                                    : "0");
      } else {
        final_binary_instruction.append(regname_to_regno[curr_operation.at(2)]);
        final_binary_instruction.append(regname_to_regno[curr_operation.at(3)]);
        final_binary_instruction.append(regname_to_regno[curr_operation.at(1)]);
        final_binary_instruction.append("0");
      }
    } else if (find(i_type.begin(), i_type.end(), opname) != i_type.end()) {
      final_binary_instruction += opname_to_opcode[opname];
      final_binary_instruction.append(regname_to_regno[curr_operation.at(2)]);
      final_binary_instruction.append(regname_to_regno[curr_operation.at(1)]);
      // cout << to_binary(stoi(curr_operation.at(3)), 3) << endl;
      final_binary_instruction.append(to_binary(stoi(curr_operation.at(3)), 3));
    } else if (find(j_type.begin(), j_type.end(), opname) != j_type.end()) {
      final_binary_instruction += opname_to_opcode[opname];
      bitset<7> immediate(curr_operation.at(1));
      final_binary_instruction.append(immediate.to_string());
    } else {
      cout << "Error, Unknown Operation" << endl;
      return 1;
    }

    // cout << final_binary_instruction << endl;
    unsigned int result = bitset<10>(final_binary_instruction).to_ulong();
    cout << hex << uppercase << result << endl;
  }

  return 0;
}
