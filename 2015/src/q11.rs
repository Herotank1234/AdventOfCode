use std::fs;
use std::collections::HashSet;

fn increment_password(password: String) -> String {
  let mut next_password = "".to_string();
  let mut carry = true;

  for char in password.chars().rev() {
    let next_char = char as u8 + if carry { 1 } else { 0 };
    if next_char == 123 {
      next_password.push('a');
      carry = true;
    } else {
      next_password.push(next_char as char);
      carry = false;
    }
  }

  if carry { next_password.push('a') }

  return next_password.chars().rev().collect::<String>();
}

fn has_straight(password: &String) -> bool {
  let password_vec = password.chars().collect::<Vec<char>>();
  for i in 0..password_vec.len() - 2 {
    if password_vec[i + 2] as i16 - password_vec[i + 1] as i16 == 1 &&
      password_vec[i + 1] as i16 - password_vec[i] as i16 == 1
    {
      return true;
    }
  }
  return false;
}

fn no_forbidden_letters(password: &String) -> bool {
  for letter in password.chars() {
    match letter {
      'i' | 'o' | 'l' => return false,
      _ => ()
    }
  }
  return true;
}

fn has_non_overlapping_pairs(password: &String) -> bool {
  let password_vec = password.chars().collect::<Vec<char>>();
  let mut pairs: HashSet<String> = HashSet::new();

  for i in 0..password_vec.len() - 1 {
    if password_vec[i] == password_vec[i + 1] {
      let pair = format!("{}{}", 
        password_vec[i].to_string(), password_vec[i + 1].to_string());
      if pairs.is_empty() {
        pairs.insert(pair);
      } else {
        if !pairs.contains(&pair) {
          return true;
        }
      }
    }
  }

  return false;
}

fn get_next_password(initial_password: &String) -> String {
  let mut curr_password = initial_password.clone();
  
  loop {
    curr_password = increment_password(curr_password);

    if has_straight(&curr_password) && no_forbidden_letters(&curr_password) && 
      has_non_overlapping_pairs(&curr_password) 
    {
      break;
    }
  }

  return curr_password;
}

fn main() {
  let initial_password = fs::read_to_string("./data/q11.txt")
    .expect("Should have been able to read file");
  println!("Part 1: {}", get_next_password(&initial_password));
  println!("Part 2: {}", get_next_password(&get_next_password(&initial_password)));
}