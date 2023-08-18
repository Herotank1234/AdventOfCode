use std::fs;

fn get_group(
  packages: &Vec<u64>, 
  curr_group: &Vec<u64>, 
  limit: u64) -> Vec<Vec<u64>>
{ 
  let sum = curr_group.iter().sum::<u64>();
  if sum == limit { return vec![curr_group.clone()] }

  if sum > limit { return vec![] }

  let mut next_packages = packages.clone();
  let mut valid_groups: Vec<Vec<u64>> = Vec::new();
  for package in packages.iter() {
    let mut next_group = curr_group.clone();
    next_group.push(*package);
    next_packages.remove(0);

    let groups = get_group(&next_packages, &next_group, limit);

    groups.iter().for_each(|group| valid_groups.push(group.clone()));
  }

  return valid_groups;
}

fn get_quantum_entanglement(packages: &Vec<u64>, num_groups: u64) -> u64 {
  let weight_limit: u64 = packages.iter().sum::<u64>() / num_groups;
  let mut groups = get_group(packages, &Vec::new(), weight_limit);

  groups.sort_by(|a, b| a.len().cmp(&b.len()));
  let min_len = groups[0].len();
  let mut quantum_entanglements = groups
    .iter()
    .take_while(|group| group.len() == min_len)
    .map(|group| group.iter().product::<u64>())
    .collect::<Vec<u64>>();

  quantum_entanglements.sort();

  return quantum_entanglements[0];
}

fn main() {
  let contents = fs::read_to_string("./data/q24.txt")
    .expect("Should have been able to read file");
  let packages = contents
    .split("\n")
    .map(|package| package.parse::<u64>().unwrap())
    .collect::<Vec<u64>>();

  println!("Part 1: {}", get_quantum_entanglement(&packages, 3));
  println!("Part 2: {}", get_quantum_entanglement(&packages, 4));
}