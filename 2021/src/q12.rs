use std::fs;
use std::collections::{HashMap, HashSet};

fn dfs_unique_paths(
  edges: &HashMap<String, Vec<String>>,
  curr_vertex: &String,
  visited: &HashSet<String>,
) -> u32 {
  if *curr_vertex == "end".to_string() {
    return 1;
  }

  let mut unique_paths: u32 = 0;
  let next_vertices = edges.get(curr_vertex).unwrap();

  for vertex in next_vertices.iter() {
    let mut next_visited = visited.clone();
    if curr_vertex.chars().nth(0).unwrap().is_lowercase() {
      next_visited.insert(curr_vertex.clone());
    }
    if next_visited.contains(vertex) { continue; }

    let curr_paths = dfs_unique_paths(edges, vertex, &next_visited);

    unique_paths += curr_paths;
  }

  return unique_paths;
}

fn count_unique_paths(edges: &HashMap<String, Vec<String>>) -> u32 {
  return dfs_unique_paths(edges, &"start".to_string(), &HashSet::new());
}

fn dfs_unique_paths_visit_small_twice(
  edges: &HashMap<String, Vec<String>>,
  curr_vertex: &String,
  visited: &HashSet<String>,
  visited_twice: &HashSet<String>,
  curr_path: &String
) -> HashSet<String> {
  if *curr_vertex == "end".to_string() {
    let mut path = HashSet::new();
    path.insert(curr_path.clone());
    return path;
  }

  let mut unique_paths: HashSet<String> = HashSet::new();
  let next_vertices = edges.get(curr_vertex).unwrap();

  for vertex in next_vertices.iter() {
    if *vertex == "start".to_string() { continue; }
    if visited.contains(vertex) && !visited_twice.is_empty() { continue; }

    let mut next_visited = visited.clone();
    let mut next_visited_twice = visited_twice.clone();

    if curr_vertex.chars().nth(0).unwrap().is_lowercase() {
      if !next_visited.contains(curr_vertex) {
        next_visited.insert(curr_vertex.clone());
      } else if next_visited_twice.is_empty() {
        next_visited_twice.insert(curr_vertex.clone());
      } else {
        continue;
      }
    }

    let mut next_path = curr_path.clone();
    next_path.insert_str(next_path.len(), &curr_vertex);
    next_path.insert(next_path.len(), ',');

    let curr_paths = dfs_unique_paths_visit_small_twice(edges, 
      vertex, &next_visited, &next_visited_twice, &next_path);

    unique_paths.extend(curr_paths);
  }

  return unique_paths;
}

fn count_unique_paths_visit_small_twice(edges: &HashMap<String, Vec<String>>) -> usize {
  return dfs_unique_paths_visit_small_twice(edges, &"start".to_string(), 
    &HashSet::new(), &HashSet::new(), &String::new()).len();
}

fn main() {
  let contents = fs::read_to_string("./data/q12.txt")
    .expect("Should have been able to read file");

  let mut edges: HashMap<String, Vec<String>> = HashMap::new();

  for line in contents.split('\n') {
    let vertices: Vec<String> = line.split('-').map(|vertex| vertex.to_string())
      .collect();

    let entry = edges.entry(vertices[0].clone())
      .or_insert(Vec::new());
    entry.push(vertices[1].clone());
    let entry_rev = edges.entry(vertices[1].clone())
      .or_insert(Vec::new());
    entry_rev.push(vertices[0].clone());
  }

  println!("Part 1: {}", count_unique_paths(&edges));
  println!("Part 2: {}", count_unique_paths_visit_small_twice(&edges));
}