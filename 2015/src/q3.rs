use std::fs;
use std::collections::HashSet;
use std::collections::HashMap;

fn visit_houses(directions: &str) -> usize {
  let direction_map: HashMap<char, (i8, i8)> = HashMap::from([
    ('^', (-1, 0)),
    ('>', (0, 1)),
    ('v', (1, 0)),
    ('<', (0, -1))
  ]);
  let mut visited: HashSet<(i8, i8)> = HashSet::new();
  let mut curr_pos: (i8, i8) = (0, 0);
  visited.insert(curr_pos);

  for direction in directions.chars() {
    let (dx, dy) = direction_map.get(&direction).unwrap();
    let (x, y) = curr_pos;
    curr_pos = (x + dx, y + dy);
    visited.insert(curr_pos);
  }

  return visited.len();
}

fn visit_houses_with_robot(directions: &str) -> usize {
  let direction_map: HashMap<char, (i8, i8)> = HashMap::from([
    ('^', (-1, 0)),
    ('>', (0, 1)),
    ('v', (1, 0)),
    ('<', (0, -1))
  ]);
  let mut visited: HashSet<(i8, i8)> = HashSet::new();
  let mut curr_pos_santa: (i8, i8) = (0, 0);
  let mut curr_pos_robot: (i8, i8) = (0, 0);
  visited.insert(curr_pos_santa);
  let mut is_santa: bool = true;

  for direction in directions.chars() {
    let (dx, dy) = direction_map.get(&direction).unwrap();
    let (x, y) = if is_santa {curr_pos_santa} else {curr_pos_robot};
    if is_santa {
      curr_pos_santa = (x + dx, y + dy)
    } else {
      curr_pos_robot = (x + dx, y + dy)
    };
    visited.insert((x, y));
    is_santa = !is_santa
  }

  return visited.len();
}

fn main() {
  let directions = fs::read_to_string("./data/q3.txt")
    .expect("Should have been able to read file");
  println!("Part 1: {}", visit_houses(&directions));
  println!("Part 2: {}", visit_houses_with_robot(&directions));
}