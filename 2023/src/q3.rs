use std::fs;
use std::collections::HashSet;

#[derive(Debug)]
struct Number {
  num: u32,
  y: usize,
  x1: usize,
  x2: usize
}

fn get_num_positions(grid: &&Vec<Vec<char>>) -> Vec<Number> {
  let mut positions: Vec<Number> = Vec::new();
  let mut curr_number: String = String::new();
  let mut in_number: bool = false;
  let mut start_x: usize = 0;

  for y in 0..grid.len() {
    for x in 0.. grid[0].len() {
      match grid[y][x] {
        '0'..='9' => {
          if !in_number { 
            curr_number.clear();
            in_number = true; 
            start_x = x;
          }
          curr_number.push(grid[y][x]);
        }

        '.' => {
          if in_number {
            in_number = false;
            positions.push(Number { num: curr_number.parse::<u32>().unwrap(), y,
              x1: start_x, x2: x });
          }
        }
        _ => { 
          if in_number {
            in_number = false;
            positions.push(Number { num: curr_number.parse::<u32>().unwrap(), y,
              x1: start_x, x2: x });
          } else {
            continue;
          }
        }
      }
    }

    if in_number {
      in_number = false;
      positions.push(Number { num: curr_number.parse::<u32>().unwrap(), y,
        x1: start_x, x2: grid[0].len() });
    }
  }

  return positions;
}

fn is_symbol(chr: char) -> bool {
  match chr {
    '0'..='9' | '.' => return false,
    _ => return true
  }
}

fn is_next_to_symbol(number: &Number, grid: &Vec<Vec<char>>) -> bool {
  
  if number.y > 0 {
    let lower = if number.x1 > 0 { number.x1 - 1 } else { number.x1 };
    let upper = if number.x2 < grid[0].len() - 1 { number.x2 + 1 } else { number.x2 };
    for x in lower..upper{
      if is_symbol(grid[number.y - 1][x]) { return true }
    }
  }

  if number.x1 > 0 {
    if is_symbol(grid[number.y][number.x1 - 1]) { return true }
  }

  if number.x2 < grid[0].len() {
    if is_symbol(grid[number.y][number.x2]) { return true }
  }

  if number.y < grid.len() - 1 {
    let lower = if number.x1 > 0 { number.x1 - 1 } else { number.x1 };
    let upper = if number.x2 < grid[0].len() - 1 { number.x2 + 1 } else  { number.x2 };
    for x in lower..upper{
      if is_symbol(grid[number.y + 1][x]) { return true }
    }
  }

  return false;
}

fn sum_part_numbers(grid: &Vec<Vec<char>>) -> u32 {
  let mut total_sum: u32 = 0;
  
  let num_positions = get_num_positions(&grid);

  for number in num_positions.iter() {
    if is_next_to_symbol(number, grid) {
      total_sum += number.num; 
    }
  }

  return total_sum;
}

fn in_range(y: isize, x: isize, max_y: isize, max_x: isize) -> bool {
  return y >= 0 && y < max_y && x >= 0 && x < max_x;
}

fn get_number(grid: &Vec<Vec<char>>, y: usize, x: usize) -> (usize, usize, u32) {
  let mut num: String = String::new();
  let curr_y: isize = y as isize;
  let mut curr_x: isize = x as isize;
  let h: isize = grid.len() as isize;
  let w: isize = grid[0].len() as isize;

  while in_range(curr_y, curr_x - 1, h, w) && grid[y][(curr_x - 1) as usize] >= '0' && 
    grid[y][(curr_x - 1) as usize] <= '9' 
  {
    curr_x -= 1;
  }

  let start_x = curr_x as usize;

  while curr_x < w && grid[y][curr_x as usize] >= '0' && grid[y][curr_x as usize] <= '9' {
    num.push(grid[y][curr_x as usize]);
    curr_x += 1;
  }

  return (start_x, y, num.parse::<u32>().unwrap());
}

fn get_adjacent_numbers(grid: &Vec<Vec<char>>, y: usize, x: usize) -> Vec<u32> {
  let mut adjacent_numbers: HashSet<(usize, usize, u32)> = HashSet::new();
  let curr_y: isize = y as isize;
  let curr_x: isize = x as isize;

  for next_y in curr_y - 1..curr_y + 2 {
    for next_x in curr_x - 1..curr_x + 2 {
      if in_range(next_y, next_x, grid[0].len() as isize, grid.len() as isize) {
        match grid[next_y as usize][next_x as usize] {
          '0'..='9' => {
            adjacent_numbers.insert(get_number(grid, next_y as usize, next_x as usize));
          }
          _ => continue
        }
      }
    }
  }

  return adjacent_numbers.iter().map(|(_, _, n)| *n).collect::<Vec<u32>>();
}

fn sum_gear_ratios(grid: &Vec<Vec<char>>) -> u32 {
  let mut gear_ratios: u32 = 0;

  for y in 0..grid.len() {
    for x in 0..grid[0].len() {
      match grid[y][x] {
        '*' => {
          let adjacent_numbers = get_adjacent_numbers(grid, y, x);
          if adjacent_numbers.len() == 2 { 
            gear_ratios += adjacent_numbers[0] * adjacent_numbers[1];
          }
        }

        _ => continue
      }
    }
  }

  return gear_ratios;
}

fn main() {
  let contents = fs::read_to_string("./data/q3.txt")
    .expect("Should have been able to read file");
  let grid: Vec<Vec<char>> = contents.split('\n')
    .map(|line| line.chars().collect()).collect();

  println!("Part 1: {}", sum_part_numbers(&grid));
  println!("Part 2: {}", sum_gear_ratios(&grid));
}