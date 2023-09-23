use std::cmp::max;
use std::fmt;
use std::fs;

#[derive(Clone, PartialEq)]
enum SF {
  OpenBracket,
  CloseBracket,
  Comma,
  Value(u32)
}

impl fmt::Display for SF {
  fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
    match self {
      SF::OpenBracket => write!(f, "["),
      SF::CloseBracket => write!(f, "]"),
      SF::Comma => write!(f, ","),
      SF::Value(val) => write!(f, "{}", val)
    }
  }
}

fn add_sf(left: &Vec<SF>, right: &Vec<SF>) -> Vec<SF> {
  let mut result = Vec::new();
  result.push(SF::OpenBracket);
  result.append(&mut left.clone());
  result.push(SF::Comma);
  result.append(&mut right.clone());
  result.push(SF::CloseBracket);
  return result;
}

fn explode(sf_num: &mut Vec<SF>, index: usize, left: u32, right: u32) {
  let mut left_index = index - 1;
  loop {
    match sf_num[left_index] {
      SF::OpenBracket => (),
      SF::CloseBracket => (),
      SF::Comma => (),
      SF::Value(val) => {
        sf_num[left_index] = SF::Value(val + left);
        break;
      }
    }
    if left_index == 0 { break; }
    left_index -= 1;
  }

  let mut right_index = index + 3;
  loop {
    match sf_num[right_index] {
      SF::OpenBracket => (),
      SF::CloseBracket => (),
      SF::Comma => (),
      SF::Value(val) => {
        sf_num[right_index] = SF::Value(val + right);
        break;
      }
    }
    if right_index == sf_num.len() - 1 { break; }
    right_index += 1;
  }
  
  let replace_index = index - 1;
  for _ in 0..5 {
    sf_num.remove(replace_index);
  }
  sf_num.insert(replace_index, SF::Value(0));
}

fn split(sf_num: &mut Vec<SF>, index: usize, value: u32) {
  let lower = value / 2;
  let upper = value / 2 + (value % 2);
  
  sf_num.remove(index);
  sf_num.insert(index, SF::CloseBracket);
  sf_num.insert(index, SF::Value(upper));
  sf_num.insert(index, SF::Comma);
  sf_num.insert(index, SF::Value(lower));
  sf_num.insert(index, SF::OpenBracket);
}

fn reduce_sf(sf_num: &mut Vec<SF>) -> bool {
  let mut index: usize = 0;
  let mut depth: i32 = -1;
  while index < sf_num.len() {
    match sf_num[index] {
      SF::OpenBracket => depth += 1,
      SF::Comma => (),
      SF::CloseBracket => depth -= 1,
      SF::Value(left) => {
        if depth >= 4 {
          if sf_num[index + 1] == SF::Comma && matches!(sf_num[index + 2], SF::Value(..)) {
            let right = match sf_num[index + 2] {
              SF::OpenBracket => 0,
              SF::CloseBracket => 0,
              SF::Comma => 0,
              SF::Value(val) => val,
            };
            explode(sf_num, index, left, right);
            return true;
          }
        }
      }
    }
    index += 1;
  }

  index = 0;
  while index < sf_num.len() {
    match sf_num[index] {
      SF::OpenBracket => (),
      SF::Comma => (),
      SF::CloseBracket => (),
      SF::Value(val) => {
        if val > 9 {
          split(sf_num, index, val);
          return true;
        }
      }
    }
    index += 1;
  }
  return false;
}

fn add_and_reduce(left: &Vec<SF>, right: &Vec<SF>) -> Vec<SF> {
  let mut result = add_sf(left, right);
  loop {
    if !reduce_sf(&mut result) { break; }
  }
  return result;
}

fn get_magnitude(sf_num: &Vec<SF>, index: usize) -> (usize, u32) {
  match sf_num[index] {
    SF::OpenBracket => {
      let (next_index, left) = get_magnitude(sf_num, index + 1);
      let (final_index, right) = get_magnitude(sf_num, next_index + 1);
      return (final_index + 1, 3 * left + 2 * right);
    }
    SF::Comma => unreachable!(),
    SF::CloseBracket => unreachable!(),
    SF::Value(val) => return (index + 1, val)
  }
}

fn magnitude(sf_num: &Vec<SF>) -> u32 {
  let (_, mag) = get_magnitude(sf_num, 0);
  return mag;
}

fn add_and_get_magnitude(sf_nums: &Vec<Vec<SF>>) -> u32 {
  let mut result = sf_nums[0].clone();

  for i in 1..sf_nums.len() {
    result = add_and_reduce(&result, &sf_nums[i].clone());
  }

  return magnitude(&result);
}

fn parse_to_sf(str: &str) -> Vec<SF> {
  let mut sf = Vec::new();

  for c in str.chars() {
    match c {
      '[' => sf.push(SF::OpenBracket),
      ',' => sf.push(SF::Comma),
      ']' => sf.push(SF::CloseBracket),
      val => sf.push(SF::Value(val.to_digit(10).unwrap()))
    }
  }

  return sf;
}

fn find_max_magnitude_between_two_sf(sf_nums: &Vec<Vec<SF>>) -> u32 {
  let mut max_mag = 0;
  for i in 0..sf_nums.len() {
    for j in i + 1..sf_nums.len() {
      max_mag = max(max_mag, magnitude(&add_and_reduce(&sf_nums[i], &sf_nums[j])));
      max_mag = max(max_mag, magnitude(&add_and_reduce(&sf_nums[j], &sf_nums[i])));
    }
  }
  return max_mag;
}

fn main() {
  let contents = fs::read_to_string("./data/q18.txt")
    .expect("Should have been able to read file");

  let sf_nums: Vec<Vec<SF>> = contents.split('\n')
    .map(|str| parse_to_sf(str))
    .collect();
  println!("Part 1: {}", add_and_get_magnitude(&sf_nums));
  println!("Part 2: {}", find_max_magnitude_between_two_sf(&sf_nums));
}