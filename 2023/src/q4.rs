use std::fs;
use std::collections::HashMap;
use regex::Regex;

struct Card {
  winning_nums: Vec<u32>,
  nums: Vec<u32>
}

fn count_winning_nums(card: &Card) -> u32 {
  return card.nums.iter().map(|num| card.winning_nums.contains(num) as u32).sum();
}

fn sum_points(cards: &Vec<Card>) -> u32 {
  let mut points = 0;
  for card in cards {
    let winning_count = count_winning_nums(&card);
    if winning_count > 0 { points += u32::pow(2, winning_count - 1); }
  }
  return points;
}

fn sum_cards(cards: &Vec<Card>) -> u32 {
  let mut cards_freq = HashMap::new();
  for i in 0..cards.len() { cards_freq.insert(i, 1); } 

  for i in 0..cards.len() {
    let points = count_winning_nums(&cards[i]) as usize;
    for j in i + 1..i + 1 + points {
      *cards_freq.get_mut(&j).unwrap() += *cards_freq.get_mut(&i).unwrap();
    }
  }

  return cards_freq.values().sum();
}

fn main() {
  let contents = fs::read_to_string("./data/q4.txt")
    .expect("Should have been able to read file");
  let lines = contents.split('\n').collect::<Vec<&str>>();

  let mut cards = Vec::new();
  for line in lines {
    let colon_split = line.split(":").collect::<Vec<&str>>();

    let bar_split = colon_split[1].split("|").collect::<Vec<&str>>();
    let winning_part = bar_split[0];
    let nums_part = bar_split[1];

    let re = Regex::new(r"[0-9]+").unwrap();
    let winning_nums = re.find_iter(winning_part).map(|num| 
      num.as_str().parse::<u32>().unwrap()).collect::<Vec<u32>>();
    let nums = re.find_iter(nums_part).map(|num| num.as_str().parse::<u32>().unwrap())
      .collect::<Vec<u32>>();
    cards.push(Card{winning_nums, nums});
  }

  println!("Part 1: {}", sum_points(&cards));
  println!("Part 2: {}", sum_cards(&cards));
}