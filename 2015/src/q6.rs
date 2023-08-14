use std::fs;
use regex::Regex;

#[derive(Debug)]
struct Instruction {
  instruction: String,
  lower_x: usize,
  lower_y: usize,
  upper_x: usize,
  upper_y: usize,
}

impl Instruction {
  fn get_x_range(&self) -> std::ops::Range<usize> {
    return self.lower_x..self.upper_x + 1;
  }

  fn get_y_range(&self) -> std::ops::Range<usize> {
    return self.lower_y..self.upper_y + 1;
  }
}

fn turn_on_lights(lights: &mut Vec<Vec<bool>>, instruction: &Instruction) {
  for y in instruction.get_y_range() {
    for x in instruction.get_x_range() {
      lights[y][x] = true;
    }
  }
}

fn turn_off_lights(lights: &mut Vec<Vec<bool>>, instruction: &Instruction) {
  for y in instruction.get_y_range() {
    for x in instruction.get_x_range() {
      lights[y][x] = false;
    }
  }
}

fn toggle_lights(lights: &mut Vec<Vec<bool>>, instruction: &Instruction) {
  for y in instruction.get_y_range() {
    for x in instruction.get_x_range() {
      lights[y][x] = !lights[y][x];
    }
  }
}

fn run_and_count_lights_on(instructions: &Vec<Instruction>) -> u32 {
  let mut lights: Vec<Vec<bool>> = vec![vec![false; 1000]; 1000];

  for instruction in instructions.iter() {
    match instruction.instruction.as_str() {
      "turn on" => turn_on_lights(&mut lights, &instruction),
      "turn off" => turn_off_lights(&mut lights, &instruction),
      "toggle" => toggle_lights(&mut lights, &instruction),
      _ => ()
    }
  }

  return lights.iter().fold(0, |acc, c|
    acc + c.iter().fold(0, |acc1, val|
      acc1 + (*val as u32)));
}

fn turn_up_light_brightness(lights: &mut Vec<Vec<u32>>, instruction: &Instruction) {
  for y in instruction.get_y_range() {
    for x in instruction.get_x_range() {
      lights[y][x] += 1;
    }
  }
}

fn turn_down_light_brightness(lights: &mut Vec<Vec<u32>>, instruction: &Instruction) {
  for y in instruction.get_y_range() {
    for x in instruction.get_x_range() {
      if lights[y][x] == 0 { continue }
      lights[y][x] -= 1;
    }
  }
}

fn toggle_light_brightness(lights: &mut Vec<Vec<u32>>, instruction: &Instruction) {
  for y in instruction.get_y_range() {
    for x in instruction.get_x_range() {
      lights[y][x] += 2;
    }
  }
}

fn run_with_brightness_and_count_lights_on(instructions: &Vec<Instruction>) -> u32 {
  let mut lights: Vec<Vec<u32>> = vec![vec![0; 1000]; 1000];

  for instruction in instructions.iter() {
    match instruction.instruction.as_str() {
      "turn on" => turn_up_light_brightness(&mut lights, &instruction),
      "turn off" => turn_down_light_brightness(&mut lights, &instruction),
      "toggle" => toggle_light_brightness(&mut lights, &instruction),
      _ => ()
    }
  }

  return lights.iter().fold(0, |acc, c|
    acc + c.iter().sum::<u32>());
}

fn main() {
  let contents = fs::read_to_string("./data/q6.txt")
    .expect("Should have been able to read file");

  let re 
    = Regex::new(r"(turn on|turn off|toggle) ([\d]+),([\d]+) through ([\d]+),([\d]+)")
    .unwrap();

  let instructions: Vec<Instruction> = contents.split("\n")
    .map(|instruction_str| {
      let caps = re.captures(instruction_str).unwrap();
      Instruction {
        instruction: caps[1].to_string(),
        lower_x: caps[2].parse::<usize>().unwrap(),
        lower_y: caps[3].parse::<usize>().unwrap(),
        upper_x: caps[4].parse::<usize>().unwrap(),
        upper_y: caps[5].parse::<usize>().unwrap()
      }
    })
    .collect::<Vec<Instruction>>();

  println!("Part 1: {}", run_and_count_lights_on(&instructions));
  println!("Part 2: {}", run_with_brightness_and_count_lights_on(&instructions));
}