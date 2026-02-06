use std::collections::HashMap;
use std::fs;
use regex::Regex;

#[derive(Debug)]
struct Node {
  left: String,
  right: String
}

fn count_steps_to_reach_end(steps: &Vec<char>, nodes: &HashMap<String, Node>) -> u32 {
  let mut curr_node = "AAA".to_string();
  let end = "ZZZ".to_string();
  let mut steps_taken = 0;
  let mut steps_index = 0;

  while curr_node != end {
    match steps[steps_index] {
      'R' => curr_node = nodes[&curr_node].right.clone(),
      'L' => curr_node = nodes[&curr_node].left.clone(),
      _ => println!("{} could not be matched", steps[steps_index])
    }
    steps_taken += 1;
    steps_index += 1;
    if steps_index == steps.len() { steps_index = 0; }
  }

  return steps_taken
}

fn gcd(a: u64, b: u64) -> u64 {
  let mut larger = if a > b { a } else { b };
  let mut smaller = if a > b { b } else { a }; 
  while smaller != 0 {
    let rem = larger % smaller;
    larger = smaller;
    smaller = rem;
  }
  return larger;
}

fn lcm(a: u64, b: u64) -> u64 {
  if a == 1 { return b; }
  if b == 1 { return a; }
  return a * (b / gcd(a, b));
}

fn count_steps_to_reach_end_simul(steps: &Vec<char>, nodes: &HashMap<String, Node>) -> u64 {
  let mut curr_nodes = nodes.keys().into_iter().filter(|key| key.ends_with('A'))
    .map(|key| key.clone()).collect::<Vec<String>>();
  let mut steps_needed = Vec::new();

  for node in curr_nodes.iter_mut() {
    let mut steps_taken = 0;
    let mut steps_index = 0;

    while !node.ends_with('Z') {
      match steps[steps_index] {
        'R' => *node = nodes[node].right.clone(),
        'L' => *node = nodes[node].left.clone(),
        _ => println!("{} could not be matched", steps[steps_index])
      }
      steps_taken += 1;
      steps_index += 1;
      if steps_index == steps.len() { steps_index = 0; }
    }
    steps_needed.push(steps_taken);
  }

  return steps_needed.iter().fold(1, |acc, steps| lcm(acc, *steps));
}

fn main() {
  let contents = fs::read_to_string("./data/q8.txt")
    .expect("Should have been able to read file");
  let lines = contents.split('\n').collect::<Vec<&str>>();
  let re = Regex::new(r"([A-Z0-9]{3}) = \(([A-Z0-9]{3}), ([A-Z0-9]{3})\)").unwrap();

  let steps = lines[0].chars().collect::<Vec<char>>();
  let nodes = lines.iter().skip(2).map(|line| {
    let caps = re.captures(line).unwrap();
    return (caps[1].to_string(), Node{left: caps[2].to_string(), right: caps[3].to_string()});
  }).collect::<HashMap<String, Node>>();

  println!("Part 1: {}", count_steps_to_reach_end(&steps, &nodes));
  println!("Part 2: {}", count_steps_to_reach_end_simul(&steps, &nodes));
}