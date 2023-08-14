use std::fs;
use std::collections::HashMap;

fn count_nice_strings(strings: &Vec<&str>) -> u32 {
  let nice_strings: u32 = strings.iter()
    .map(|string: &&str| {
      let char_vec: Vec<char> = string.chars().collect::<Vec<char>>();
      let mut num_vowels: u32 = 0;
      let mut double_letters: u32 = 0;
      let mut has_forbidden_string: bool = false;
      for i in 0..char_vec.len() {
        match char_vec[i] {
          'a' | 'e' | 'i' | 'o' | 'u' => num_vowels += 1,
          _ => ()
        }
        if i != char_vec.len() - 1 {
          if char_vec[i] == char_vec[i + 1] { double_letters += 1 }
          let window: String = format!("{}{}", char_vec[i], char_vec[i + 1]);
          match window.as_str() {
            "ab" | "cd" | "pq" | "xy" => has_forbidden_string = true,
            _ => ()
          }
        }
      }
      if num_vowels >= 3 && double_letters >= 1 && !has_forbidden_string { 1 } else { 0 }
    })
    .sum();
  return nice_strings;
}

fn count_nice_strings_new_way(strings: &Vec<&str>) -> u32 {
  let mut nice_strings = 0;

  for string in strings.iter() {
    let char_vec = string.chars().collect::<Vec<char>>();
    let mut has_repeated_letters = false;
    let mut double_letters: HashMap<String, usize> = HashMap::new();
    for i in 0..char_vec.len() - 1 {
      let window = format!("{}{}", char_vec[i], char_vec[i + 1]);
      if double_letters.contains_key(&window) {
        let first_seen_index = double_letters.get(&window).unwrap();
        if i - first_seen_index > 1 {
          has_repeated_letters = true;
          break;
        }
      } else {
        double_letters.insert(window, i);
      }
    }

    if !has_repeated_letters { continue }

    let mut has_aba = false;

    for i in 0..char_vec.len() - 2 {
      if char_vec[i] == char_vec[i + 2] {
        has_aba = true;
        break;
      }
    }

    if !has_aba { continue }
    nice_strings += 1;
  }

  return nice_strings;
}

fn main() {
  let contents = fs::read_to_string("./data/q5.txt")
    .expect("Should have been able to read file");
  let strings: Vec<&str> = contents.split("\n").collect::<Vec<&str>>();

  println!("Part 1: {}", count_nice_strings(&strings));
  println!("Part 2: {}", count_nice_strings_new_way(&strings));
}