use std::fs;

enum Instruction {
  Forward,
  Down,
  Up
}

fn get_final_pos(instructions: &Vec<(Instruction, u32)>) -> u32 {
  let mut x: u32 = 0;
  let mut y: u32 = 0;

  for (instr, steps) in instructions {
    match instr {
      Instruction::Forward => x += steps,
      Instruction::Down => y += steps,
      Instruction::Up => y -= steps
    }
  }

  return x * y;
}

fn get_final_pos_with_aim(instructions: &Vec<(Instruction, u32)>) -> u32 {
  let mut x: u32 = 0;
  let mut y: u32 = 0;
  let mut aim: u32 = 0;

  for (instr, steps) in instructions {
    match instr {
      Instruction::Forward => {
        x += steps;
        y += aim * steps;
      }
      Instruction::Down => aim += steps,
      Instruction::Up => aim -= steps
    }
  }

  return x * y;
}

fn main() {
  let contents = fs::read_to_string("./data/q2.txt")
    .expect("Should have been able to read file");

  let instructions: Vec<(Instruction, u32)> = contents.split('\n')
    .map(|str| {
      let parts = str.split(' ').collect::<Vec<&str>>();
      let instr = match parts[0] {
        "forward" => Instruction::Forward,
        "down" => Instruction::Down,
        "up" => Instruction::Up,
        _ => {
          println!("Unexpected instruction {}", parts[0]);
          Instruction::Up
        }
      };
      let steps = parts[1].parse::<u32>().unwrap();
      (instr, steps)
    })
    .collect::<Vec<(Instruction, u32)>>();

  println!("Part 1: {}", get_final_pos(&instructions));
  println!("Part 2: {}", get_final_pos_with_aim(&instructions));
}