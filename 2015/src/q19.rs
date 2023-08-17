use std::{fs, collections::{HashMap, HashSet}};

fn replace_molecule(
  molecule: &Vec<char>, 
  key: &String, 
  index: usize, 
  replacement: &String) -> String 
{
  let mut next_molecule = molecule.clone();
  for _ in 0..key.len() {
    next_molecule.remove(index);
  }

  for letter in replacement.chars().rev() {
    next_molecule.insert(index, letter);
  }

  return next_molecule.iter().collect::<String>();
}

fn count_distinct_molecules_after_replacement(
  initial_molecule: &String, 
  rules: &HashMap<String, Vec<String>>) -> usize 
{
  let molecule_vec: Vec<char> = initial_molecule.chars().collect();
  let mut distinct_molcules: HashSet<String> = HashSet::new();
  let mut index: usize = 0;

  while index < molecule_vec.len() {
    if molecule_vec[index] >= 'A' && molecule_vec[index] <= 'Z' {
      let key = molecule_vec[index].to_string();

      if rules.contains_key(&key) {
        let replacements = rules.get(&key).unwrap();
        for replacement in replacements.iter() {
          let new_molecule 
            = replace_molecule(&molecule_vec, &key, index, replacement);
          distinct_molcules.insert(new_molecule);
        }
      }
    } else if molecule_vec[index] >= 'a' && molecule_vec[index] <= 'z' {
      let key = format!("{}{}", molecule_vec[index - 1], molecule_vec[index]);

      if rules.contains_key(&key) {
        let replacements = rules.get(&key).unwrap();
        for replacement in replacements.iter() {
          let new_molecule 
            = replace_molecule(&molecule_vec, &key, index - 1, replacement);
          distinct_molcules.insert(new_molecule);
        }
      }
    }

    index += 1;
  }

  return distinct_molcules.len();
}

fn try_reduce(
  molecule: &String, 
  longest_match: usize, 
  rules: &HashMap<String, String>) -> (u32, String)
{
  let mut reductions: u32 = 0;
  let mut molecule_vec: Vec<char> = molecule.chars().collect();
  let mut index: isize = (molecule_vec.len() - 1) as isize;

  while index >= 0 {
    let mut window = String::new();
    let mut window_index = index;
    window.push(molecule_vec[window_index as usize]);

    while window_index + 1 < molecule_vec.len() as isize && window.len() < longest_match && 
      !rules.contains_key(&window) 
    {
      window_index += 1;
      window.push(molecule_vec[window_index as usize]);
    }

    if rules.contains_key(&window) {
      let replacement = rules.get(&window).unwrap();
      molecule_vec = replace_molecule(&molecule_vec, &window, index as usize, replacement)
        .chars()
        .collect();
      reductions += 1;
    }

    index -= 1;
  }

  return (reductions, molecule_vec.iter().collect());
}

fn molecule_fabrication(target: &String, rules: &HashMap<String, Vec<String>>) -> u32 {
  let mut steps: u32 = 0;
  let mut curr_molecule = target.clone();
  let mut reverse_rules: HashMap<String, String> = HashMap::new();
  let mut longest_match: usize = 0;
  for (k, v) in rules.iter() {
    for result in v.iter() {
      if result.len() > longest_match { longest_match = result.len() }
      reverse_rules.insert(result.clone(), k.clone());
    }
  }

  while curr_molecule.as_str() != "e" {
    let (reductions, next_molecule) 
      = try_reduce(&curr_molecule, longest_match, &reverse_rules);
    steps += reductions;
    curr_molecule = next_molecule;
  }

  return steps;
}

fn main() {
  let contents = fs::read_to_string("./data/q19.txt")
    .expect("Should have been able to read file");
  let mut initial_molecule: String = String::new();
  let mut rules: HashMap<String, Vec<String>> = HashMap::new();
  let mut parsing_rules: bool = true;

  for line in contents.split("\n") {
    if line == "" {
      parsing_rules = false;
      continue;
    }

    if parsing_rules {
      let rule = line.split(" => ")
        .map(|particle| particle.to_string())
        .collect::<Vec<String>>();
      if rules.contains_key(&rule[0]) {
        rules.get_mut(&rule[0]).unwrap().push(rule[1].clone());
      } else {
        rules.insert(rule[0].clone(), vec![rule[1].clone()]);
      }
    } else {
      initial_molecule = line.to_string();
    }
  }

  println!("Part 1: {}", count_distinct_molecules_after_replacement(&initial_molecule, &rules));
  println!("Part 2: {}", molecule_fabrication(&initial_molecule, &rules));
}