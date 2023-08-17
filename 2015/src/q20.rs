use std::fs;

fn get_lowest_house_exceeding_limit(limit: &u32) -> u32 {
  let house_number_limit: u32 = limit / 10;
  let mut factors: Vec<Vec<u32>> = vec![vec![]; house_number_limit as usize];
  for factor in 1..house_number_limit + 1 {
    for index in (factor..house_number_limit + 1).step_by(factor as usize) {
      factors[index as usize - 1].push(factor);
    }
  }

  let mut house_number = 0;
  for i in 0..factors.len() {
    if factors[i].iter().sum::<u32>() >= house_number_limit { 
      house_number = (i + 1) as u32;
      break;
    }
  }
  return house_number;
}

fn get_new_lowest_house_exceeding_limit(limit: &u32) -> u32 {
  let house_number_limit: u32 = (limit / 11) + 1;
  let mut factors: Vec<Vec<u32>> = vec![vec![]; house_number_limit as usize];
  for factor in 1..house_number_limit + 1 {
    let mut delivered_to: u32 = 0;
    for index in (factor..house_number_limit + 1).step_by(factor as usize) {
      if delivered_to == 50 { break }
      factors[index as usize - 1].push(factor);
      delivered_to += 1;
    }
  }

  let mut house_number = 0;
  for i in 0..factors.len() {
    if factors[i].iter().sum::<u32>() * 11 >= *limit { 
      house_number = (i + 1) as u32;
      break;
    }
  }
  return house_number;
}

fn main() {
  let contents = fs::read_to_string("./data/q20.txt")
    .expect("Should have been able to read file");
  let present_limit = contents.parse::<u32>().unwrap();

  println!("Part 1: {}", get_lowest_house_exceeding_limit(&present_limit));
  println!("Part 2: {}", get_new_lowest_house_exceeding_limit(&present_limit));
}