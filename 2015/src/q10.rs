use std::fs;

fn transform_string(string: &String, reps: usize) -> usize {
  let mut curr_string = string.clone();

  for _ in 0..reps {
    let mut next_string = "".to_string();
    let curr_string_vec = curr_string.chars().collect::<Vec<char>>();
    let mut index = 0;
    while index < curr_string_vec.len() {
      let curr_char = curr_string_vec[index];
      let mut char_count: u32 = 1;

      while index + 1 < curr_string_vec.len() && curr_string_vec[index + 1] == curr_char {
        index += 1;
        char_count += 1;
      }

      next_string.push(char::from_digit(char_count, 10).unwrap());
      next_string.push(curr_char);
      index += 1;
    }

    curr_string = next_string;
  }

  return curr_string.len();
}

fn main() {
  let string = fs::read_to_string("./data/q10.txt")
    .expect("Should have been able to read file");
  println!("Part 1: {}", transform_string(&string, 40));
  println!("Part 2: {}", transform_string(&string, 50));
}