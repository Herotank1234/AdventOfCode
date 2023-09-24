use std::collections::HashMap;
use std::cmp::max;
use std::fs;
use regex::Regex;

#[derive(Clone)]
struct Player {
  pos: u32,
  points: u32
}

impl Player {
  fn new(pos: u32) -> Self {
    return Self { pos, points: 0 };
  }

  fn move_spaces(&mut self, spaces: u32) {
    self.pos = ((self.pos + spaces - 1) % 10) + 1;
    self.points += self.pos;
  }

  fn has_won(&self) -> bool {
    return self.points >= 1000;
  }

  fn get_pos(&self) -> u32 {
    return self.pos;
  }

  fn get_points(&self) -> u32 {
    return self.points;
  }
}

struct Dice {
  val: u32,
  max_val: u32,
  rolled: u32
}

impl Dice {
  fn new() -> Self {
    return Self { val: 1, max_val: 100, rolled: 0 };
  }

  fn next(&mut self) -> u32 {
    let curr_val = self.val;
    self.val += 1;
    if self.val > self.max_val { self.val = 1; }
    self.rolled += 1;
    return curr_val;
  }

  fn get_rolled(&self) -> u32 {
    return self.rolled;
  }
}

fn get_final_score(pos1: u32, pos2: u32) -> u32 {
  let mut player1 = Player::new(pos1);
  let mut player2 = Player::new(pos2);
  let mut is_player1 = true;

  let mut dice = Dice::new();

  while !(player1.has_won() || player2.has_won()) {
    let curr_player = if is_player1 { &mut player1 } else { &mut player2 };
    is_player1 = !is_player1;

    let spaces_to_move = dice.next() + dice.next() + dice.next();
    (*curr_player).move_spaces(spaces_to_move);
  }

  let losing_player = if player1.has_won() { &player2 } else { &player1 };
  return losing_player.get_points() * dice.get_rolled();
}

fn calculate_wins(player1: &Player, player2: &Player, is_player1: bool, 
  memo: &mut HashMap<String, (u64, u64)>, roll_freq: &HashMap<u32, u64>
) -> (u64, u64) {
  let mut p1_points = player1.get_points();
  if p1_points > 21 { p1_points = 21; }
  let mut p2_points = player2.get_points();
  if p2_points > 21 { p2_points = 21; }

  let key = format!("{}-{}-{}-{}-{}", player1.get_pos(), p1_points, 
    player2.get_pos(), p2_points, is_player1);

  if memo.contains_key(&key) { return *memo.get(&key).unwrap()}

  if p1_points == 21 {
    let result = (1, 0);
    memo.insert(key, result);
    return result;
  }

  if p2_points == 21 {
    let result = (0, 1);
    memo.insert(key, result);
    return result;
  }

  let next_is_player1 = !is_player1;
  let mut wins = (0, 0);

  if is_player1 {
    for i in 3..10 {
      let freq = roll_freq.get(&i).unwrap();
      let mut next_player1 = player1.clone();
      next_player1.move_spaces(i);
      let next_wins = calculate_wins(&next_player1, player2, next_is_player1, 
        memo, roll_freq);
      wins = (wins.0 + (next_wins.0 * freq), wins.1 + (next_wins.1 * freq));
    }
  } else {
    for i in 3..10 {
      let freq = roll_freq.get(&i).unwrap();
      let mut next_player2 = player2.clone();
      next_player2.move_spaces(i);
      let next_wins = calculate_wins(player1, &next_player2, next_is_player1, 
        memo, roll_freq);
      wins = (wins.0 + (next_wins.0 * freq), wins.1 + (next_wins.1 * freq));
    }
  }
  memo.insert(key, wins);
  return wins;
}

/*
  Dice Roll Total Frequencies
  3: 1 1 1
  4: 1 1 2, 1 2 1, 2 1 1
  5: 1 1 3, 1 2 2, 1 3 1, 2 1 2, 2 2 1, 3 1 1
  6: 1 2 3, 1 3 2, 2 1 3, 2 2 2, 2 3 1, 3 1 2, 3 2 1
  7: 1 3 3, 2 2 3, 2 3 2, 3 1 3, 3 2 2, 3 3 1
  8: 2 3 3, 3 2 3, 3 3 2
  9: 3 3 3
*/

fn get_win_difference(pos1: u32, pos2: u32) -> u64 {
  let mut memo: HashMap<String, (u64, u64)> = HashMap::new();
  let roll_frequencies: HashMap<u32, u64> = HashMap::from_iter(vec![
    (3, 1), (4, 3), (5, 6), (6, 7), (7, 6), (8, 3), (9, 1)
  ]);
  let (p1_win, p2_win) = calculate_wins(&Player::new(pos1), &Player::new(pos2), 
    true, &mut memo, &roll_frequencies);
  return max(p1_win, p2_win);
}

fn main() {
  let contents = fs::read_to_string("./data/q21.txt")
    .expect("Should have been able to read file");

  let lines: Vec<&str> = contents.split('\n').collect();

  let re = Regex::new("Player [12] starting position: ([\\d]+)\\s?").unwrap();
  let mut captures = re.captures(lines[0]);
  let mut pos1: u32 = 0;
  let mut pos2: u32 = 0;
  match captures {
    Some(groups) => pos1 = groups[1].parse::<u32>().unwrap(),
    None => println!("Unable to match line: {}", lines[0]) 
  }

  captures = re.captures(lines[1]);
  match captures {
    Some(groups) => pos2 = groups[1].parse::<u32>().unwrap(),
    None => println!("Unable to match line: {}", lines[1])
  }

  println!("Part 1: {}", get_final_score(pos1, pos2));
  println!("Part 2: {}", get_win_difference(pos1, pos2));
}