use std::{fs, collections::HashMap};

fn count_bits_at_index(nums: &Vec<&str>, index: usize) -> HashMap<char, u32> {
  let mut counts: HashMap<char, u32> = HashMap::from_iter(
    vec![('0', 0), ('1', 0)]
  );

  for num in nums {
    *(counts.get_mut(&(num.chars().nth(index).unwrap())).unwrap()) += 1;
  }

  return counts;
}

fn count_bits(nums: &Vec<&str>) -> HashMap<u32, HashMap<char, u32>> {
  let mut bit_counts: HashMap<u32, HashMap<char, u32>> = HashMap::new();
  for i in 0..nums[0].len() {
    let counts = count_bits_at_index(nums, i);
    bit_counts.insert(i as u32, counts);
  }
  return bit_counts;
}

fn get_power_consumption(nums: &Vec<&str>) -> u32 {
  let bit_counts: HashMap<u32, HashMap<char, u32>> = count_bits(nums);

  let mut gamma: String = String::new();
  let mut epsilon: String = String::new();

  for index in 0..nums[0].len() {
    let counts = bit_counts.get(&(index as u32)).unwrap();
    if counts[&'0'] > counts[&'1'] {
      gamma.push('0');
      epsilon.push('1');
    } else {
      gamma.push('1');
      epsilon.push('0');
    }
  }

  return u32::from_str_radix(&gamma, 2).unwrap() * 
    u32::from_str_radix(&epsilon, 2).unwrap();
}

fn get_life_support_rating(nums: &Vec<&str>) -> u32 {
  let mut oxy_rating = nums.clone();
  let mut co2_rating = nums.clone();

  for index in 0..nums[0].len() {
    if oxy_rating.len() > 1 {
      let counts: HashMap<char, u32> = 
        count_bits_at_index(&oxy_rating, index);

      let oxy_filter = if counts[&'1'] >= counts[&'0'] { '1' } else { '0' };
      oxy_rating = oxy_rating.iter()
        .filter(|num| num.chars().nth(index).unwrap() == oxy_filter)
        .map(|num| *num)
        .collect::<Vec<&str>>();
    }

    if co2_rating.len() > 1 {
      let counts: HashMap<char, u32> = 
        count_bits_at_index(&co2_rating, index);

      let co2_filter = if counts[&'0'] <= counts[&'1'] { '0' } else { '1' };
      co2_rating = co2_rating.iter()
        .filter(|num| num.chars().nth(index).unwrap() == co2_filter)
        .map(|num| *num)
        .collect::<Vec<&str>>();
    }

    if oxy_rating.len() == 1 && co2_rating.len() == 1 { break; }
  }

  return u32::from_str_radix(oxy_rating[0], 2).unwrap() *
    u32::from_str_radix(co2_rating[0], 2).unwrap();
}

fn main() {
  let contents = fs::read_to_string("./data/q3.txt")
    .expect("Should have been able to read file");

  let nums: Vec<&str> = contents.split('\n').collect::<Vec<&str>>();

  println!("Part 1: {}", get_power_consumption(&nums));
  println!("Part 2: {}", get_life_support_rating(&nums));
}