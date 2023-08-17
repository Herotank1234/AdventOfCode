use std::{fs, collections::HashMap};

#[derive(Debug, Clone)]
enum Instruction {
  Hlf(String),
  Tpl(String),
  Inc(String),
  Jmp(i32),
  Jie(String, i32),
  Jio(String, i32)
}

fn run(instructions: &Vec<Instruction>, registers: &mut HashMap<String, u32>) {
  let mut index: usize = 0;

  while index < instructions.len() {
    match &instructions[index] {
      Instruction::Hlf(reg) => {
        *registers.get_mut(reg).unwrap() /= 2;
        index += 1;
      }

      Instruction::Tpl(reg) => {
        *registers.get_mut(reg).unwrap() *= 3;
        index += 1;
      }

      Instruction::Inc(reg) => {
        *registers.get_mut(reg).unwrap() += 1;
        index += 1;
      }

      Instruction::Jmp(offset) => {
        index = (index as i32 + offset) as usize;
      }

      Instruction::Jie(reg, offset) => {
        if *registers.get(reg).unwrap() % 2 == 0 {
          index = (index as i32 + offset) as usize;
        } else {
          index += 1;
        }
      }

      Instruction::Jio(reg, offset) => {
        if *registers.get(reg).unwrap() == 1 {
          index = (index as i32 + offset) as usize;
        } else {
          index += 1;
        }
      }
    } 
  }
}

fn run_and_get_value_of_reg(instructions: &Vec<Instruction>, register: String) -> u32 {
  let mut registers = HashMap::from_iter([
    ("a".to_string(), 0),
    ("b".to_string(), 0)
  ]);
  run(instructions, &mut registers);
  return registers[&register];
}

fn set_reg_run_and_get_value_of_reg(instructions: &Vec<Instruction>, register: String) -> u32 {
  let mut registers = HashMap::from_iter([
    ("a".to_string(), 1),
    ("b".to_string(), 0)
  ]);
  run(instructions, &mut registers);
  return registers[&register];
}

fn main() {
  let contents = fs::read_to_string("./data/q23.txt")
    .expect("Should have been able to read file");
  let mut instructions: Vec<Instruction> = Vec::new();

  for line in contents.split("\n") {
    let parts = line.split(" ").collect::<Vec<&str>>();
    match parts[0] {
      "hlf" => instructions.push(Instruction::Hlf(parts[1].to_string())),
      
      "tpl" => instructions.push(Instruction::Tpl(parts[1].to_string())),
      
      "inc" => instructions.push(Instruction::Inc(parts[1].to_string())),
      
      "jmp" => instructions.push(Instruction::Jmp(parts[1].parse::<i32>().unwrap())),
      
      "jie" => {
        let register = parts[1].chars().nth(0).unwrap().to_string();
        let offset = parts[2].parse::<i32>().unwrap();
        instructions.push(Instruction::Jie(register, offset));
      }

      "jio" => {
        let register = parts[1].chars().nth(0).unwrap().to_string();
        let offset = parts[2].parse::<i32>().unwrap();
        instructions.push(Instruction::Jio(register, offset));
      }

      _ => println!("Was unable to match line: {}", line),
    }
  }

  println!("Part 1: {}", run_and_get_value_of_reg(&instructions, "b".to_string()));
  println!("Part 2: {}", set_reg_run_and_get_value_of_reg(&instructions, "b".to_string()));
}