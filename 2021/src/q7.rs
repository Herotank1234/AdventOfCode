use std::fs;

fn get_min_fuel_to_align(positions: &Vec<u32>) -> u32 {
  let mut curr_positions = positions.clone();
  curr_positions.sort();
  let median = curr_positions[curr_positions.len() / 2];
  
  return curr_positions.iter()
    .map(|val| val.abs_diff(median))
    .sum::<u32>();
}

fn get_compound_fuel_used(val1: &u32, val2: &u32) -> u32 {
  let diff = val1.abs_diff(*val2);
  return diff * (diff + 1) / 2;
}

fn get_min_compound_fuel_to_align(positions: &Vec<u32>) -> u32 {
  let curr_positions = positions.clone();
  let mean: u32 = curr_positions.iter().sum::<u32>() / curr_positions.len() as u32;

  return curr_positions.iter()
    .map(|val| get_compound_fuel_used(&val, &mean))
    .sum::<u32>();
}

fn main() {
  let contents = fs::read_to_string("./data/q7.txt")
    .expect("Should have been able to read file");

  let positions = contents.split(',')
    .map(|str| str.parse::<u32>().unwrap())
    .collect::<Vec<u32>>();

  println!("Part 1: {}", get_min_fuel_to_align(&positions));
  println!("Part 2: {}", get_min_compound_fuel_to_align(&positions));
}