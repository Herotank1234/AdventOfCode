use std::fs;

#[derive(Debug)]
struct Equipment {
  name: String,
  cost: u32,
  dmg: u32,
  amr: u32
}

#[derive(Debug)]
struct Shop {
  weapons: Vec<Equipment>,
  armour: Vec<Equipment>,
  rings: Vec<Equipment>
}

#[derive(Debug)]
struct Character {
  hp: i32,
  dmg: i32,
  amr: i32
}

fn fight(player: &Character, boss: &Character) -> bool {
  let mut curr_player_hp = player.hp;
  let mut curr_boss_hp = boss.hp;

  let mut player_dmg_dealt = player.dmg - boss.amr;
  if player_dmg_dealt < 1 { player_dmg_dealt = 1 }

  let mut boss_dmg_dealt = boss.dmg - player.amr;
  if boss_dmg_dealt < 1 { boss_dmg_dealt = 1 }

  while curr_player_hp > 0 && curr_boss_hp > 0 {
    curr_boss_hp -= player_dmg_dealt;
    if curr_boss_hp <= 0 { break }
    curr_player_hp -= boss_dmg_dealt;
  }

  return curr_boss_hp <= 0;
}

fn calculate_min_gold_needed_to_win(shop: &Shop, boss: &Character) -> u32 {
  let mut min_cost = u32::MAX;
  
  for weapon in shop.weapons.iter() {
    for armour in shop.armour.iter() {
      for left_ring in shop.rings.iter() {
        for right_ring in shop.rings.iter() {
          if left_ring.name != "None" && left_ring.name == right_ring.name { continue }

          let cost = weapon.cost + armour.cost + left_ring.cost + right_ring.cost;
          let dmg = weapon.dmg + armour.dmg + left_ring.dmg + right_ring.dmg;
          let amr = weapon.amr + armour.amr + left_ring.amr + right_ring.amr;

          let won = fight(&Character{hp: 100, dmg: dmg as i32, amr: amr as i32}, boss);
          if won {
            if cost < min_cost { min_cost = cost }
          }
        }
      }
    }
  }

  return min_cost;
}

fn calculate_max_gold_needed_to_lose(shop: &Shop, boss: &Character) -> u32 {
  let mut max_cost = u32::MIN;
  
  for weapon in shop.weapons.iter() {
    for armour in shop.armour.iter() {
      for left_ring in shop.rings.iter() {
        for right_ring in shop.rings.iter() {
          if left_ring.name != "None" && left_ring.name == right_ring.name { continue }

          let cost = weapon.cost + armour.cost + left_ring.cost + right_ring.cost;
          let dmg = weapon.dmg + armour.dmg + left_ring.dmg + right_ring.dmg;
          let amr = weapon.amr + armour.amr + left_ring.amr + right_ring.amr;

          let won = fight(&Character{hp: 100, dmg: dmg as i32, amr: amr as i32}, boss);
          if !won {
            if cost > max_cost { max_cost = cost }
          }
        }
      }
    }
  }

  return max_cost;
}

fn main() {
  let contents = fs::read_to_string("./data/q21.txt")
    .expect("Should have been able to read file");

  let lines = contents.split("\n").collect::<Vec<&str>>();
  let boss_hp = lines[0].split(": ").collect::<Vec<&str>>()[1].parse::<i32>().unwrap();
  let boss_dmg = lines[1].split(": ").collect::<Vec<&str>>()[1].parse::<i32>().unwrap();
  let boss_amr = lines[2].split(": ").collect::<Vec<&str>>()[1].parse::<i32>().unwrap();
  let boss = Character {hp: boss_hp, dmg: boss_dmg, amr: boss_amr};

  let weapons: Vec<Equipment> = vec![
    Equipment {name: "Dagger".to_string(), cost: 8, dmg: 4, amr: 0},
    Equipment {name: "Shortsword".to_string(), cost: 10, dmg: 5, amr: 0},
    Equipment {name: "Warhammer".to_string(), cost: 25, dmg: 6, amr: 0},
    Equipment {name: "Longsword".to_string(), cost: 40, dmg: 7, amr: 0},
    Equipment {name: "Greataxe".to_string(), cost: 74, dmg: 8, amr: 0}
  ];

  let armour: Vec<Equipment> = vec![
    Equipment {name: "None".to_string(), cost: 0, dmg: 0, amr: 0},
    Equipment {name: "Leather".to_string(), cost: 13, dmg: 0, amr: 1},
    Equipment {name: "Chainmail".to_string(), cost: 31, dmg: 0, amr: 2},
    Equipment {name: "Splintmail".to_string(), cost: 53, dmg: 0, amr: 3},
    Equipment {name: "Bandedmail".to_string(), cost: 75, dmg: 0, amr: 4},
    Equipment {name: "Platemail".to_string(), cost: 102, dmg: 0, amr: 5}
  ];

  let rings: Vec<Equipment> = vec![
    Equipment {name: "None".to_string(), cost: 0, dmg: 0, amr: 0},
    Equipment {name: "Damage +1".to_string(), cost: 25, dmg: 1, amr: 0},
    Equipment {name: "Damage +2".to_string(), cost: 50, dmg: 2, amr: 0},
    Equipment {name: "Damage +3".to_string(), cost: 100, dmg: 3, amr: 0},
    Equipment {name: "Defence +1".to_string(), cost: 20, dmg: 0, amr: 1},
    Equipment {name: "Defence +2".to_string(), cost: 40, dmg: 0, amr: 2},
    Equipment {name: "Defence +3".to_string(), cost: 80, dmg: 0, amr: 3}
  ];

  let shop = Shop{weapons: weapons, armour: armour, rings: rings};

  println!("Part 1: {}", calculate_min_gold_needed_to_win(&shop, &boss));
  println!("Part 2: {}", calculate_max_gold_needed_to_lose(&shop, &boss));
}