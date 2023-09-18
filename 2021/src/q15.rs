use std::fs;
use std::cmp::Reverse;
use std::collections::HashSet;
use std::collections::BinaryHeap;

fn in_bounds(i: isize, j: isize, length: isize, width: isize) -> bool {
  return i >= 0 && i < length && j >= 0 && j < width;
}

fn get_lowest_risk_level(grid: &Vec<Vec<u32>>) -> u32 {
  let length = grid.len() as isize;
  let width = grid[0].len() as isize;
  let finish = ((length - 1) as usize, (width - 1) as usize);
  let mut to_visit: BinaryHeap<Reverse<(u32, usize, usize)>> 
    = BinaryHeap::new();
  to_visit.push(Reverse((0, 0, 0)));

  let mut visited: HashSet<(usize, usize)> = HashSet::new();

  let directions: Vec<(isize, isize)> = vec![(-1, 0), (0, 1), (1, 0), (0, -1)];

  while !to_visit.is_empty() {
    let (risk, i, j) = to_visit.pop().unwrap().0;
    if (i, j) == finish { return risk; }

    if visited.contains(&(i, j)) { continue; }
    visited.insert((i, j));

    for (di, dj) in directions.iter() {
      let next_i = i as isize + di;
      let next_j = j as isize + dj;
      if in_bounds(next_i, next_j, length, width) && 
        !visited.contains(&(next_i as usize, next_j as usize)) {
        let next_risk = risk + grid[next_i as usize][next_j as usize];
        to_visit.push(Reverse((next_risk, next_i as usize, next_j as usize)));
      }
    }

  }

  return 0;
}

fn extend(grid: &Vec<Vec<u32>>) -> Vec<Vec<u32>> {
  let length = grid.len();
  let width = grid[0].len();

  let mut next_grid = vec![vec![0; width * 5]; length * 5];

  for i in 0..length * 5 {
    for j in 0..width * 5 {
      next_grid[i][j] = (((grid[i % length][j % width] + 
        (i / length) as u32 + (j / width) as u32) - 1) % 9) + 1;
    }
  }

  return next_grid;
}

fn main() {
  let contents = fs::read_to_string("./data/q15.txt")
    .expect("Should have been able to read file");

  let grid: Vec<Vec<u32>> = contents.split('\n')
    .map(|row| row.chars()
      .map(|val| val.to_digit(10).unwrap())
      .collect())
    .collect();

  println!("Part 1: {}", get_lowest_risk_level(&grid));
  println!("Part 2: {}", get_lowest_risk_level(&extend(&grid)));
}