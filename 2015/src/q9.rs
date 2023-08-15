use std::fs;
use regex::Regex;
use itertools::Itertools;
use std::collections::HashMap;
use std::collections::HashSet;

fn find_shortest_path_through_all_nodes(
    nodes: &HashSet<String>, 
    vertices: &HashMap<String, u32>
) -> u32 
{
  let mut shortest_path: u32 = u32::MAX;

  for perm in nodes.iter().permutations(nodes.len()) {
    let mut curr_length: u32 = 0;

    for i in 0..perm.len() - 1 {
      let curr_vertex = format!("{},{}", perm[i], perm[i + 1]);
      curr_length += vertices.get(&curr_vertex).unwrap();
    }

    if curr_length < shortest_path { shortest_path = curr_length }
  }

  return shortest_path;
}

fn find_longest_path_through_all_nodes(
  nodes: &HashSet<String>, 
  vertices: &HashMap<String, u32>
) -> u32 
{
let mut longest_path: u32 = u32::MIN;

for perm in nodes.iter().permutations(nodes.len()) {
  let mut curr_length: u32 = 0;

  for i in 0..perm.len() - 1 {
    let curr_vertex = format!("{},{}", perm[i], perm[i + 1]);
    curr_length += vertices.get(&curr_vertex).unwrap();
  }

  if curr_length > longest_path { longest_path = curr_length }
}

return longest_path;
}

fn main() {
  let contents = fs::read_to_string("./data/q9.txt")
    .expect("Should have been able to read file");

  let mut vertices: HashMap<String, u32> = HashMap::new();
  let mut nodes: HashSet<String> = HashSet::new();
  let re = Regex::new(r"([A-Za-z]+) to ([A-Za-z]+) = ([\d]+)").unwrap();

  for vertex in contents.split("\n") {
    let caps = re.captures(vertex);
    match caps {
      Some(cap) => {
        let route = format!("{},{}", cap[1].to_string(), cap[2].to_string());
        let backwards_route = format!("{},{}", cap[2].to_string(), cap[1].to_string());
        let distance = cap[3].parse::<u32>().unwrap();
        vertices.insert(route, distance);
        vertices.insert(backwards_route, distance);
        nodes.insert(cap[1].to_string());
        nodes.insert(cap[2].to_string());
      },
      None => println!("Was unable to match line: {}", vertex)
    }
  }

  println!("Part 1: {}", find_shortest_path_through_all_nodes(&nodes, &vertices));
  println!("Part 2: {}", find_longest_path_through_all_nodes(&nodes, &vertices));
}