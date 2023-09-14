use std::fs;
use std::collections::HashMap;

struct Entry {
  signal_patterns: Vec<String>,
  output_values: Vec<String>
}

impl Entry {
  fn new(signal_patterns: Vec<String>, output_values: Vec<String>) -> Self {
    return Self { signal_patterns, output_values };
  }
}

fn count_easy_digits(entries: &Vec<Entry>) -> u32 {
  let mut easy_digits = 0;
  for entry in entries {
    for output_val in entry.output_values.iter() {
      match output_val.len() {
        2 | 3 | 4 | 7 => easy_digits += 1,
        _ => ()
      }
    }
  }
  return easy_digits;
}

fn contains_letters(signal: &String, letters: &String) -> bool {
  return letters.chars()
    .map(|letter| signal.contains(letter))
    .all(|contains| contains);
}

fn find_mapping(signal_patterns: &Vec<String>) -> HashMap<String, u32> {
  let mut not_found: Vec<String> = signal_patterns.clone().iter()
    .map(|str| {
      let mut new_str = str.chars().collect::<Vec<char>>();
      new_str.sort();
      let sorted: String = new_str.iter().collect();
      return sorted;
    })
    .collect();

  let mut mapping: HashMap<u32, String> = HashMap::new();

  /* Easy values 1, 4, 7, 8 */
  not_found.retain(|signal| {
    match signal.len() {
      2 => mapping.insert(1, signal.to_owned()),
      3 => mapping.insert(7, signal.to_owned()),
      4 => mapping.insert(4, signal.to_owned()),
      7 => mapping.insert(8, signal.to_owned()),
      _ => None
    };
    return !vec![2, 3, 4, 7].contains(&signal.len());
  });

  /* 
    Values left: 0, 2, 3, 5, 6, 9
    0, 6, 9 have length 6
    2, 3, 5 have length 5
  */

  /* Only 4 is contained in 9 */
  let four: String = mapping.get(&4).unwrap().clone();
  not_found.retain(|signal| {
    let contains = contains_letters(signal, &four);
    if contains {
      mapping.insert(9, signal.to_owned());
    }
    return !contains;
  });

  /* 7 is contained in 0, 3, differentiate by length */
  let seven: String = mapping.get(&7).unwrap().clone();
  let mut contains_seven: Vec<String> = Vec::new();

  not_found.retain(|signal| {
    let contains = contains_letters(signal, &seven);
    if contains {
      contains_seven.push(signal.to_owned());
    }
    return !contains;
  });

  if contains_seven[0].len() == 5 {
    mapping.insert(3, contains_seven[0].to_owned());
    mapping.insert(0, contains_seven[1].to_owned());
  } else {
    mapping.insert(3, contains_seven[1].to_owned());
    mapping.insert(0, contains_seven[0].to_owned());
  }

  /* 6 is the only one remaining with length 6 */
  not_found.retain(|signal| {
    if signal.len() == 6 {
      mapping.insert(6, signal.to_owned());
    }
    return signal.len() != 6;
  });

  /* 
    2 and 5 are then differentiated by using 9, only 5 should be contained in 9 
  */
  let nine: String = mapping.get(&9).unwrap().clone();
  not_found.retain(|signal| {
    let contains = contains_letters(&nine, signal);
    if contains {
      mapping.insert(5, signal.to_owned());
    }
    return !contains;
  });

  mapping.insert(2, not_found[0].to_owned());
  return mapping.iter()
    .map(|(k, v)| (v.to_owned(), k.to_owned()))
    .collect::<HashMap<String, u32>>();
}

fn decode(mapping: &HashMap<String, u32>, output_values: &Vec<String>) -> u32 {
  let curr_output: Vec<String> = output_values.clone().iter()
    .map(|str| {
      let mut new_str = str.chars().collect::<Vec<char>>();
      new_str.sort();
      let sorted: String = new_str.iter().collect();
      return sorted;
    })
    .collect();

  let output: String = curr_output.iter()
    .map(|str| (*mapping.get(str).unwrap() as u8 + '0' as u8) as char)
    .collect();
  return output.parse::<u32>().unwrap();
}

fn decode_and_sum(entries: &Vec<Entry>) -> u32 {
  return entries.iter()
    .map(|entry| {
      let mapping = find_mapping(&entry.signal_patterns);
      let value = decode(&mapping, &entry.output_values);
      return value;
    })
    .sum();
}

fn main() {
  let contents = fs::read_to_string("./data/q8.txt")
    .expect("Should have been able to read file");

  let mut entries: Vec<Entry> = Vec::new();

  for line in contents.split('\n') {
    let parts = line.split(" | ").collect::<Vec<&str>>();
    let signal_patterns = parts[0].split(' ')
      .map(|str| str.to_string())
      .collect::<Vec<String>>();
    let output_values = parts[1].split(' ')
      .map(|str| str.to_string())
      .collect::<Vec<String>>();

    entries.push(Entry::new(signal_patterns, output_values));
  }

  println!("Part 1: {}", count_easy_digits(&entries));
  println!("Part 2: {}", decode_and_sum(&entries));
}