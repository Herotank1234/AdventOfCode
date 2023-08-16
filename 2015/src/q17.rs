use std::fs;

fn count_combinations(containers: &Vec<u32>, curr_size: u32, total_size: u32) -> u32 {
  if total_size == curr_size { return 1 }

  let mut combinations: u32 = 0;
  let mut next_containers: Vec<u32> = containers.clone();
  for i in 0..containers.len() {
    if curr_size + containers[i] > total_size { break }

    let next_size = curr_size + containers[i];
    next_containers.remove(0);
    combinations += count_combinations(&next_containers, next_size, total_size);
  }

  return combinations;
}

fn get_combinations(
  containers: &Vec<u32>,
  inventory: &Vec<u32>,
  curr_size: u32, 
  total_size: u32) -> Vec<Vec<u32>> 
{
  if total_size == curr_size { return vec![inventory.clone()] }

  let mut combinations: Vec<Vec<u32>> = Vec::new();
  let mut next_containers: Vec<u32> = containers.clone();
  for i in 0..containers.len() {
    if curr_size + containers[i] > total_size { break }

    let next_size = curr_size + containers[i];
    let mut next_inventory = inventory.clone();
    next_inventory.push(containers[i]);
    next_containers.remove(0);
    
    let curr_combinations 
      = get_combinations(&next_containers, &next_inventory, next_size, total_size);

    curr_combinations.iter().for_each(|comb| combinations.push(comb.clone()));
  }

  return combinations;
}

fn count_combinations_that_fit(containers: &Vec<u32>, total_size: u32) -> u32 {
  return count_combinations(containers, 0, total_size);
}

fn count_ways_to_fill_minimum_containers(containers: &Vec<u32>, total_size: u32) -> usize {
  let inventory: Vec<u32> = Vec::new();
  let combinations 
    = get_combinations(containers, &inventory, 0, total_size);

  let minimum_containers_needed = combinations
    .iter()
    .map(|comb| comb.len())
    .min()
    .unwrap();

  let ways_to_use_min_containers = combinations
    .iter()
    .filter(|comb| comb.len() == minimum_containers_needed)
    .collect::<Vec<&Vec<u32>>>()
    .len();

  return ways_to_use_min_containers;
}

fn main() {
  let contents = fs::read_to_string("./data/q17.txt")
    .expect("Should have been able to read file");

  let mut containers = contents
    .split("\n")
    .map(|container| container.parse::<u32>().unwrap())
    .collect::<Vec<u32>>();
  containers.sort();

  println!("Part 1: {}", count_combinations_that_fit(&containers, 150));
  println!("Part 2: {}", count_ways_to_fill_minimum_containers(&containers, 150));
}