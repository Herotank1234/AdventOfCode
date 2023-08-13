use std::fs;
use md5;

fn find_integer_hash_with_leading_zeroes(secret_key: &str, number_of_zeroes: usize) -> u32 {
  let mut number: u32 = 1;
  loop {
    let plaintext = format!("{}{}", secret_key, number.to_string());
    let hash_string = format!("{:X}", md5::compute(plaintext));
    let num_leading_zeroes: usize = hash_string.chars()
      .take_while(|c: &char| *c == '0')
      .collect::<Vec<char>>()
      .len();
    if num_leading_zeroes >= number_of_zeroes { break; }
    number += 1;
  }
  return number;
}

fn main() {
  let secret_key = fs::read_to_string("./data/q4.txt")
    .expect("Should have been able to read file");
  println!("Part 1: {}", find_integer_hash_with_leading_zeroes(&secret_key, 5));
  println!("Part 2: {}", find_integer_hash_with_leading_zeroes(&secret_key, 6));
}