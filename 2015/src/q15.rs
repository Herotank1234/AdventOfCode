use std::fs;
use regex::Regex;

#[derive(Debug)]
struct Ingredient {
  capacity: i32,
  durability: i32,
  flavor: i32,
  texture: i32,
  calories: i32
}

impl Ingredient {
  fn get_capacity_score(&self, amount: u32) -> i32 {
    return self.capacity * amount as i32;
  }

  fn get_durability_score(&self, amount: u32) -> i32 {
    return self.durability * amount as i32;
  }

  fn get_flavor_score(&self, amount: u32) -> i32 {
    return self.flavor * amount as i32;
  }

  fn get_texture_score(&self, amount: u32) -> i32 {
    return self.texture * amount as i32;
  }

  fn get_calories(&self, amount: u32) -> i32 {
    return self.calories * amount as i32;
  }
}

fn get_combinations(total: u32, parts: u32) -> Vec<Vec<u32>> {
  if parts == 1 { return vec![vec![total]] }

  let mut combinations: Vec<Vec<u32>> = Vec::new();

  for i in 1..(total + 1) - (parts - 1) {
    let curr_comb = vec![i];
    let mut next_combs = get_combinations(total - i, parts - 1);
    for next_comb in next_combs.iter_mut() {
      let mut total_combination = curr_comb.clone();
      total_combination.append(next_comb);
      combinations.push(total_combination);
    }
  }

  return combinations;
}

fn get_highest_score(ingredients: &Vec<Ingredient>) -> u32 {
  let combinations = get_combinations(100, ingredients.len() as u32);

  let mut total_score: u32 = 0;
  for combination in combinations.iter() {
    let mut capacity_score: i32 = 0;
    let mut durability_score: i32 = 0;
    let mut flavor_score: i32 = 0;
    let mut texture_score: i32 = 0;

    for i in 0..combination.len() {
      capacity_score += ingredients[i].get_capacity_score(combination[i]);
      durability_score += ingredients[i].get_durability_score(combination[i]);
      flavor_score += ingredients[i].get_flavor_score(combination[i]);
      texture_score += ingredients[i].get_texture_score(combination[i]);
    }

    if capacity_score <= 0 || durability_score <= 0 || 
      flavor_score <= 0 || texture_score <= 0 { continue }
    
    let curr_score: u32 = capacity_score as u32 * durability_score as u32 * 
      flavor_score as u32 * texture_score as u32;

    if total_score < curr_score { total_score = curr_score }
  }

  return total_score;
}

fn get_highest_score_with_calorie_check(ingredients: &Vec<Ingredient>) -> u32 {
  let combinations = get_combinations(100, ingredients.len() as u32);

  let mut total_score: u32 = 0;
  for combination in combinations.iter() {
    let mut capacity_score: i32 = 0;
    let mut durability_score: i32 = 0;
    let mut flavor_score: i32 = 0;
    let mut texture_score: i32 = 0;
    let mut calories: i32 = 0;

    for i in 0..combination.len() {
      capacity_score += ingredients[i].get_capacity_score(combination[i]);
      durability_score += ingredients[i].get_durability_score(combination[i]);
      flavor_score += ingredients[i].get_flavor_score(combination[i]);
      texture_score += ingredients[i].get_texture_score(combination[i]);
      calories += ingredients[i].get_calories(combination[i]);
    }

    if capacity_score <= 0 || durability_score <= 0 || 
      flavor_score <= 0 || texture_score <= 0 { continue }

    if calories != 500 { continue }
    
    let curr_score: u32 = capacity_score as u32 * durability_score as u32 * 
      flavor_score as u32 * texture_score as u32;

    if total_score < curr_score { total_score = curr_score }
  }

  return total_score;
}

fn main() {
  let contents = fs::read_to_string("./data/q15.txt")
    .expect("Should have been able to read file");

  let re = Regex::new("[A-Za-z]+: \
    capacity (-?[\\d]+), durability (-?[\\d]+), flavor (-?[\\d]+), \
    texture (-?[\\d]+), calories (-?[\\d]+)").unwrap();
  let mut ingredients: Vec<Ingredient> = Vec::new();

  contents.split("\n").for_each(|line| {
    let captures = re.captures(line);

    match captures {
      Some(matched_groups) => {
        let ingredient = Ingredient {
          capacity: matched_groups[1].parse::<i32>().unwrap(),
          durability: matched_groups[2].parse::<i32>().unwrap(),
          flavor: matched_groups[3].parse::<i32>().unwrap(),
          texture: matched_groups[4].parse::<i32>().unwrap(),
          calories: matched_groups[5].parse::<i32>().unwrap(),
        };

        ingredients.push(ingredient);
      },

      None => println!("Was unable to match line: {}", line)
    }
  });

  println!("Part 1: {}", get_highest_score(&ingredients));
  println!("Part 2: {}", get_highest_score_with_calorie_check(&ingredients));
}