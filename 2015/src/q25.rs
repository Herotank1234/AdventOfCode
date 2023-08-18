use std::fs;
use regex::Regex;

fn get_value(row: &i32, col: &i32) -> u64{
  let mut curr_row: i32 = 1;
  let mut curr_col: i32 = 1;
  let mut max_row: i32 = 1;
  let mut prev_val: u64 = 20151125;
  let mut curr_val: u64;
  let multiplier: u64 = 252533;
  let modulo: u64 = 33554393;

  loop {
    curr_row -= 1;
    curr_col += 1;

    if curr_row == 0 {
      max_row += 1;
      curr_row = max_row;
      curr_col = 1;
    }

    curr_val = (prev_val * multiplier) % modulo;

    if curr_row == *row && curr_col == *col {
      break;
    }

    prev_val = curr_val;
  }

  return curr_val;
}

fn main() {
  let contents = fs::read_to_string("./data/q25.txt")
    .expect("Should have been able to read file");
  let re = Regex::new("To continue, please consult the code grid \
    in the manual.  Enter the code at row ([\\d]+), column ([\\d]+).").unwrap();
  let captures = re.captures(&contents);
  let mut row: i32 = 0;
  let mut col: i32 = 0;

  match captures {
    Some(matched_groups) => {
      row = matched_groups[1].parse::<i32>().unwrap();
      col = matched_groups[2].parse::<i32>().unwrap();
    }

    None => println!("Was unable to match line: {}", contents),
  }

  println!("Part 1: {}", get_value(&row, &col));
  println!("Part 2: {}", 0);
}