use std::fs;

fn parse_backslash(string: &str, index: usize) -> usize {
  return match string.chars().nth(index + 1).unwrap() {
    '\"' | '\\' => index + 2,
    'x' => index + 4,
    _ => {
      println!("{} not recognised", string.chars().nth(index + 1).unwrap());
      index + 1
    }
  }
}

fn chr_diff(strings: &Vec<&str>) -> u32 {
  return strings.iter().map(|string| {
    let mut index: usize = 1;
    let mut mem_size: u32 = 0;
    while index < string.len() - 1 {
      match string.chars().nth(index).unwrap() {
        '\\' => index = parse_backslash(string, index),
        _ => index += 1
      }
      mem_size += 1;
    }
    string.len() as u32 - mem_size
  }).sum();
}

fn chr_diff_opposite(strings: &Vec<&str>) -> u32 {
  return strings.iter().map(|string| {
    let mut size: u32 = 2;
    for char in string.chars() {
      match char {
        '\\' | '\"' => size += 2,
        _ => size += 1
      }
    }
    size - string.len() as u32
  }).sum();
}

fn main() {
  let contents = fs::read_to_string("./data/q8.txt")
    .expect("Should have been able to read file");
  let strings = contents.split("\n").collect::<Vec<&str>>();

  println!("Part 1: {}", chr_diff(&strings));
  println!("Part 2: {}", chr_diff_opposite(&strings));
}