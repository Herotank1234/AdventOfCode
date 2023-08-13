use std::fs;

fn run_elevator(brackets: &String) -> i16 {
  let mut curr_level: i16 = 0;
  for bracket in brackets.chars() {
    curr_level += if bracket == '(' { 1 } else { -1 };
  }
  return curr_level;
}

fn find_index_that_enters_basement(brackets: &String) -> u16 {
  let mut curr_level: i16 = 0;
  let mut index: u16 = 0;
  for bracket in brackets.chars() {
    index += 1;
    curr_level += if bracket == '(' { 1 } else { -1 };
    if curr_level == -1 {
      break;
    }
  }
  return index;
}

fn main() {
  let brackets = fs::read_to_string("./data/q1.txt")
    .expect("Should have been able to read file");

  println!("Part 1: {}", run_elevator(&brackets));
  println!("Part 2: {}", find_index_that_enters_basement(&brackets));
}
