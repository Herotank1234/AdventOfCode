use std::fs;
use std::collections::HashMap;
use std::cmp;
use regex::Regex;

#[derive(Debug)]
struct Vent {
  l_x: u32,
  l_y: u32,
  u_x: u32,
  u_y: u32
}

impl Vent {
  fn new(l_x: u32, l_y: u32, u_x: u32, u_y: u32) -> Self {
    return Self { l_x, l_y, u_x, u_y };
  }
}

fn count_overlapping_hznt_vrt_points(vents: &Vec<Vent>) -> u32 {
  let mut points: HashMap<(u32, u32), u32> = HashMap::new();

  for vent in vents {
    if vent.l_x != vent.u_x && vent.l_y != vent.u_y { continue; }

    if vent.l_x == vent.u_x {
      let lower = cmp::min(vent.l_y, vent.u_y);
      let upper = cmp::max(vent.l_y, vent.u_y);

      for y in lower..upper + 1 {
        let entry = points.entry((vent.l_x, y)).or_insert(0);
        *entry += 1;
      }
    } else {
      let lower = cmp::min(vent.l_x, vent.u_x);
      let upper = cmp::max(vent.l_x, vent.u_x);

      for x in lower..upper + 1 {
        let entry = points.entry((x, vent.l_y)).or_insert(0);
        *entry += 1;
      }
    }
  }

  return points.iter()
    .filter(|((_, _), v)| **v > 1)
    .collect::<Vec<(&(u32, u32), &u32)>>()
    .len() as u32;
}

fn count_overlapping_hznt_vrt_diag_points(vents: &Vec<Vent>) -> u32 {
  let mut points: HashMap<(u32, u32), u32> = HashMap::new();

  for vent in vents {
    if vent.l_x != vent.u_x && vent.l_y != vent.u_y { 
      let mut x = vent.l_x;
      let mut y = vent.l_y;

      while x != vent.u_x && y != vent.u_y {
        let entry = points.entry((x, y)).or_insert(0);
        *entry += 1;
        if vent.l_x < vent.u_x { x += 1 } else { x -= 1 };
        if vent.l_y < vent.u_y { y += 1 } else { y -= 1 };
      }
      let entry = points.entry((x, y)).or_insert(0);
      *entry += 1;
    } else if vent.l_x == vent.u_x {
      let lower = cmp::min(vent.l_y, vent.u_y);
      let upper = cmp::max(vent.l_y, vent.u_y);

      for y in lower..upper + 1 {
        let entry = points.entry((vent.l_x, y)).or_insert(0);
        *entry += 1;
      }
    } else {
      let lower = cmp::min(vent.l_x, vent.u_x);
      let upper = cmp::max(vent.l_x, vent.u_x);

      for x in lower..upper + 1 {
        let entry = points.entry((x, vent.l_y)).or_insert(0);
        *entry += 1;
      }
    }
  }

  return points.iter()
    .filter(|((_, _), v)| **v > 1)
    .collect::<Vec<(&(u32, u32), &u32)>>()
    .len() as u32;
}

fn main() {
  let contents = fs::read_to_string("./data/q5.txt")
    .expect("Should have been able to read file");
  let re = Regex::new("([\\d]+),([\\d]+) -> ([\\d]+),([\\d]+)\\s?").unwrap();
  
  let mut vents: Vec<Vent> = Vec::new();

  for str in contents.split('\n') {
    let captures = re.captures(str);
    match captures {
      Some(groups) => {
        vents.push(Vent::new(groups[1].parse::<u32>().unwrap(), 
        groups[2].parse::<u32>().unwrap(), 
        groups[3].parse::<u32>().unwrap(), 
        groups[4].parse::<u32>().unwrap()
        ));
      },
      None => {
        println!("Unable to match line: {}", str);
      }
    }
  }
    
  println!("Part 1: {}", count_overlapping_hznt_vrt_points(&vents));
  println!("Part 2: {}", count_overlapping_hznt_vrt_diag_points(&vents));
}