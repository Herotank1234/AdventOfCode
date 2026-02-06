use std::collections::HashMap;
use std::fs;
use phf::phf_map;

#[derive(Clone, Copy, PartialEq)]
enum HandType {
  HighCard = 0,
  OnePair = 1,
  TwoPair = 2,
  Three = 3,
  FullHouse = 4,
  Four = 5,
  Five = 6
}

#[derive(Clone, Copy, Eq, Hash, Ord, PartialOrd, PartialEq,)]
enum Card {
  Joker = 0,
  Two = 1,
  Three = 2,
  Four = 3,
  Five = 4,
  Six = 5,
  Seven = 6,
  Eight = 7,
  Nine = 8,
  Ten = 9,
  Jack = 10,
  Queen = 11,
  King = 12,
  Ace = 13,
}

static CHAR_TO_CARD: phf::Map<char, Card> = phf_map! {
  '2' => Card::Two,
  '3' => Card::Three,
  '4' => Card::Four,
  '5' => Card::Five,
  '6' => Card::Six,
  '7' => Card::Seven,
  '8' => Card::Eight,
  '9' => Card::Nine,
  'T' => Card::Ten,
  'J' => Card::Jack,
  'Q' => Card::Queen,
  'K' => Card::King,
  'A' => Card::Ace,
};

static FREQ_TO_TYPE: phf::Map<&'static str, HandType> =  phf_map! { 
  "5" => HandType::Five,
  "41" => HandType::Four,
  "32" => HandType::FullHouse,
  "311" => HandType::Three,
  "221" => HandType::TwoPair,
  "2111" => HandType::OnePair,
  "11111" => HandType::HighCard
};

#[derive(Clone)]
struct Hand {
  cards: Vec<Card>,
  hand_type: HandType,
  bet: u64
}

impl Hand {
  fn new(cards: &str, bet: u64) -> Self {
    let mut card_freqs = HashMap::new();
    let mut cards_translated = Vec::new();
    for c in cards.chars() {
      cards_translated.push(*(CHAR_TO_CARD.get(&c).unwrap()));
      if card_freqs.contains_key(&c) {
        *card_freqs.get_mut(&c).unwrap() += 1;
      } else {
        card_freqs.insert(c, 1);
      }
    }

    let mut freqs_vec = card_freqs.values().collect::<Vec<&u64>>();
    freqs_vec.sort_by(|a, b| b.cmp(a));
    let freqs_str = freqs_vec.iter().fold("".to_owned(), |acc, c| acc + &c.to_string());
    let hand_type = FREQ_TO_TYPE.get(&freqs_str).unwrap();

    return Self{cards: cards_translated, hand_type: *hand_type, bet};
  }
}

fn calculate_total_winnings(hands: &Vec<Hand>) -> u64 {
  let mut curr_hands = hands.clone();
  curr_hands.sort_by(|a, b| {
    if a.hand_type == b.hand_type {
      return a.cards.cmp(&b.cards);
    } else {
      return (a.hand_type as u8).cmp(&(b.hand_type as u8));
    }
  });
  
  let mut winnings = 0;
  let mut rank = 1;
  for hand in curr_hands {
    winnings += hand.bet * rank;
    rank += 1;
  }
  return winnings;
}

fn update_joker_hands(hands: &mut Vec<Hand>) {
  for hand in hands.iter_mut() {
    if hand.cards.contains(&Card::Jack) {
      let mut joker_count = 0;
      let mut card_freqs = HashMap::new();

      for card in hand.cards.iter_mut() {
        if *card == Card::Jack {
          *card = Card::Joker;
          joker_count += 1;
        } else {
          if card_freqs.contains_key(&card) {
            *card_freqs.get_mut(&card).unwrap() += 1;
          } else {
            card_freqs.insert(card, 1);
          }
        }
      }

      if joker_count == 5 { continue; }

      let mut card_with_highest_freq = Card::Ace;
      let mut highest_freq = 0;
      for (k, v) in card_freqs.iter() {
        if *v > highest_freq {
          card_with_highest_freq = **k;
          highest_freq = *v;
        }
      }

      *card_freqs.get_mut(&card_with_highest_freq).unwrap() += joker_count;

      let mut freqs_vec = card_freqs.values().collect::<Vec<&u64>>();
      freqs_vec.sort_by(|a, b| b.cmp(a));
      let freqs_str = freqs_vec.iter().fold("".to_owned(), |acc, c| acc + &c.to_string());
      let hand_type = FREQ_TO_TYPE.get(&freqs_str).unwrap();
      hand.hand_type = *hand_type;
    }
  }
}

fn calculate_total_winnings_with_joker(hands: &Vec<Hand>) -> u64 {
  let mut curr_hands = hands.clone();
  update_joker_hands(&mut curr_hands);
  return calculate_total_winnings(&curr_hands);
}

fn main() {
  let contents = fs::read_to_string("./data/q7.txt")
    .expect("Should have been able to read file");
  let hands = contents.split('\n').map(|line| {
    let parts = line.split(' ').collect::<Vec<&str>>();
    return Hand::new(parts[0], parts[1].parse::<u64>().unwrap());
  }).collect::<Vec<Hand>>();

  println!("Part 1: {}", calculate_total_winnings(&hands));
  println!("Part 2: {}", calculate_total_winnings_with_joker(&hands));
}