use std::fs;
use std::collections::HashMap;
use regex::Regex;

fn get_calibration_value(line: &String) -> u32 {
  let mut first_value: char = '_';
  let mut last_value: char =  '_';

  for c in line.chars() {
    if c >= '0' && c <= '9' {
      if first_value == '_' { first_value = c; }
      last_value = c;
    }
  }

  let calibration_string: String = format!("{}{}", first_value, last_value);
  return calibration_string.parse::<u32>().unwrap();
}

fn sum_calibration_values(lines: &Vec<String>) -> u32 {
  let calibration_values: Vec<u32> = lines.iter()
    .map(|line| get_calibration_value(&line)).collect();
  return calibration_values.iter().sum();
}

fn convert(reg_match: &str) -> char {
  if reg_match.len() == 1 { return reg_match.chars().next().unwrap(); }

  let keywords: HashMap<&str, char> = HashMap::from([
    ("zero", '0'),
    ("one", '1'),
    ("two", '2'),
    ("three", '3'),
    ("four", '4'),
    ("five", '5'),
    ("six", '6'),
    ("seven", '7'),
    ("eight", '8'),
    ("nine", '9')
  ]);
  return *keywords.get(reg_match).unwrap();
}

fn get_real_calibration_value(line: &String) -> u32 {
  let rev_line = line.chars().rev().collect::<String>();
  let re = Regex::new("([0-9]|one|two|three|four|five|six|seven|eight|nine|zero)").unwrap();
  let rev_re = Regex::new("([0-9]|eno|owt|eerht|ruof|evif|xis|neves|thgie|enin|orez)").unwrap();
  
  let first_value: char = convert(re.find(line).unwrap().as_str());
  let rev_match = rev_re.find(rev_line.as_str()).unwrap().as_str().chars().rev()
    .collect::<String>();
  let last_value: char = convert(rev_match.as_str());

  let result: String = format!("{}{}", first_value, last_value);
  return result.parse::<u32>().unwrap();
}

fn sum_real_calibration_values(lines: &Vec<String>) -> u32 {
  let calibration_values: Vec<u32> = lines.iter()
    .map(|line| get_real_calibration_value(&line))
    .collect();
  return calibration_values.iter().sum();
}

fn main() {
  let contents = fs::read_to_string("./data/q1.txt")
    .expect("Should have been able to read file");

  let lines: Vec<String> = contents.split('\n')
    .map(|line| line.to_string()).collect();

  println!("Part 1: {}", sum_calibration_values(&lines));
  println!("Part 2: {}", sum_real_calibration_values(&lines));
}