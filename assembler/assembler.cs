using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;

namespace ncasm
{
	//wrapper for main method
	static class Program
	{
		static void Main(string[] args) {
			Instruction ins = new Instruction();
			ins.opcode = OpCode.MOV;
			ins.register = Register.AX;
			ins.addressing_mode = Addressing_mode.reg_val;
			ins.data = 5;

			Console.WriteLine("Is this little endian? " + BitConverter.IsLittleEndian);

			Console.Write("Input: "); string code = Console.ReadLine();

			Assembler a = new Assembler(code);
			a.Scan();
			foreach(Token t in a.tokens)
				Console.WriteLine(t.ToString());
			a.Assemble();

			foreach(Instruction i in a.instructions)
				Console.WriteLine(i.ToString());
		}
	}

	//below here is the assembler thing

	static class OpCodes
	{
		public const byte NOP = 0;
		public const byte MOV = 1;
		public const byte JMP = 2;
		public const byte CMP = 3;
		public const byte MTH = 4;
		public const byte INT = 5;
		public const byte BIO = 6;
		public const byte STK = 7;
		public const byte PWR = 8;
	}

	static class Registers
	{
		public const byte A = 0;
		public const byte B = 1;
		public const byte C = 2;
		public const byte D = 3;
		public const byte AX = 4;
		public const byte BX = 5;
		public const byte CX = 6;
		public const byte DX = 7;
		public const byte AXX = 8;
		public const byte BXX = 9;
		public const byte CXX = 10;
		public const byte DXX = 11;
		public const byte IP = 12;
		public const byte SP = 13;
		public const byte BP = 14;
		public const byte HP = 15;
		public const byte SI = 16;
		public const byte DI = 17;
		public const byte IVT = 18;
		public const byte PG = 19;
		public const byte EQ = 20;
		public const byte GT = 21;
		public const byte RN = 22;
		public const byte HLT = 23;
		public const byte PE = 24;
		public const byte MD = 25;
		public const byte TRAP = 26;
		public const byte FAULT = 27;
		public const byte FAULT_ADDR = 28;
	}

	public enum OpCode : byte
	{
		NOP = 0,
		MOV,
		JMP,
		CMP,
		MTH,
		INT,
		BIO,
		STK,
		PWR
	}

	public enum Register : byte
	{
		A = 0,B,C,D,
		AX,BX,CX,DX,
		AXX,BXX,CXX,DXX,
		IP,SP,BP,HP,
		SI,DI,
		IVT,PG,EQ,GT,RN,HLT,PE,MD,
		TRAP,FAULT,FAULT_ADDR,
		NULL
	}

	public enum Addressing_mode : byte
	{
		reg_reg = 0, //mov reg, reg
		reg_val,		 //mov reg, val
		reg_adr,		 //mov reg, [adr]
		reg_aor,		 //mov reg, [reg]
		aor_reg,		 //mov [reg], reg
		aor_aor,		 //mov [reg], [reg]
		adr_reg		  //mov [adr], reg
	}

	public enum valtype
	{
		reg,
		val,
		aor,
		adr
	}

	public struct Instruction
	{
		public string label;

		public OpCode opcode;
		public Register register;

		public valtype LvalType;
		public valtype RvalType;

		public Addressing_mode addressing_mode;
		public byte flags;
		public int data;

		public byte[] toBytes() {
			byte[] result = new byte[8];
			result[0] = (byte)opcode;
			result[1] = (byte)register;
			result[2] = (byte)addressing_mode;
			result[3] = flags;
			byte[] datab = BitConverter.GetBytes(data);
			result[4] = datab[0];
			result[5] = datab[1];
			result[6] = datab[2];
			result[7] = datab[3];

			return result;
		}

		public override string ToString() {
			StringBuilder result = new StringBuilder("instruction: {");
			if(label != null) result.Append("label: " + label + ",");
			result.Append("opcode: " + opcode.ToString() + ",");
			result.Append("register: " + register.ToString() + ",");
			result.Append("LvalType: " + LvalType.ToString() + ",");
			result.Append("RvalType: " + RvalType.ToString() + ",");
			result.Append("flags: " + flags.ToString() + ",");
			result.Append("data: " + data + "}");
			return result.ToString();
		}
	}

	public struct DefinedBytes
	{
		public string label;
		public uint size;
		public object data;
	}

	public class Token
	{
		public const byte WORD = 0;
		public const byte BYTE_LIT = 1;
		public const byte INT_LIT = 2;
		public const byte UINT_LIT = 3;
		public const byte FLOAT_LIT = 4;
		public const byte STRING_LIT = 5;
		public const byte SYMBOL = 6;
		public const byte BOOL_LIT = 7;
    public const byte CHAR_LIT = 8;

		public object value;
		public byte type;

		public string TypeString
		{
			get
			{
				switch (type) {
					case 0:
					return "WORD";
					case 1:
					return "BYTE_LIT";
					case 2:
					return "INT_LIT";
					case 3:
					return "uINT_LIT";
					case 4:
					return "FLOAT_LIT";
					case 5:
					return "STRING_LIT";
					case 6:
					return "SYMBOL";
					case 7:
					return "BOOL_LIT";
					case 8:
					return "CHAR_LIT";
					default:
					return "unknown";
				}
			}
		}

		public override string ToString() {
			return "token: {type:" + TypeString + ",value:" + value.ToString() + "}";
		}
	}

	public static class Extentions
	{
		public static string f(this string that, params object[] args) {
			return String.Format(that,args);
		}
	}

	public class Assembler
	{
		string code;
		public List<Token> tokens = new List<Token>();
		public List<Instruction> instructions = new List<Instruction>();
		List<DefinedBytes> definedbytes = new List<DefinedBytes>();
		List<Tuple<int,string>> calls = new List<Tuple<int,string>>();
	  byte[] program;
	  int i = 0;

		internal Instruction CurrentInstruction;

		public Assembler (string Code) {
			code = Code;
		}

		public Token CurrentToken
		{
			get
			{
				return tokens[i];
			}
		}

		public void Next(){
			i++;
		}

		public bool Expect(Token t) {
			if(Check(t)) return true;
			throw new ParseException("Unexpected token",i,this);
		}

		public bool Expect(char c) {
			if(Check(c)) return true;
			throw new ParseException("Unexpected symbol, expected '{0}', got:{1}".f(c,tokens[i].value), i, this);
		}

		public bool Expect(byte TokenType) {
			if(Check(TokenType)) return true;
			throw new ParseException("Unexpected token type",i,this);
		}

		public void Assert(bool condition, string failmsg) {
			if (!condition) throw new ParseException(failmsg,i,this);
		}

		public bool Check(Token t) {
			return CurrentToken == t;
		}

		public bool Check(char c) {
			return (Check(Token.CHAR_LIT) || Check(Token.SYMBOL)) && (char)CurrentToken.value == c;
		}

		public bool Check(byte TokenType) {
			return CurrentToken.type == TokenType;
		}

		public bool Check(string val) {
			return Check(Token.WORD) && CurrentToken.value as string == val;
		}

		public bool EOF
		{
			get {
				return i >= code.Length;
			}
		}

    public byte[] GetProgram() {
        return program.ToArray();
    }

		public void Assemble() {
      for(i = 0; i < tokens.Count; i++) {
				CurrentInstruction = new Instruction();
				Console.WriteLine(CurrentToken.value as string);
				if(Check("mov")) {
					CurrentInstruction.opcode = OpCode.MOV;
					Next();

					// --- parse Lvalue in mov
					ParseLval();
					Next();

					Expect(',');
					Next();

					ParseRval();
				}
				instructions.Add(CurrentInstruction);
			}
      InsertCalls();
		}

    public void InsertCalls() {
    }

		public void ParseLval()
		{
			if(Check(Token.WORD)) {
				Register reg = Register.NULL;
				if(Register.TryParse(tokens[i].value as string, out reg)) { //mov reg, ?
					CurrentInstruction.register = reg;
					CurrentInstruction.LvalType = valtype.reg;
				} else {
					throw new ParseException("Token is not a register, cannot move", i, this);
				}
			} else if (Check('[')) { //mov [?], ?
					Next();
					Expect(Token.WORD);
					if(tokens[i].type == Token.WORD) {
						Register reg = Register.NULL;
						if(Register.TryParse(tokens[i].value as string, out reg)) { //mov [reg], ?
							CurrentInstruction.register = reg;
							CurrentInstruction.LvalType = valtype.aor;
						} else { //mov [adr], reg
							//assume this is a label

							CurrentInstruction.LvalType = valtype.adr;
							calls.Add(new Tuple<int,string>(i,CurrentToken.value as string));
						}
						Next();
					} else if (tokens[i].type == Token.INT_LIT) { //mov [adr], reg
						CurrentInstruction.data = (int)tokens[i].value;
						CurrentInstruction.LvalType = valtype.adr;
						Next();
					} else if (tokens[i].type == Token.UINT_LIT) { //mov [adr], reg
						CurrentInstruction.data = (int)(uint)tokens[i].value;
						CurrentInstruction.LvalType = valtype.adr;
						Next();
					} else if (tokens[i].type == Token.BYTE_LIT) { //mov [adr], reg
						CurrentInstruction.data = (int)(byte)tokens[i].value;
						CurrentInstruction.LvalType = valtype.adr;
						Next();
					}
					Expect(']');
				} else {
					throw new ParseException("Invalid token for Lvalue in mov",i,this);
				}
		}

		public void ParseRval()
		{
			if(Check(Token.BYTE_LIT)) { //mov ?, val
				Assert((CurrentInstruction.LvalType == valtype.reg || CurrentInstruction.LvalType == valtype.aor),
				"Cannot move byte to anything other than a register or address of a register");

				CurrentInstruction.data = (int)(byte)CurrentToken.value;
				CurrentInstruction.RvalType = valtype.val;
			} else if (Check(Token.INT_LIT)) {
				Assert((CurrentInstruction.LvalType == valtype.reg || CurrentInstruction.LvalType == valtype.aor),
				"Cannot move int to anything other than a register or address of a register");

				CurrentInstruction.data = (int)CurrentToken.value;
				CurrentInstruction.RvalType = valtype.val;
			} else if (Check(Token.UINT_LIT)) {
				Assert((CurrentInstruction.LvalType == valtype.reg || CurrentInstruction.LvalType == valtype.aor),
				"Cannot move uint to anything other than a register or address of a register");

				CurrentInstruction.data = (int)(uint)CurrentToken.value;
				CurrentInstruction.RvalType = valtype.val;
			} else if (Check(Token.WORD)) {
				Register reg = Register.NULL;
				if(Register.TryParse(tokens[i].value as string, out reg)) {
					if(CurrentInstruction.LvalType == valtype.reg) {
						CurrentInstruction.data = (int)reg;
						CurrentInstruction.RvalType = valtype.reg;
					}
				} else {
					//assume this is a label
					Assert(CurrentInstruction.addressing_mode == Addressing_mode.reg_reg || CurrentInstruction.addressing_mode == Addressing_mode.reg_val || CurrentInstruction.addressing_mode == Addressing_mode.aor_reg,
					"Cannot move address to anything other than register or address of register");

					CurrentInstruction.RvalType = valtype.val;
					calls.Add(new Tuple<int,string>(i,CurrentToken.value as string));
				}
			} else if (Check('[')) {
				Next();
				if(Check(Token.WORD)) {
					Register reg = Register.NULL;
					if(Register.TryParse(tokens[i].value as string, out reg)) {
						if(CurrentInstruction.addressing_mode == Addressing_mode.reg_reg) {
							CurrentInstruction.data = (int)reg;
							CurrentInstruction.RvalType = valtype.aor;
						}
					} else {
						//assume label
						CurrentInstruction.RvalType = valtype.val;
						calls.Add(new Tuple<int,string>(i,CurrentToken.value as string));
					}
				}
				Next();
				Expect(']');
			} else {
				throw new ParseException("WHAT!?",i,this);
			}
		}

		public void Scan()
		{
			tokens = new List<Token>();
			i = 0;
			while (!EOF)
			{
				if (char.IsDigit(code[i]))
				{
					StringBuilder strb = new StringBuilder();
					while (!EOF && char.IsDigit(code[i]))
					{
							strb.Append(code[i]);
							i++;
					}
					if(!EOF) {
					if (code[i] == '.')
					{
							strb.Append('.');
							i++;
							while (char.IsDigit(code[i]))
							{
									strb.Append(code[i]);
									i++;
							}
							i--;
							float f = (float)Convert.ToDecimal(strb.ToString());
							tokens.Add(new Token() { type = Token.FLOAT_LIT, value = f });
							continue;
					}
					else if (code[i] == 'x')
					{
							i += 2;
							strb.Clear();
							strb.Append(code[i]);
							i++;
							strb.Append(code[i]);
							byte b = byte.Parse(strb.ToString(), System.Globalization.NumberStyles.HexNumber);
							tokens.Add(new Token() { type = Token.BYTE_LIT, value = b });
							continue;
					}
					}
					string str = strb.ToString();
					int n = Convert.ToInt32(str);
					tokens.Add(new Token() { type = Token.INT_LIT, value = n });
					continue;
				}
				else if (code[i] == '#') {
					while(code[i] != '\n') {
						i++;
					}
				}
				else if (char.IsLetter(code[i]) || code[i] == '_')
				{
						StringBuilder strb = new StringBuilder();
						while (i < code.Length && (char.IsLetterOrDigit(code[i]) || code[i] == '@' || code[i] == '.' || code[i] == '_'))
						{
								strb.Append(code[i]);
								i++;
						}
						if (strb.ToString() == "true")
						{
								tokens.Add(new Token() { type = Token.BOOL_LIT, value = true });
						}
						else if (strb.ToString() == "false")
						{
								tokens.Add(new Token() { type = Token.BYTE_LIT, value = false });
						}
						else
						{
								tokens.Add(new Token() { type = Token.WORD, value = strb.ToString() });
						}
						i--;
				}
				else if (code[i] == '"')
				{
						bool escaped = false;
						bool writtenescape = false;
						bool r = true;
						StringBuilder strb = new StringBuilder();
						i++;
						r = (code[i] != '"');
						while (r)
						{
								if (escaped && code[i] == 'n')
								{
										strb.Append("\n");
										writtenescape = true;
								}
								escaped = false;
								if (code[i] == '\\')
								{
										escaped = true;
								}
								else
								{
										if(!writtenescape) {
											strb.Append(code[i]);
								}
								writtenescape = false;
							}
							i++;
							if (!escaped)
							{
								r = (code[i] != '"');
							}
						}
							tokens.Add(new Token() { type = Token.STRING_LIT, value = strb.ToString() });
					} else if (code[i] == '\'') {
						i++;
						char c = code[i];
						i++; i++;
						tokens.Add(new Token() {type = Token.CHAR_LIT, value = c});
					}
					else if (code[i] == '\n')
					{
							//nop
					}
					else if (code[i] == ' ')
					{
							//nop
					}
					else if (code[i] == '\t')
					{
						//nop
					}
					else if (code[i] == '\r')
					{
						//nop
					}
					else
					{
							tokens.Add(new Token() { type = Token.SYMBOL, value = code[i] });
					}
					i++;
				}
			}
	}

	class ParseException : Exception
	{
		int tokenid;
		Assembler assembler;

		public ParseException(string message, int tokenid, Assembler asm) : base(message + ";; TOKEN TRACE:{0} ;; CURRENT INSTRUCTION: {1}".f(asm.tokens[tokenid].ToString(),asm.CurrentInstruction.ToString())) {
			this.assembler = asm;
			this.tokenid = tokenid;
		}
	}
}
