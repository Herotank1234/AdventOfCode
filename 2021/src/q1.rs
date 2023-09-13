use std::fs;

fn larger_than_prev(weights: &Vec<u32>, window_size: usize) -> u32 {
  let mut larger: u32 = 0;
  for i in 0..weights.len() - window_size{
    let mut curr_weight: u32 = 0;
    let mut next_weight: u32 = 0;

    for j in i..i + window_size {
      curr_weight += weights[j];
    }
    for j in i + 1.. i + 1 + window_size {
      next_weight += weights[j];
    }

    larger += if next_weight > curr_weight { 1 } else { 0 };
  }

  return larger;
}

fn main() {
  let contents = fs::read_to_string("./data/q1.txt")
    .expect("Should have been able to read file");

  let weights: Vec<u32> = contents.split('\n')
    .map(|str| str.parse::<u32>().unwrap())
    .collect::<Vec<u32>>();

  println!("Part 1: {}", larger_than_prev(&weights, 1));
  println!("Part 2: {}", larger_than_prev(&weights, 3));
}