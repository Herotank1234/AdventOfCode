use std::fs;
use regex::Regex;
use std::collections::HashMap;

#[derive(Debug, Clone)]
struct Reindeer {
  name: String,
  speed: u32,
  time_running: u32,
  time_resting: u32,
  rest_until: u32,
  run_until: u32,
  distance_ran: u32,
  is_running: bool
}

impl Default for Reindeer {
  fn default() -> Reindeer {
    Reindeer { 
      name: String::new(),
      speed: 0, 
      time_running: 0, 
      time_resting: 0, 
      rest_until: 0, 
      run_until: 0, 
      distance_ran: 0, 
      is_running: false 
    }
  }
}

fn calculate_longest_distance_travelled(
  reindeers: &mut Vec<Reindeer>,
  seconds: u32) -> u32 
{
  let mut curr_reindeers = reindeers.clone();

  for second in 0..seconds {
    for reindeer in curr_reindeers.iter_mut() {
      if !reindeer.is_running && reindeer.rest_until == second {
        reindeer.is_running = true;
        reindeer.run_until = second + reindeer.time_running;
      }

      if reindeer.is_running && reindeer.run_until == second {
        reindeer.is_running = false;
        reindeer.rest_until = second + reindeer.time_resting;
      }

      if reindeer.is_running {
        reindeer.distance_ran += reindeer.speed;
      }
    }
  }
  let mut max_distance: u32 = 0;
  for reindeer in curr_reindeers.iter() {
    if reindeer.distance_ran > max_distance { max_distance = reindeer.distance_ran }
  }

  return max_distance;
}

fn get_leading_reindeers(reindeers: &&Vec<Reindeer>) -> Vec<String> {
  let mut leading_reindeers: Vec<String> = Vec::new();
  let mut max_distance: u32 = 0;
  
  for reindeer in reindeers.iter() {
    if reindeer.distance_ran > max_distance { max_distance = reindeer.distance_ran }
  }

  for reindeer in reindeers.iter() {
    if reindeer.distance_ran == max_distance {
      leading_reindeers.push(reindeer.name.clone());
    }
  }
  
  return leading_reindeers;
}


fn calculate_highest_points(
  reindeers: &mut Vec<Reindeer>,
  seconds: u32) -> u32 
{
  let mut curr_reindeers = reindeers.clone();
  let mut points = curr_reindeers
    .iter()
    .map(|reindeer| (reindeer.name.clone(), 0))
    .collect::<HashMap<String, u32>>();

  for second in 0..seconds {
    for reindeer in curr_reindeers.iter_mut() {
      if !reindeer.is_running && reindeer.rest_until == second {
        reindeer.is_running = true;
        reindeer.run_until = second + reindeer.time_running;
      }

      if reindeer.is_running && reindeer.run_until == second {
        reindeer.is_running = false;
        reindeer.rest_until = second + reindeer.time_resting;
      }

      if reindeer.is_running {
        reindeer.distance_ran += reindeer.speed;
      }
    }

    let reindeers_in_the_lead = get_leading_reindeers(&&curr_reindeers);
    reindeers_in_the_lead.iter().for_each(|reindeer| 
      *points.get_mut(reindeer).unwrap() += 1
    );
  }

  return *points.values().max().unwrap();
}

fn main() {
  let contents = fs::read_to_string("./data/q14.txt")
    .expect("Should have been able to read file");

  let re = Regex::new("([A-Za-z]+) can fly ([\\d]+) km/s \
    for ([\\d]+) seconds, but then must rest for ([\\d]+) seconds.").unwrap();
  let mut reindeers: Vec<Reindeer> = Vec::new();

  for line in contents.split("\n") {
    let captures = re.captures(line);
    match captures {
      Some(matched_groups) => {
        let reindeer_name = matched_groups[1].to_string();
        let speed = matched_groups[2].parse::<u32>().unwrap();
        let time_running = matched_groups[3].parse::<u32>().unwrap();
        let time_resting = matched_groups[4].parse::<u32>().unwrap();

        let reindeer = Reindeer {
          name: reindeer_name,
          speed: speed, 
          time_running: time_running, 
          time_resting: time_resting,
          ..Default::default()
        };
        reindeers.push(reindeer);
      },

      None => {
        println!("Was unable to match line: {}", line);
      }
    }
  }

  println!("Part 1: {}", calculate_longest_distance_travelled(&mut reindeers, 2503));
  println!("Part 2: {}", calculate_highest_points(&mut reindeers, 2503));
}