use std::fs;
use std::cmp::max;
use regex::Regex;

struct Target {
  lower_x: i32,
  upper_x: i32,
  lower_y: i32,
  upper_y: i32
}

impl Target {
  fn new(lower_x: i32, upper_x: i32, lower_y: i32, upper_y: i32) -> Self {
    return Self { lower_x, lower_y, upper_x, upper_y };
  }
}

struct Probe {
  x: i32,
  y: i32,
  x_vel: i32,
  y_vel: i32
}

impl Probe {
  fn new(x_vel: i32, y_vel: i32) -> Self {
    return Self { x: 0, y: 0, x_vel, y_vel };
  }

  fn step(&mut self) {
    self.x += self.x_vel;
    self.y += self.y_vel;
    if self.x_vel != 0 { self.x_vel -= self.x_vel / self.x_vel.abs(); }
    self.y_vel -= 1;
  }

  fn get_y(&self) -> i32 {
    return self.y;
  }

  fn is_hit(&self, target: &Target) -> bool {
    return target.lower_x <= self.x && self.x <= target.upper_x &&
      target.lower_y <= self.y && self.y <= target.upper_y;
  }

  fn is_missed(&self, target: &Target) -> bool {
    return self.x > target.upper_x || self.y < target.lower_y;
  }
}

fn get_largest_y(target: &Target) -> i32 {
  let mut max_y: i32 = 0;
  let max_x_vel = target.upper_x;

  for x in 0..max_x_vel + 1 {
    for y in 0..1000 {
      let mut probe = Probe::new(x, y);
      let mut curr_max_y = 0;

      while !(probe.is_hit(target) || probe.is_missed(target)) {
        probe.step();
        curr_max_y = max(curr_max_y, probe.get_y());
      }

      if probe.is_hit(target) { max_y = max(max_y, curr_max_y); }
    }
  }

  return max_y;
}

fn count_unique_initial_vel_that_hit(target: &Target) -> u32 {
  let max_x_vel = target.upper_x;
  let mut vel_hits = 0;

  for x in 0..max_x_vel + 1 {
    for y in -1000..1000 {
      let mut probe = Probe::new(x, y);

      while !(probe.is_hit(target) || probe.is_missed(target)) {
        probe.step();
      }

      if probe.is_hit(target) { vel_hits += 1; }
    }
  }

  return vel_hits;
}

fn main() {
  let contents = fs::read_to_string("./data/q17.txt")
    .expect("Should have been able to read file");

  let re = Regex::new("target area: x=(-?[\\d]+)..(-?[\\d]+), y=(-?[\\d]+)..(-?[\\d]+)\\s?").unwrap();
  let captures = re.captures(&contents);

  let target: Target;

  match captures {
    Some(groups) => {
      let lower_x = groups[1].parse::<i32>().unwrap();
      let upper_x = groups[2].parse::<i32>().unwrap();
      let lower_y = groups[3].parse::<i32>().unwrap();
      let upper_y = groups[4].parse::<i32>().unwrap();
      target = Target::new(lower_x, upper_x, lower_y, upper_y);
    }
    None => {
      println!("Was unable to match line: {}", contents);
      target = Target::new(0, 0, 0, 0);
    }
  }

  println!("Part 1: {}", get_largest_y(&target));
  println!("Part 2: {}", count_unique_initial_vel_that_hit(&target));
}