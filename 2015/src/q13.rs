use std::fs;
use std::collections::HashMap;
use std::collections::HashSet;
use regex::Regex;
use itertools::Itertools;

fn calculate_max_happiness(
  attendees: &HashSet<String>, 
  potential_happiness: &HashMap<String, HashMap<String, i32>>
) -> i32 
{
  let mut max_happiness: i32 = i32::MIN;

  for perm in attendees.iter().permutations(attendees.len()) {
    let mut curr_happiness: i32 = 0;
    for i in 0..perm.len() {
      let person = perm[i];
      let left = perm[if i == 0 { perm.len() - 1 } else { i - 1 }];
      let right = perm[(i + 1) % perm.len()];
      curr_happiness += potential_happiness.get(person).unwrap().get(left).unwrap();
      curr_happiness += potential_happiness.get(person).unwrap().get(right).unwrap();
    }

    if curr_happiness > max_happiness { max_happiness = curr_happiness }
  }

  return max_happiness;
}


fn calculate_max_happiness_with_me(
  attendees: &HashSet<String>, 
  potential_happiness: &HashMap<String, HashMap<String, i32>>
) -> i32 
{
  let mut attendees_with_me = attendees.clone();
  let mut potential_happiness_with_me = potential_happiness.clone();

  for (_, happiness) in potential_happiness_with_me.iter_mut() {
    happiness.insert("Me".to_string(), 0);
  }

  let me_values = attendees_with_me
    .iter()
    .map(|attendee| (attendee.clone(), 0 as i32))
    .clone()
    .collect::<HashMap<String, i32>>();
  potential_happiness_with_me.insert("Me".to_string(), me_values);
  attendees_with_me.insert("Me".to_string());
  
  return calculate_max_happiness(&attendees_with_me, &potential_happiness_with_me);
}

fn main() {
  let contents = fs::read_to_string("./data/q13.txt")
    .expect("Should have been able to read file");

  let mut potential_happiness: HashMap<String, HashMap<String, i32>> = HashMap::new();
  let mut attendees: HashSet<String> = HashSet::new();
  let re = Regex::new("([A-Za-z]+) would (gain|lose) ([\\d]+) \
    happiness units by sitting next to ([A-Za-z]+).").unwrap();

  for happiness in contents.split("\n") {
    let cap = re.captures(happiness);
    match cap {
      Some(matched_groups) => {
        let key =  matched_groups[1].to_string();
        if !potential_happiness.contains_key(&key) {
          potential_happiness.insert(key.clone(), HashMap::new());
        }

        let change_direction: i32 = if &matched_groups[2] == "gain" { 1 } else { -1 };
        let change: i32 = matched_groups[3].parse::<i32>().unwrap();
        potential_happiness
          .get_mut(&key)
          .unwrap()
          .insert(matched_groups[4].to_string(), change * change_direction);
        attendees.insert(key);
      },

      None => println!("Was unable to match line: {}", happiness),
    }
  }

  println!("Part 1: {}", calculate_max_happiness(&attendees, &potential_happiness));
  println!("Part 2: {}", calculate_max_happiness_with_me(&attendees, &potential_happiness));
}