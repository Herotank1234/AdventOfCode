use std::fs;

fn in_bounds(i: isize, j: isize, length: isize, width: isize) -> bool {
  return i >= 0 && i < length && j >= 0 && j < width;
}

fn count_flashes_after_step(grid: &mut Vec<Vec<u32>>) -> u32 {
  for row in grid.iter_mut() {
    for col in row.iter_mut() {
      *col += 1;
    }
  }

  let length = grid.len() as isize;
  let width = grid[0].len() as isize;
  let mut flashed = vec![vec![false; width as usize]; length as usize];
  let directions: Vec<(isize, isize)> = vec![
    (-1, -1), (-1, 0), (-1, 1),
    (0, -1),           (0, 1), 
    (1, -1),  (1, 0),  (1, 1)];

  loop {
    let mut curr_flashed: i32 = 0;

    for i in 0..length as isize {
      for j in 0..width as isize {
        if grid[i as usize][j as usize] <= 9 { continue; }
        
        for (di, dj) in directions.iter() {
          let next_i = i + di;
          let next_j = j + dj;
          if in_bounds(next_i, next_j, length, width) && 
            !flashed[next_i as usize][next_j as usize] {
            grid[next_i as usize][next_j as usize] += 1;
          }
        }

        flashed[i as usize][j as usize] = true;
        grid[i as usize][j as usize] = 0;
        curr_flashed += 1;
      }
    }

    if curr_flashed == 0 { break; }
  }

  return flashed.iter()
    .fold(0,|acc, row| 
      acc + row.iter().fold(0, |acc1, col| acc1 + (*col as u32)));
}

fn count_flashes_after_100_steps(grid: &Vec<Vec<u32>>) -> u32 {
  let mut curr_grid = grid.clone();
  let mut total_flashes = 0;

  for _ in 0..100 {
    let flashes = count_flashes_after_step(&mut curr_grid);
    total_flashes += flashes;
  }

  return total_flashes;
}

fn is_all_flashed_after_step(grid: &mut Vec<Vec<u32>>) -> bool {
  for row in grid.iter_mut() {
    for col in row.iter_mut() {
      *col += 1;
    }
  }

  let length = grid.len() as isize;
  let width = grid[0].len() as isize;
  let mut flashed = vec![vec![false; width as usize]; length as usize];
  let directions: Vec<(isize, isize)> = vec![
    (-1, -1), (-1, 0), (-1, 1),
    (0, -1),           (0, 1), 
    (1, -1),  (1, 0),  (1, 1)];

  loop {
    let mut curr_flashed: i32 = 0;

    for i in 0..length as isize {
      for j in 0..width as isize {
        if grid[i as usize][j as usize] <= 9 { continue; }
        
        for (di, dj) in directions.iter() {
          let next_i = i + di;
          let next_j = j + dj;
          if in_bounds(next_i, next_j, length, width) && 
            !flashed[next_i as usize][next_j as usize] {
            grid[next_i as usize][next_j as usize] += 1;
          }
        }

        flashed[i as usize][j as usize] = true;
        grid[i as usize][j as usize] = 0;
        curr_flashed += 1;
      }
    }

    if curr_flashed == 0 { break; }
  }

  return flashed.iter()
    .all(|row| row.iter().all(|flash| *flash));
}

fn find_first_step_to_flash_all(grid: &Vec<Vec<u32>>) -> u32 {
  let mut curr_grid = grid.clone();
  let mut steps = 0;
  loop {
    let all_flashed = is_all_flashed_after_step(& mut curr_grid);
    steps += 1;
    if all_flashed { return steps; }
  }
}

fn main() {
  let contents = fs::read_to_string("./data/q11.txt")
    .expect("Should have been able to read file");

  let grid: Vec<Vec<u32>> = contents.split('\n')
    .map(|str| str.chars()
      .map(|val| val.to_digit(10).unwrap())
      .collect::<Vec<u32>>())
    .collect();

  println!("Part 1: {}", count_flashes_after_100_steps(&grid));
  println!("Part 2: {}", find_first_step_to_flash_all(&grid));
}