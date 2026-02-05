use std::cmp::min;
use std::cmp::Reverse;
use std::fs;
use priority_queue::PriorityQueue;

#[derive(Clone)]
struct Range {
  src: u64,
  dst: u64,
  range: u64
}

struct RangeMap {
  ranges: Vec<Range>
}

#[derive(Hash, PartialEq, Eq)]
struct SeedRangeState {
  src: u64,
  range: u64,
  map_index: usize
}

fn translate(seed: &u64, maps: &Vec<RangeMap>) -> u64 {
  let mut val = *seed;

  for rm in maps {
    for range in rm.ranges.iter() {
      if val >= range.src && val <= range.src + range.range {
        val = range.dst + (val - range.src);
        break;
      }
    }
  }
  return val;
}

fn find_smallest_location(seeds: &Vec<u64>, maps: &Vec<RangeMap>) -> u64 {
  let mut smallest_location = u64::MAX;
  for seed in seeds {
    smallest_location = min(smallest_location, translate(seed, maps));
  }
  return smallest_location;
}

fn find_smallest_location_with_seed_range(seeds: &Vec<u64>, maps: &Vec<RangeMap>) -> u64 {
  let mut smallest_location = u64::MAX;
  let mut to_be_checked = PriorityQueue::new();
  for i in (0..seeds.len()).step_by(2) {
    to_be_checked.push(SeedRangeState{src: seeds[i], range: seeds[i + 1], map_index: 0}, Reverse(0));
  }

  while !to_be_checked.is_empty() {
    let (curr_state, _) = to_be_checked.pop().unwrap();

    if curr_state.map_index == 7 {
      smallest_location = min(smallest_location, curr_state.src);
      continue;
    }

    let mut found = false;

    for range in maps[curr_state.map_index].ranges.iter() {
      if curr_state.src >= range.src && curr_state.src < range.src + range.range {
        if curr_state.src + curr_state.range <= range.src + range.range {
          let new_src = range.dst + (curr_state.src - range.src);
          to_be_checked.push(SeedRangeState{src: new_src, range: curr_state.range, map_index: curr_state.map_index + 1},
            Reverse(curr_state.map_index + 1));
          found = true;
          break;

        } else {
          let new_range = range.src + range.range - curr_state.src;
          let leftover_range = curr_state.range - new_range;
          let new_src = range.dst + (curr_state.src - range.src);
          to_be_checked.push(SeedRangeState{src: new_src, range: new_range, map_index: curr_state.map_index + 1},
            Reverse(curr_state.map_index + 1));
          to_be_checked.push(SeedRangeState{src: curr_state.src + new_range, range: leftover_range, map_index: curr_state.map_index},
            Reverse(curr_state.map_index));
          found = true;
          break;
        }

      } else if (curr_state.src + curr_state.range - 1) >= range.src && (curr_state.src + curr_state.range - 1) < range.src + range.range {
        let new_range = curr_state.src + curr_state.range - range.src;
        let leftover_range = curr_state.range - new_range;
        to_be_checked.push(SeedRangeState{src: curr_state.src, range: leftover_range, map_index: curr_state.map_index},
            Reverse(curr_state.map_index));
        to_be_checked.push(SeedRangeState{src: range.dst, range: new_range, map_index: curr_state.map_index + 1},
          Reverse(curr_state.map_index + 1));
        found = true;
        break;
      }
    }

    if !found {
      to_be_checked.push(SeedRangeState{src: curr_state.src, range: curr_state.range, map_index: curr_state.map_index + 1}, 
        Reverse(curr_state.map_index + 1));
    }
  }

  return smallest_location;
}

fn main() {
  let contents = fs::read_to_string("./data/q5.txt")
    .expect("Should have been able to read file");
  let sections = contents.split("\n").collect::<Vec<&str>>();

  let seeds = sections[0].split(' ').skip(1).map(|s| s.parse::<u64>().unwrap())
    .collect::<Vec<u64>>();

  let mut range_maps = Vec::new();
  let mut curr_rm = Vec::new();

  for row in sections.iter().skip(2) {
    if *row == "" {
      range_maps.push(RangeMap{ranges: curr_rm.clone()});
      curr_rm.clear();
    } else if row.chars().next().unwrap().is_digit(10) {
      let vals = row.split(' ').map(|val| val.parse::<u64>().unwrap()).collect::<Vec<u64>>();
      curr_rm.push(Range{ src: vals[1], dst: vals[0], range: vals[2] });
    } else {
      continue;
    }
  }
  range_maps.push(RangeMap{ranges: curr_rm.clone()});

  println!("Part 1: {}", find_smallest_location(&seeds, &range_maps));
  println!("Part 2: {}", find_smallest_location_with_seed_range(&seeds, &range_maps));
}