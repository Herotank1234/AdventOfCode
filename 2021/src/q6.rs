use std::fs;
use std::collections::HashMap;

fn count_final_lanternfish(initial_values: &Vec<u32>, days: u32) -> u64 {
  let mut fish_timers: HashMap<u32, u64> = HashMap::new();

  for i in 0..9 {
    fish_timers.insert(i, 0);
  }

  for val in initial_values {
    *(fish_timers.get_mut(val).unwrap()) += 1;
  }

  for _ in 0..days {
    let mut next_fish_timers: HashMap<u32, u64> = HashMap::new();

    for i in (0..8).rev() {
      next_fish_timers.insert(i, *fish_timers.get(&(i + 1)).unwrap());
    }

    *next_fish_timers.get_mut(&6).unwrap() += *fish_timers.get(&0).unwrap();
    next_fish_timers.insert(8, *fish_timers.get(&0).unwrap());
    fish_timers = next_fish_timers;
  }

  return fish_timers.iter()
    .fold(0, |a, (_, v)| a + v);
}

fn main() {
  let contents = fs::read_to_string("./data/q6.txt")
    .expect("Should have been able to read file");
  let initial_values = contents.split(',')
    .map(|val| val.parse::<u32>().unwrap())
    .collect::<Vec<u32>>();

  println!("Part 1: {}", count_final_lanternfish(&initial_values, 80));
  println!("Part 2: {}", count_final_lanternfish(&initial_values, 256));
}