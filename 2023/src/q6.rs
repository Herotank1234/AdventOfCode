use std::fs;

fn get_ways_to_win(time: u32, best_dist: u32) -> u32 {
  let mut ways_to_win = 0;
  for wait_time in 0..=time {
    let dist_traveled = (time - wait_time) * wait_time;
    if dist_traveled > best_dist { ways_to_win += 1; }
  }
  return ways_to_win;
}

fn multiply_ways_to_win(times: &Vec<u32>, best_dists: &Vec<u32>) -> u32 {
  let mut total = 1;
  for i in 0..times.len() {
    total *= get_ways_to_win(times[i], best_dists[i]);
  }
  return total;
}

fn get_ways_to_win_concat(times: &Vec<u32>, best_dists: &Vec<u32>) -> u64 {
  let final_time = times.iter().fold("".to_owned(), |acc, time| acc + &time.to_string())
    .parse::<u64>().unwrap();
  let final_dist = best_dists.iter().fold("".to_owned(), |acc, time| acc + &time.to_string())
    .parse::<u64>().unwrap();

  let mut ways_to_lose = 0;
  for wait_final_time in 0..=final_time {
    let dist_traveled = (final_time - wait_final_time) * wait_final_time;
    if dist_traveled <= final_dist { 
      ways_to_lose += 1; 
    } else {
      break;
    }
  }
  return final_time - (2 * ways_to_lose - 1);
}

fn main() {
  let contents = fs::read_to_string("./data/q6.txt")
    .expect("Should have been able to read file");

  let lines = contents.split('\n').collect::<Vec<&str>>();
  let times = lines[0].split_whitespace().skip(1).map(|val| val.parse::<u32>().unwrap())
    .collect::<Vec<u32>>();
  let best_dists = lines[1].split_whitespace().skip(1).map(|val| val.parse::<u32>().unwrap())
    .collect::<Vec<u32>>();

  println!("Part 1: {}", multiply_ways_to_win(&times, &best_dists));
  println!("Part 2: {}", get_ways_to_win_concat(&times, &best_dists));
}