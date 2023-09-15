use std::fs;
use std::collections::{HashSet, HashMap};

fn get_corrupted_score(brackets_list: &Vec<&str>) -> u32 {
  let mut score = 0;
  let opening_brackets: HashSet<char> = HashSet::from_iter(vec!['(', '[', '{', '<']);
  let scoring_system: HashMap<char, u32> = HashMap::from_iter(
    vec![(')', 3), (']', 57), ('}', 1197), ('>', 25137)]
  );
  let pairs: HashMap<char, char> = HashMap::from_iter(
    vec![(')', '('), (']', '['), ('}', '{'), ('>', '<')]
  );

  for brackets in brackets_list {
    let mut stack: Vec<char> = Vec::new();
    for bracket in brackets.chars() {
      if opening_brackets.contains(&bracket) { 
        stack.push(bracket);
      } else {
        let opening_bracket = stack.pop().unwrap();
        if *pairs.get(&bracket).unwrap() != opening_bracket {
          score += *scoring_system.get(&bracket).unwrap();
          break;
        }
      }
    }
  }

  return score;
}

fn get_median_complete_score(brackets_list: &Vec<&str>) -> u64 {
  let mut scores: Vec<u64> = Vec::new();
  let opening_brackets: HashSet<char> = HashSet::from_iter(vec!['(', '[', '{', '<']);
  let scoring_system: HashMap<char, u64> = HashMap::from_iter(
    vec![('(', 1), ('[', 2), ('{', 3), ('<', 4)]
  );
  let pairs: HashMap<char, char> = HashMap::from_iter(
    vec![(')', '('), (']', '['), ('}', '{'), ('>', '<')]
  );

  for brackets in brackets_list {
    let mut stack: Vec<char> = Vec::new();
    let mut incomplete = false;

    for bracket in brackets.chars() {
      if opening_brackets.contains(&bracket) { 
        stack.push(bracket);
      } else {
        let opening_bracket = stack.pop().unwrap();
        if *pairs.get(&bracket).unwrap() != opening_bracket {
          incomplete = true;
          break;
        }
      }
    }

    if incomplete { continue; }
    if stack.is_empty() { continue; }

    let mut curr_score: u64 = 0;
    while !stack.is_empty() {
      curr_score *= 5;
      let points = scoring_system.get(&stack.pop().unwrap()).unwrap();
      curr_score += *points;
    }

    scores.push(curr_score);
  }

  scores.sort();

  return scores[scores.len() / 2];
}

fn main() {
  let contents = fs::read_to_string("./data/q10.txt")
    .expect("Should have been able to read file");
  let brackets_list: Vec<&str> = contents.split('\n').collect();

  println!("Part 1: {}", get_corrupted_score(&brackets_list));
  println!("Part 2: {}", get_median_complete_score(&brackets_list));
}