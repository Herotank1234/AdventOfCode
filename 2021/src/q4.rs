use std::fs;

#[derive(Clone)]
struct BingoCard {
  nums: Vec<Vec<u32>>,
  marked: Vec<Vec<bool>>
}

impl BingoCard {
  fn new(nums: Vec<Vec<u32>>) -> Self {
    let marked: Vec<Vec<bool>> = vec![vec![false; nums[0].len()]; nums.len()];
    return Self { nums, marked }
  }

  fn mark(&mut self, value: &u32) {
    let mut found = false;

    for i in 0..self.nums.len() {
      for j in 0.. self.nums[0].len() {
        if self.nums[i][j] == *value {
          found = true;
          self.marked[i][j] = true;
          break;
        }
      }
      if found { break; }
    }
  }

  fn has_won(&mut self) -> bool {
    for row in self.marked.iter() {
      if row.iter().all(|mark| *mark) { return true; }
    }

    for j in 0..self.marked[0].len() {
      let mut sum = 0;
      for i in 0..self.marked.len() {
        sum += self.marked[i][j] as u32;
      }
      if sum == self.marked.len() as u32 { return true; }
    }

    return false;
  }

  fn get_score(&mut self, value: &u32) -> u32 {
    let mut rem_nums = 0;

    for i in 0..self.marked.len() {
      for j in 0..self.marked[0].len() {
        if !self.marked[i][j] { rem_nums += self.nums[i][j]; }
      }
    }

    return rem_nums * *value;
  }
}

fn find_first_winning_board_score(
  bingo_nums: &Vec<u32>, 
  bingo_cards: &mut Vec<BingoCard>
) -> u32 {
  let mut curr_cards = bingo_cards.clone();

  for num in bingo_nums {
    for bingo_card in curr_cards.iter_mut() {
      bingo_card.mark(&num);
      if bingo_card.has_won() { return bingo_card.get_score(&num); }
    }
  }

  return 0;
}

fn find_last_winning_board_score(
  bingo_nums: &Vec<u32>, 
  bingo_cards: &mut Vec<BingoCard>
) -> u32 {
  let mut curr_cards = bingo_cards.clone();
  let mut won_cards = vec![false; curr_cards.len()];

  for num in bingo_nums {
    for i in 0..curr_cards.len() {
      if won_cards[i] { continue; }

      let bingo_card = curr_cards.get_mut(i).unwrap();
      bingo_card.mark(&num);
      if bingo_card.has_won() { won_cards[i] = true; }

      if won_cards.iter().all(|won| *won) { return bingo_card.get_score(&num); }
    }
  }

  return 0;
}

fn main() {
  let contents = fs::read_to_string("./data/q4.txt")
    .expect("Should have been able to read file");

  let contents_vec = contents.split('\n').collect::<Vec<&str>>();

  let bingo_nums = contents_vec[0].split(',')
    .map(|str| str.parse::<u32>().unwrap())
    .collect::<Vec<u32>>();

  let mut bingo_cards: Vec<BingoCard> = Vec::new();

  let mut i = 2;
  while i < contents_vec.len() {
    let mut nums: Vec<Vec<u32>> = Vec::new();
    for _ in 0..5 {
      let row = contents_vec[i].split_whitespace()
        .map(|num| num.parse::<u32>().unwrap())
        .collect::<Vec<u32>>();
      nums.push(row);
      i += 1;
    }
    i += 1;
    bingo_cards.push(BingoCard::new(nums));
  }

  println!("Part 1: {}", find_first_winning_board_score(&bingo_nums, &mut bingo_cards));
  println!("Part 2: {}", find_last_winning_board_score(&bingo_nums, &mut bingo_cards));
}