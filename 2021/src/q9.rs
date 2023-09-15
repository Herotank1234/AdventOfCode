use std::fs;
use std::collections::VecDeque;

fn in_bounds(i: isize, j: isize, length: isize, width: isize) -> bool {
  return i >= 0 && i < length && j >= 0 && j < width;
}

fn get_low_points(grid: &Vec<Vec<u32>>) -> Vec<((usize, usize), u32)> {
  let mut low_points: Vec<((usize, usize), u32)> = Vec::new();
  let directions: Vec<(isize, isize)> = vec![(-1, 0), (0, 1), (1, 0), (0, -1)];

  let length = grid.len() as isize;
  let width = grid[0].len() as isize;

  for i in 0..length {
    for j in 0..width {

      if grid[i as usize][j as usize] == 9 { continue; }
      let mut higher_neighbours: u32 = 0;
      let mut neighbours: u32 = 0;

      for (di, dj) in directions.iter() {
        let next_i = i + di;
        let next_j = j + dj;

        if in_bounds(next_i, next_j, length, width) {
          neighbours += 1;
          if grid[next_i as usize][next_j as usize] > grid[i as usize][j as usize] {
            higher_neighbours += 1;
          }
        }
      }

      if higher_neighbours == neighbours { 
        low_points.push(((i as usize, j as usize), grid[i as usize][j as usize])); 
      }
    }
  }
  return low_points;
}

fn calculate_total_risk_level(grid: &Vec<Vec<u32>>) -> u32 {
  let low_points: Vec<((usize, usize), u32)> = get_low_points(grid);
  return low_points.iter().map(|((_, _), val)| val + 1).sum();
}

fn get_basin_size(grid: &Vec<Vec<u32>>, initial_i: &usize, initial_j: &usize) -> u32 {
  let directions: Vec<(isize, isize)> = vec![(-1, 0), (0, 1), (1, 0), (0, -1)];

  let length = grid.len() as isize;
  let width = grid[0].len() as isize;

  let mut to_visit: VecDeque<(usize, usize)> = VecDeque::new();
  to_visit.push_back((*initial_i, *initial_j));

  let mut visited = vec![vec![false; width as usize]; length as usize];

  while !to_visit.is_empty() {
    let (curr_i, curr_j) = to_visit.pop_front().unwrap();
    if grid[curr_i][curr_j] == 9 { continue; }

    if visited[curr_i][curr_j] { continue; }
    visited[curr_i][curr_j] = true;

    for (di, dj) in directions.iter() {
      let next_i = curr_i as isize + di;
      let next_j = curr_j as isize + dj;
      if in_bounds(next_i, next_j, length, width) && 
        !visited[next_i as usize][next_j as usize] &&
        grid[next_i as usize][next_j as usize] > grid[curr_i][curr_j] 
      {
        to_visit.push_back((next_i as usize, next_j as usize));
      }
    }
  }

  return visited.iter().fold(0, |acc, row| 
    acc + row.iter().fold(0, |acc1, mark| acc1 + (*mark as u32)));
}

fn get_product_of_top_three_basin(grid: &Vec<Vec<u32>>) -> u32 {
  let low_points: Vec<((usize, usize), u32)> = get_low_points(grid);

  let mut basin_sizes: Vec<u32> = low_points.iter()
    .map(|((i, j), _)| get_basin_size(grid, i, j))
    .collect();
  basin_sizes.sort_by(|a, b| b.cmp(a));

  return basin_sizes[0] * basin_sizes[1] * basin_sizes[2];
}

fn main() {
  let contents = fs::read_to_string("./data/q9.txt")
    .expect("Should have been able to read file");
  let grid: Vec<Vec<u32>> = contents.split('\n')
    .map(|line| 
      line.chars()
        .map(|val| val.to_digit(10).unwrap())
        .collect::<Vec<u32>>()
    )
    .collect();

  println!("Part 1: {}", calculate_total_risk_level(&grid));
  println!("Part 2: {}", get_product_of_top_three_basin(&grid));
}