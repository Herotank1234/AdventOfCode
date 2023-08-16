use std::{fs, collections::HashMap};
use regex::Regex;

#[derive(Debug)]
struct Sue {
  id: u32,
  attributes: Vec<(String, u32)>,
}

fn find_my_sue_id(sues: &Vec<Sue>, my_sue: &HashMap<String, u32>) -> u32 {
  for sue in sues.iter() {
    let mut found = true;
    for (key, value) in sue.attributes.iter() {
      let my_sue_value = my_sue.get(key).unwrap();
      if my_sue_value != value {
        found = false;
        break;
      }
    }
    if found { return sue.id }
  }
  return 0;
}

fn find_my_sue_real_id(sues: &Vec<Sue>, my_sue: &HashMap<String, u32>) -> u32 {
  for sue in sues.iter() {
    let mut found = true;
    for (key, value) in sue.attributes.iter() {
      let my_sue_value = my_sue.get(key).unwrap();
      match key.as_str() {
        "cats" | "trees" => {
          if value < my_sue_value {
            found = false;
            break;
          }
        },

        "pomeranians" | "goldfish" => {
          if value > my_sue_value {
            found = false;
            break;
          }
        },

        _ => {
          if value != my_sue_value {
            found = false;
            break;
          }
        }
      }
    }
    if found { return sue.id }
  }
  return 0;
}

fn main() {
  let contents = fs::read_to_string("./data/q16.txt")
    .expect("Should have been able to read file");

  let my_sue: HashMap<String, u32> = HashMap::from_iter([
    ("children".to_string(), 3),
    ("cats".to_string(), 7),
    ("samoyeds".to_string(), 2),
    ("pomeranians".to_string(), 3),
    ("akitas".to_string(), 0),
    ("vizslas".to_string(), 0),
    ("goldfish".to_string(), 5),
    ("trees".to_string(), 3),
    ("cars".to_string(), 2),
    ("perfumes".to_string(), 1),
  ]);
  let mut sues: Vec<Sue> = Vec::new();
  let re = Regex::new("Sue ([\\d]+): ([a-z]+): \
    ([\\d]+), ([a-z]+): ([\\d]+), ([a-z]+): ([\\d]+)").unwrap();

  contents.split("\n").for_each(|line| {
    let captures = re.captures(line);
    match captures {
      Some(matched_groups) => {
        let id: u32 = matched_groups[1].parse::<u32>().unwrap();
        let mut attributes: Vec<(String, u32)> = Vec::new();

        for i in (2..matched_groups.len()).step_by(2) {
          let key = matched_groups[i].to_string();
          let value = matched_groups[i + 1].parse::<u32>().unwrap();
          attributes.push((key, value));
        }

        sues.push(Sue { id: id, attributes: attributes });
      }
      None => println!("Was unable to match line: {}", line),
    }
  });

  println!("Part 1: {}", find_my_sue_id(&sues, &my_sue));
  println!("Part 2: {}", find_my_sue_real_id(&sues, &my_sue));
}