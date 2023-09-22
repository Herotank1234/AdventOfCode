use std::fs;

fn parse_index(str: &String) -> usize {
  let mut index: usize = 0;
  let base: usize = 2;
  let mut expo: i32 = 8;
  for c in str.chars() {
    if c == '#' { index += base.pow(expo as u32); }
    expo -= 1;
  }

  return index;
}

fn in_bounds(i: isize, j: isize, length: isize, width: isize) -> bool {
  return i >= 0 && i < length && j >= 0 && j < width;
}

fn enhance(algo: &Vec<char>, grid: &Vec<Vec<char>>, fill: char) -> Vec<Vec<char>> {
  let length = grid.len();
  let width = grid[0].len();
  let mut result = vec![vec!['.'; width + 2]; length + 2];
  let directions: Vec<(isize, isize)> = vec![
    (-1, -1), (-1, 0), (-1, 1),
    (0, -1), (0, 0), (0, 1),
    (1, -1), (1, 0), (1, 1)
  ];

  for i in 0..(length + 2) as isize {
    for j in 0..(width + 2) as isize {
      let mut index_str = String::new();

      for (di, dj) in directions.iter() {
        let next_i = i + di;
        let next_j = j + dj;
        let char_to_insert;

        if in_bounds(next_i - 1, next_j - 1, length as isize, width as isize) {
          char_to_insert = grid[(next_i - 1) as usize][(next_j - 1) as usize];
        } else {
          char_to_insert = fill;
        }

        index_str.insert(index_str.len(), char_to_insert);
      }
      let index = parse_index(&index_str);
      let next_char = algo[index];
      result[i as usize][j as usize] = next_char;
    }
  }

  return result;
}

fn count_lit(grid: &Vec<Vec<char>>) -> u32 {
  return grid.iter().map(|row| row.iter()
    .filter(|chr| **chr == '#')
    .collect::<Vec<&char>>().len() as u32)
  .sum();
}

fn enhance_and_count_lit(algo: &Vec<char>, grid: &Vec<Vec<char>>, repititions: u32) -> u32 {
  let mut curr_grid = grid.clone();

  let empty_fill = algo[0];
  let empty_fill_index = parse_index(&algo[0].to_string().repeat(9));
  let empty_fill_rev = algo[empty_fill_index];
  let mut is_empty_fill = false;

  for _ in 0..repititions {
    let curr_fill = if is_empty_fill { empty_fill } else { empty_fill_rev };
    curr_grid = enhance(algo, &curr_grid, curr_fill);
    is_empty_fill = !is_empty_fill;
  }

  return count_lit(&curr_grid);
}

fn main() {
  let contents = fs::read_to_string("./data/q20.txt")
    .expect("Should have been able to read file");
  let lines: Vec<&str> = contents.split('\n').collect();

  let image_enhance_algo: Vec<char> = lines[0].chars().collect();
  let mut initial_grid: Vec<Vec<char>> = Vec::new();

  for i in 2..lines.len() {
    let row: Vec<char> = lines[i].chars().collect();
    initial_grid.push(row);
  }

  println!("Part 1: {}", enhance_and_count_lit(&image_enhance_algo, &initial_grid, 2));
  println!("Part 2: {}", enhance_and_count_lit(&image_enhance_algo, &initial_grid, 50));
}