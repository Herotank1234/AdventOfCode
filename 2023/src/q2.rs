use std::fs;

const MAX_RED: u32 = 12;
const MAX_GREEN: u32 = 13;
const MAX_BLUE: u32 = 14;

#[derive(Debug)]
struct Set {
  red: u32,
  green: u32,
  blue: u32
}

impl Set {
  pub fn new(red: u32, green: u32, blue: u32) -> Self {
    return Self { red, green, blue };
  }

  pub fn is_valid(&self) -> bool {
    return self.red <= MAX_RED && self.green <= MAX_GREEN && self.blue <= MAX_BLUE;
  }
}

#[derive(Debug)]
struct Game {
  id: u32,
  sets: Vec<Set>
}

impl Game {
  pub fn new(id: u32) -> Self {
    return Self { id, sets: Vec::new() };
  }

  pub fn add_set(&mut self, set: Set) {
    self.sets.push(set);
  }

  pub fn is_valid(&self) -> bool {
    for set in self.sets.iter() {
      if !set.is_valid() { return false; }
    }
    return true;
  }

  pub fn get_power(&self) -> u32 {
    let mut min_red = 0;
    let mut min_blue = 0;
    let mut min_green = 0;

    for set in self.sets.iter() {
      if set.red > min_red { min_red = set.red; }
      if set.green> min_green{ min_green = set.green; }
      if set.blue > min_blue { min_blue = set.blue; }
    }

    return min_red * min_green * min_blue;
  }
} 

fn parse_line_to_game(line: &str) -> Game {
  let line_vec:Vec<char> = line.chars().collect();
  let mut ptr = 5;
  let mut game_id: String = String::new();

  while line_vec[ptr] != ':' {
    game_id.push(line_vec[ptr]);
    ptr += 1;
  }

  let mut game = Game::new(game_id.parse::<u32>().unwrap());

  ptr += 2;

  let mut red: u32 = 0;
  let mut green: u32 = 0;
  let mut blue: u32 = 0;

  while ptr < line_vec.len() {
    match line_vec[ptr] {
      ' ' | ',' => ptr += 1,
      ';' => {
        game.add_set(Set::new(red, green, blue));
        ptr += 2;

        red = 0;
        green = 0;
        blue = 0;
      }
      _ => {
        let mut count: String = String::new();
        let mut color: String = String::new();

        while line_vec[ptr] != ' ' {
          count.push(line_vec[ptr]);
          ptr += 1;
        }

        ptr += 1;

        while ptr < line_vec.len() && line_vec[ptr] != ',' && line_vec[ptr] != ';' {
          color.push(line_vec[ptr]);
          ptr += 1;
        }

        match color.as_str() {
          "red" => red = count.parse::<u32>().unwrap(),
          "green" => green = count.parse::<u32>().unwrap(),
          "blue" => blue = count.parse::<u32>().unwrap(),
          _ => println!("Color not recognised: {}", color)
        }
      }
    }
  }

  game.add_set(Set::new(red, green, blue));
  return game;
}

fn parse_contents(contents: &str) -> Vec<Game> {
  return contents.split('\n')
    .map(|line| parse_line_to_game(&line))
    .collect::<Vec<Game>>();
}

fn sum_valid_set_ids(games: &Vec<Game>) -> u32 {
  let mut valid_ids_sum = 0;

  for game in games.iter() {
    if game.is_valid() { valid_ids_sum += game.id; }
  }

  return valid_ids_sum;
}

fn sum_power_sets(games: &Vec<Game>) -> u32 {
  return games.iter().map(|game| game.get_power()).sum();
}

fn main() {
  let contents = fs::read_to_string("./data/q2.txt")
    .expect("Should have been able to read file");
  let games = parse_contents(&contents);

  println!("Part 1: {}", sum_valid_set_ids(&games));
  println!("Part 2: {}", sum_power_sets(&games));
}