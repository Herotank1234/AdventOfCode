use std::{fs, collections::HashMap};

const MIN_MANA_NEEDED: i32 = 53;

#[derive(Debug, Clone)]
struct Boss {
  hp: i32,
  dmg: i32
}

#[derive(Debug, Clone)]
struct Player {
  hp: i32,
  mana: i32,
  amr: i32
}

fn simulate_battle(
  player: &Player,
  boss: &Boss,
  active_effects: &HashMap<String, u32>,
  mana_spent: u32,
  is_player_turn: bool,
  hard_mode: bool
) -> (bool, u32) {
  let mut next_player = player.clone();
  let mut next_boss = boss.clone();
  let mut next_active_effects: HashMap<String, u32> = HashMap::new();
  let effects: Vec<&str> = vec!["magic missile", "drain", "shield", "poison", "recharge"];
  let costs: HashMap<&str, i32> = HashMap::from_iter([
    ("magic missile", 53),
    ("drain", 73),
    ("shield", 113),
    ("poison", 173),
    ("recharge", 229)
  ]);

  if is_player_turn && hard_mode { next_player.hp -= 1 }

  for (effect, time) in active_effects {
    let next_time = time - 1;
    match effect.as_str() {
      "shield" => {
        if next_time == 0 { next_player.amr = 0 }
      },
      "poison" => {
        next_boss.hp -= 3;
      },
      "recharge" => {
        next_player.mana += 101;
      },
      _ => println!("Was unable to match effect: {}", effect)
    }

    if next_time != 0 { next_active_effects.insert(effect.clone(), next_time); }
  }

  if next_boss.hp <= 0 { return (true, mana_spent) }

  if next_player.hp <= 0 { return (false, mana_spent) }

  if next_player.mana < MIN_MANA_NEEDED { return (false, mana_spent) }

  if is_player_turn {
    let mut min_manas: Vec<u32> = Vec::new();

    for effect in effects.iter() {
      if next_active_effects.contains_key(&effect.to_string()) { continue }

      let mut curr_player = next_player.clone();
      let mut curr_boss = next_boss.clone();
      let mut curr_active_effects = next_active_effects.clone();

      let cost = costs.get(effect).unwrap();
      if curr_player.mana < *cost { continue }
      curr_player.mana -= cost;
      let next_mana_spent = mana_spent + (*cost as u32);

      match *effect {
        "magic missile" => curr_boss.hp -= 4, 
        "drain" => {
          curr_boss.hp -= 2;
          curr_player.hp += 2;
        }, 
        "shield" => {
          curr_player.amr = 7;
          curr_active_effects.insert(effect.to_string(), 6);
        }, 
        "poison" => {
          curr_active_effects.insert(effect.to_string(), 6);
        }
        "recharge" => {
          curr_active_effects.insert(effect.to_string(), 5);
        }
        _ => println!("Was unable to match effect {}", effect),
      }

      let (success, final_mana_spent)
        = simulate_battle(&curr_player, &curr_boss, &curr_active_effects, 
            next_mana_spent, !is_player_turn, hard_mode);
      if success { min_manas.push(final_mana_spent) }
    }

    if !min_manas.is_empty() { return (true, *min_manas.iter().min().unwrap()) }
  } else {
    let mut boss_dmg = next_boss.dmg - next_player.amr;
    if boss_dmg < 1 { boss_dmg = 1 }

    next_player.hp -= boss_dmg;

    let (success, final_mana_spent) 
      = simulate_battle(&next_player, &next_boss, &next_active_effects, 
          mana_spent, !is_player_turn, hard_mode);
    
    if success { return (success, final_mana_spent) }
  }

  return (false, mana_spent);
}

fn get_min_mana_to_win(player: &Player, boss: &Boss) -> u32 {
  let (_, min_mana) 
    = simulate_battle(player, boss, &HashMap::new(), 
      0, true, false);
  return min_mana;
}

fn get_min_mana_to_win_hard(player: &Player, boss: &Boss) -> u32 {
  let (_, min_mana) 
    = simulate_battle(player, boss, &HashMap::new(), 
      0, true, true);
  return min_mana;
}


fn main() {
  let contents = fs::read_to_string("./data/q22.txt")
    .expect("Should have been able to read file");
  let lines = contents.split("\n").collect::<Vec<&str>>();
  let boss_hp = lines[0].split(": ").collect::<Vec<&str>>()[1].parse::<i32>().unwrap();
  let boss_dmg= lines[1].split(": ").collect::<Vec<&str>>()[1].parse::<i32>().unwrap();
  let boss = Boss{hp: boss_hp, dmg: boss_dmg};
  let player = Player{hp: 50, mana: 500, amr: 0};

  println!("Part 1: {}", get_min_mana_to_win(&player, &boss));
  println!("Part 2: {}", get_min_mana_to_win_hard(&player, &boss));
}