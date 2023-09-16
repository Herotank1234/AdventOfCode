use std::fs;
use regex::Regex;

struct Fold {
  axis: char,
  fold_point: usize
}

impl Fold {
  fn new(axis: char, fold_point: usize) -> Self {
    return Self { axis, fold_point };
  }
}

fn simulate_fold(grid: &Vec<Vec<bool>>, fold: &Fold) -> Vec<Vec<bool>> {
  let mut next_grid: Vec<Vec<bool>> = Vec::new();

  match fold.axis {
    'x' => {
      next_grid = vec![vec![false; fold.fold_point]; grid.len()];
      for y in 0..grid.len() {
        for x in 0..fold.fold_point {
          if !grid[y][x] { continue; }
          next_grid[y][x] = grid[y][x];
        }
      }

      for y in 0..grid.len() {
        for x in (fold.fold_point + 1..(fold.fold_point * 2) + 1).rev() {
          if x >= grid[0].len() { continue; }
          if !grid[y][x] { continue; }
          next_grid[y][(fold.fold_point * 2) - x] = grid[y][x];
        }
      }
    },
    'y' => {
      next_grid = vec![vec![false; grid[0].len()]; fold.fold_point];
      for y in 0..fold.fold_point {
        for x in 0..grid[0].len() {
          if !grid[y][x] { continue; }
          next_grid[y][x] = grid[y][x];
        }
      }

      for y in (fold.fold_point + 1..(fold.fold_point * 2) + 1).rev() {
        for x in 0..grid[0].len() {
          if y >= grid.len() { continue; }
          if !grid[y][x] { continue; }
          next_grid[(fold.fold_point * 2) - y][x] = grid[y][x];
        }
      }
    },
    _ => ()
  }

  return next_grid;
}

fn count_stars_after_first_fold(grid: &Vec<Vec<bool>>, folds: &Vec<Fold>) -> u32 {
  let mut curr_grid = grid.clone();

  for i in 0..1 {
    curr_grid = simulate_fold(&curr_grid, &folds[i]);
  }

  return curr_grid.iter().fold(0, |acc, row| 
    acc + row.iter().fold(0, |acc, col| acc + *col as u32));
}

fn get_code_after_all_folds(grid: &Vec<Vec<bool>>, folds: &Vec<Fold>) -> String {
  let mut curr_grid = grid.clone();

  for fold in folds.iter() {
    curr_grid = simulate_fold(&curr_grid, &fold);
  }

  let mut res = String::new();

  for row in curr_grid.iter() {
    let row_str: String = row.iter()
      .map(|m| if *m {'#'} else {'.'}).collect();
    res.insert(res.len(), '\n');
    res.insert_str(res.len(), row_str.as_str());
  }

  return res;
}

fn main() {
  let contents = fs::read_to_string("./data/q13.txt")
    .expect("Should have been able to read file");

  let mut points: Vec<(usize, usize)> = Vec::new();
  let mut folds: Vec<Fold> = Vec::new();
  let mut is_folds = false;
  let re = Regex::new("fold along (x|y)=([\\d]+)\\s?").unwrap();

  for line in contents.split('\n') {
    if line == "" {
      is_folds = true;
      continue;
    }

    if !is_folds {
      let point: Vec<usize> = line.split(',').map(|val| val.parse::<usize>().unwrap())
        .collect();
      points.push((point[0], point[1]));
    } else {
      let captures = re.captures(line);
      match captures {
        Some(groups) => folds.push(Fold::new(
          groups[1].chars().nth(0).unwrap(), 
          groups[2].parse::<usize>().unwrap())),
        None => println!("Unable to match line: {}", line)
      }
    }
  }

  let mut max_x = 0;
  let mut max_y = 0;
  for (x, y) in points.iter() {
    max_x = std::cmp::max(max_x, *x);
    max_y = std::cmp::max(max_y, *y);
  }

  let mut grid: Vec<Vec<bool>> = vec![vec![false; (max_x + 1) as usize];
    (max_y + 1) as usize];
  for (x, y) in points.iter() {
    grid[*y][*x] = true;
  }

  println!("Part 1: {}", count_stars_after_first_fold(&grid, &folds));
  println!("Part 2: {}", get_code_after_all_folds(&grid, &folds));
}