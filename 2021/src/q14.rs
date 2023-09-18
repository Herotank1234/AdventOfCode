use std::fs;
use std::collections::HashMap;

fn get_max_min(molecule_count: &HashMap<char, u64>) -> (u64, u64) {
  let mut max = u64::MIN;
  let mut min = u64::MAX;

  for (_, quantity) in molecule_count.iter() {
    max = std::cmp::max(max, *quantity);
    min = std::cmp::min(min, *quantity);
  }

  return (max, min);
}

fn apply_insertion_and_get_difference(
  initial_molecule: &String,
  rules: &HashMap<String, String>,
  repetition: u32,
) -> u64 {
  let mut molecule_pairs: HashMap<String, u64> = HashMap::new();
  let molecule_chars: Vec<char> = initial_molecule.chars().collect();
  let mut last_pair = String::new();

  for i in 0..initial_molecule.len() - 1 {
    let pair: String = format!("{}{}", molecule_chars[i], molecule_chars[i + 1]);
    if i == initial_molecule.len() - 2 { last_pair = pair.clone(); }
    let entry = molecule_pairs.entry(pair).or_insert(0);
    *entry += 1;
  }

  for _ in 0..repetition {
    let mut next_molecule_pairs: HashMap<String, u64> = HashMap::new();

    for (pair, quanitity) in molecule_pairs.iter() {
      let rule = rules.get(pair).unwrap();
      let next_first_pair = format!("{}{}", pair.chars().nth(0).unwrap(), rule);
      let next_second_pair = format!("{}{}", rule, pair.chars().nth(1).unwrap());

      if *pair == last_pair { last_pair = next_second_pair.clone(); }

      let first_entry = next_molecule_pairs.entry(next_first_pair)
        .or_insert(0);
      *first_entry += quanitity;

      let second_entry = next_molecule_pairs.entry(next_second_pair)
        .or_insert(0);
      *second_entry += quanitity;
    }

    molecule_pairs = next_molecule_pairs;
  }

  let mut molecule_count: HashMap<char, u64> = HashMap::new();
  for (pair, quantity) in molecule_pairs.iter() {
    let molecules: Vec<char> = pair.chars().collect();
    let entry = molecule_count.entry(molecules[0]).or_insert(0);
    *entry += quantity;

    if *pair == last_pair {
      let last_molecule = molecule_count.entry(molecules[1])
        .or_insert(0);
      *last_molecule += 1;
    }
  }

  let (max, min) = get_max_min(&molecule_count);
  return max - min;
}

fn main() {
  let contents = fs::read_to_string("./data/q14.txt")
    .expect("Should have been able to read file");
  let lines: Vec<&str> = contents.split('\n').collect();

  let initial_molecule = lines[0].to_string();
  let mut rules: HashMap<String, String> = HashMap::new();

  for i in 2..lines.len() {
    let parts: Vec<&str> = lines[i].split(" -> ").collect();
    rules.insert(parts[0].to_string(), parts[1].to_string());
  }
  println!("Part 1: {}", apply_insertion_and_get_difference(&initial_molecule, &rules, 10));
  println!("Part 2: {}", apply_insertion_and_get_difference(&initial_molecule, &rules, 40));
}