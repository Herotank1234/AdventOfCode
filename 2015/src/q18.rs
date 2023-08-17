use std::fs;

fn count_on_neighbours(grid: &Vec<Vec<char>>, i: isize, j: isize) -> u32 {
  let mut on_neighbours: u32 = 0;
  let directions: Vec<(isize, isize)> = vec![
    (-1, -1), (-1, 0), (-1, 1),
    ( 0, -1),          ( 0, 1),
    ( 1, -1), ( 1, 0), ( 1, 1)
  ];

  for (di, dj) in directions.iter() {
    let next_i = i + di;
    let next_j = j + dj;
    if next_i >= 0 && next_i < grid.len() as isize && 
      next_j >= 0 && next_j < grid[0].len() as isize 
    {
      if grid[next_i as usize][next_j as usize] == '#' { on_neighbours += 1 }
    }
  }

  return on_neighbours;
}

fn animate(grid: &Vec<Vec<char>>) -> Vec<Vec<char>> {
  let curr_grid = grid.clone();
  let mut next_grid: Vec<Vec<char>> = Vec::new();

  for i in 0..curr_grid.len() {
    let mut next_row: Vec<char> = Vec::new();

    for j in 0..curr_grid[0].len() {
      let on_neighbours = count_on_neighbours(&curr_grid, i as isize, j as isize);
      match curr_grid[i][j] {
        '#' => {
          if on_neighbours == 2 || on_neighbours == 3 {
            next_row.push('#');
          } else {
            next_row.push('.');
          }
        },

        '.' => {
          if on_neighbours == 3 {
            next_row.push('#');
          } else {
            next_row.push('.');
          }
        }, 

        _ => println!("Character in grid not matched: {}", curr_grid[i][j])
      }
    }

    next_grid.push(next_row);
  }

  return next_grid;
}

fn animate_and_count_lights_on(grid: &Vec<Vec<char>>, reps: u32) -> usize {
  let mut final_grid = grid.clone();
  for _ in 0..reps {
    final_grid = animate(&final_grid);
  }

  return final_grid
    .iter()
    .map(|row| row
      .iter()
      .filter(|light| **light == '#')
      .collect::<Vec<&char>>()
      .len()
    )
    .sum();
}

fn turn_corners_on(grid: &Vec<Vec<char>>) -> Vec<Vec<char>> {
  let mut curr_grid = grid.clone();
  let length = curr_grid.len();
  let width = curr_grid[0].len();

  curr_grid[0][0] = '#';
  curr_grid[0][width - 1] = '#';
  curr_grid[length - 1][0] = '#';
  curr_grid[length - 1][width - 1] = '#';

  return curr_grid;
}

fn animate_and_count_lights_on_with_corners_stuck(grid: &Vec<Vec<char>>, reps: u32) -> usize {
  let mut final_grid = grid.clone();
  for _ in 0..reps {
    final_grid = turn_corners_on(&final_grid);
    final_grid = animate(&final_grid);
  }

  final_grid = turn_corners_on(&final_grid);

  return final_grid
    .iter()
    .map(|row| row
      .iter()
      .filter(|light| **light == '#')
      .collect::<Vec<&char>>()
      .len()
    )
    .sum();
}

fn main() {
  let contents = fs::read_to_string("./data/q18.txt")
    .expect("Should have been able to read file");
  let grid: Vec<Vec<char>> = contents
    .split("\n")
    .map(|line| line.chars().collect::<Vec<char>>())
    .collect::<Vec<Vec<char>>>();

  println!("Part 1: {}", animate_and_count_lights_on(&grid, 100));
  println!("Part 2: {}", animate_and_count_lights_on_with_corners_stuck(&grid, 100));
}